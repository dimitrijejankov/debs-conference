//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <parameters.h>
#include "rice_system.h"

rice_system::rice_system() {

    // output component
    oc = new output_component();

    // add the workers
    for(int i = 0; i < NUMBER_OF_WORKERS; i++) {
        workers.push_back(new worker_component(oc, &mp));
    }

    //TODO fix the windows sizes
    ic = new input_component(&mp, workers);

    // command component
    cr = new command_component();
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
        this->cr->run();
    });

    // detach the command
    command_thread.detach();

    // run the output component
    std::thread output_thread ([this]() {
        this->oc->run();
    });

    // detach the command
    output_thread.detach();

    worker_component *tmp = *workers.begin();

    // run each worker
    for(auto w = workers.begin(); w != workers.end(); w++) {

        // get the pointer for the worker
        tmp = *w;

        // run the worker
        std::thread t ([tmp]() {
            tmp->run();
        });

        // detach the thread
        t.detach();
    }

    // log the action
    printf("Sending SYSTEM_READY_SIGNAL...\n");

    // send the system ready signal
    cr->send_to_cmd_queue(SYSTEM_READY_SIGNAL);

    printf("Waiting for termination message...\n");

    // make a conditional variable flow until we finish...
    cv.wait(lk, [this]{return this->ic->is_finished();});

    printf("Sending termination message...\n");
    oc->send(TERMINATION_MESSAGE);
}

rice_system::~rice_system() {

    // free the workers
    for(auto it = workers.begin(); it != workers.end(); it++) {
        delete *it;
    }

    // free the input component
    delete ic;

    // free the output component
    delete oc;

    // free the command component
    delete cr;
}
