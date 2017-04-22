//
// Created by dimitrije on 14/04/17.
//

#ifndef HOBBITREWRITE_SYSTEM_H
#define HOBBITREWRITE_SYSTEM_H

#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <sys/socket.h>
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
    output_component oc;

    // the command component
    command_component cr;

    // mutex to sync the system
    std::mutex m;

    // conditional variable to sync the system
    std::condition_variable cv;

    // workers
    vector<worker_component*> workers;

public:

    rice_system();
    ~rice_system();

    // runs the rice_system
    void run();

};


#endif //HOBBITREWRITE_SYSTEM_H
