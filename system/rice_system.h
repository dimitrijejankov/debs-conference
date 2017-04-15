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


class rice_system : public component {
private:

    // returns the input queue name
    string input_queue_name();

    // returns the output queue name
    string output_queue_name();

    // the input channel
    channel in_channel;

    // the input queue
    queue in_queue;

    rdf_parser *parser;

public:

    rice_system();
    ~rice_system();

    // runs the rice_system
    void run();

};


#endif //HOBBITREWRITE_SYSTEM_H
