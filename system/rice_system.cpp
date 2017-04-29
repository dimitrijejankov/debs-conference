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

    // run the command component
    std::thread command_thread ([this]() {
        this->cr->run();
    });

    // run the output component
    std::thread output_thread ([this]() {
        this->oc->run();
    });

    worker_component *tmp = *workers.begin();

    // run each worker
    for(auto w = workers.begin(); w != workers.end(); w++) {

        // get the pointer for the worker
        tmp = *w;

        // run the worker
        std::thread *t = new std::thread ([tmp]() {
            tmp->run();
        });

        // store the pointer to the thread
        workers_threads.push_back(t);
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

    // notify the command component that we have finished
    cr->set_is_finished(true);

    // notify all worker components
    for(auto it = workers.begin(); it != workers.end(); ++it) {
        (*it)->set_is_finished(true);
    }

    // notify the output component
    oc->set_is_finished(true);

    // join the command thread.
    command_thread.join();

    // join the input thread...
    input_thread.join();

    // join all the workers threads
    for(auto it = workers_threads.begin(); it != workers_threads.end(); ++it) {
        (*it)->join();
    }

    // join the output component
    output_thread.join();
}

rice_system::~rice_system() {


    // free the input component
    delete ic;

    // free the workers
    for(auto it = workers.begin(); it != workers.end(); it++) {
        delete *it;
    }

    // free the output component
    delete oc;

    // free the command component
    delete cr;

    // join all the workers threads
    for(auto it = workers_threads.begin(); it != workers_threads.end(); ++it) {
        delete (*it);
    }
}
