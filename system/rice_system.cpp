//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "rice_system.h"
#include "parameters.h"


string rice_system::output_queue_name() {
    return SYSTEM_2_EVAL_STORAGE_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

rice_system::rice_system() {

}

void rice_system::run() {

    // the mutex to sync the system
    unique_lock<mutex> lk(m);

    // run the input component
    std::thread input_thread ([this]() {
        this->ic.run(this->cv, this->m);
    });

    // run the input component
    std::thread command_thread ([this]() {
        this->cr.run();
    });

    // send the system ready signal
    cr.send_to_cmd_queue(SYSTEM_READY_SIGNAL);

    // make a conditional variable flow until we finish...
    cv.wait(lk, [this]{return this->ic.is_finished();});

    // TODO send TERMINATION_MESSAGE to the output queue
    // output(TERMINATION_MESSAGE)
}

rice_system::~rice_system() {}
