//
// Created by dimitrije on 15/04/17.
//

#ifndef HOBBITREWRITE_COMPONENT_H
#define HOBBITREWRITE_COMPONENT_H

#include <amqp.h>
#include <string>

using namespace std;

struct amqp_channel {
    amqp_socket_t *socket;
    amqp_connection_state_t conn;
    int status;
};

class amqp_queue {
    string name;
    string exchange;
};

class component {
public:
    // constructor
    component();

protected:
    // the rabbit mq hostname
    string hostname;

    // port
    int port;

    // initialize the channel
    void init_channel(amqp_channel &c);

    // initialize the amqp_queue
    void init_queue(amqp_channel &c, amqp_queue &q, string queue_name);

    // initialize anonymous amqp_queue
    string init_queue(amqp_channel &c, amqp_queue &q);

    void bind_exchange(amqp_channel &c, amqp_queue &q, string &queue, string &exchange);
};


#endif //HOBBITREWRITE_COMPONENT_H
