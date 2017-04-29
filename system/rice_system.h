//
// Created by dimitrije on 14/04/17.
//

#ifndef HOBBITREWRITE_SYSTEM_H
#define HOBBITREWRITE_SYSTEM_H

#include <string>
#include <thread>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <sys/socket.h>
#include <metadata_parser.h>
#include "rdf_parser.h"
#include "utils.h"
#include "component.h"
#include "input_component.h"
#include "command_component.h"
#include "output_component.h"
#include "worker_component.h"

class rice_system {
private:

    // the input component
    input_component *ic;

    // the output component
    output_component *oc;

    // the command component
    command_component *cr;

    // metadata parser
    metadata_parser mp;

    // mutex to sync the system
    std::mutex m;

    // conditional variable to sync the system
    std::condition_variable cv;

    // workers
    vector<worker_component*> workers;

    // threads for all the workers
    vector<std::thread*> workers_threads;

public:

    rice_system();
    ~rice_system();

    // runs the rice_system
    void run();

};


#endif //HOBBITREWRITE_SYSTEM_H
