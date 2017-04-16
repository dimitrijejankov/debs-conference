//
// Created by dimitrije on 15/04/17.
//

#ifndef HOBBITREWRITE_COMPONENT_H
#define HOBBITREWRITE_COMPONENT_H

#include <amqp.h>
#include <string>

using namespace std;

struct channel {
    amqp_socket_t *socket;
    amqp_connection_state_t conn;
    int status;
};

class queue {
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
    void init_channel(channel &c);

    // initialize the queue
    void init_queue(channel &c, queue &q, string queue_name);

    // initialize anonymous queue
    string init_queue(channel &c, queue &q);

    void bind_exchange(channel &c, queue &q, string &queue, string &exchange);
};


#endif //HOBBITREWRITE_COMPONENT_H
