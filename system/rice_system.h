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

using namespace std;

class rice_system {
private:

    // returns the output queue name
    string output_queue_name();

    // the input component
    input_component ic;

    // the command receiver
    command_component cr;

    // mutex to sync the system
    mutex m;

    // conditional variable to sync the system
    condition_variable cv;

public:

    rice_system();
    ~rice_system();

    // runs the rice_system
    void run();

};


#endif //HOBBITREWRITE_SYSTEM_H
