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

struct channel {
    amqp_socket_t *socket;
    amqp_connection_state_t conn;
    int status;
};

class queue {
    string name;
};

class rice_system {
private:

    // the rabbit mq hostname
    string hostname;

    // port
    int port;

    // returns the input queue name
    string input_queue_name();

    // returns the output queue name
    string output_queue_name();

    // the input channel
    channel in_channel;

    // the input queue
    queue in_queue;

    // initialize the channel
    void init_channel(channel &c);

    // initialize the queue
    void init_queue(channel &c, queue &q, string queue_name);

    rdf_parser *parser;

public:

    rice_system();
    ~rice_system();

    // runs the rice_system
    void run();

};


#endif //HOBBITREWRITE_SYSTEM_H
