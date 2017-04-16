//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_OUTPUT_COMPONENT_H
#define HOBBITREWRITE_OUTPUT_COMPONENT_H

#include <mutex>
#include "component.h"

class output_component : public component {
private:

    // returns the output queue name
    string output_queue_name();

    // the response channel
    channel out_channel;

    // the response queue
    queue out_queue;

    mutex m;

public:

    // constructor
    output_component();

    // destructor
    ~output_component();

    // send the bytes
    void send(const char* bytes, size_t length);

    // send the string
    void send(const string value);
};


#endif //HOBBITREWRITE_OUTPUT_COMPONENT_H
