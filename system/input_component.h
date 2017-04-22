//
// Created by dimitrije on 15/04/17.
//

#ifndef HOBBITREWRITE_INPUT_THREAD_H
#define HOBBITREWRITE_INPUT_THREAD_H

#include <condition_variable>
#include "component.h"
#include "rdf_parser.h"

class input_component : public component {
protected:
    // returns the input amqp_queue name
    string input_queue_name();

    // the input channel
    amqp_channel in_channel;

    // the input amqp_queue
    amqp_queue in_queue;

    // the rdf parser
    rdf_parser *parser;

    // finished
    bool finished;

public:

    // constructor
    input_component(function<void(int, int, int, double)> callback);

    // destructor
    ~input_component();

    // runs the system
    void run(condition_variable &cv, mutex &m);

    bool is_finished();
};


#endif //HOBBITREWRITE_INPUT_THREAD_H
