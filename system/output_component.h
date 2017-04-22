//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_OUTPUT_COMPONENT_H
#define HOBBITREWRITE_OUTPUT_COMPONENT_H

#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include "component.h"

struct anomaly {

    size_t index;

    size_t machine_no;
    size_t dimension_no;
    double final_threshold;
    size_t timestamp;
};

class output_component : public component {
private:

    // returns the output amqp_queue name
    string output_queue_name();

    // the response channel
    amqp_channel out_channel;

    // the response amqp_queue
    amqp_queue out_queue;

    // mutex to sync the output
    mutex m;

    // mutex to sync the buffer
    mutex m_b;

    // condition variable to sync the buffer
    condition_variable c_b;

    // buffer for the anomalies
    priority_queue<anomaly, vector<anomaly>, std::function<bool(anomaly&, anomaly&)>> *buffer;

public:

    // constructor
    output_component();

    // destructor
    ~output_component();

    void output_anomaly(size_t idx, size_t machine_no, size_t dimension_no, double final_threshold, size_t timestamp);

    // send the bytes
    void send(const char* bytes, size_t length);

    // send the string
    void send(const string value);

    // run the component
    void run();
};


#endif //HOBBITREWRITE_OUTPUT_COMPONENT_H
