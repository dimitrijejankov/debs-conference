//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "rice_system.h"
#include "parameters.h"


rice_system::rice_system() {
    ic = new input_component([](int machine_idx, int dimension, int timestamp_idx, double value) {
        printf("Machine index %d, dimension %d, timestamp %d, value %f\n", machine_idx, dimension, timestamp_idx, value);
    });
}

void rice_system::run() {

    // the mutex to sync the system
    unique_lock<mutex> lk(m);

    // run the input component
    std::thread input_thread ([this]() {
        this->ic->run(this->cv, this->m);
    });

    // detach the input thread
    input_thread.detach();

    // run the command component
    std::thread command_thread ([this]() {
        this->cr.run();
    });

    // detach the command
    command_thread.detach();

    // log the action
    printf("Sending SYSTEM_READY_SIGNAL...\n");

    // send the system ready signal
    cr.send_to_cmd_queue(SYSTEM_READY_SIGNAL);

    printf("Waiting for termination message...\n");

    // make a conditional variable flow until we finish...
    cv.wait(lk, [this]{return this->ic->is_finished();});

    printf("Sending termination message...\n");
    oc.send(TERMINATION_MESSAGE);
}

rice_system::~rice_system() {
    delete ic;
}
