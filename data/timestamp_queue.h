//
// Created by dimitrije on 4/24/17.
//

#ifndef HOBBITREWRITE_TIMESTAMP_QUEUE_H
#define HOBBITREWRITE_TIMESTAMP_QUEUE_H


#include <cstddef>

class timestamp_queue {
public:

    // constructor
    timestamp_queue(size_t n);

    // copy constructor
    timestamp_queue(timestamp_queue &old);

    // destructor
    ~timestamp_queue();

    // get the entire data
    size_t &get_point(size_t idx);

    // get the entire data
    size_t &next_point();

    // is full
    bool &is_full();

    // get the capacity
    size_t &get_capacity();

    // goes to the next point
    void point_inserted();

private:

    // amqp_queue data
    size_t *data;

    // next position
    size_t next;

    // length
    size_t capacity;

    // full
    bool full;
};


#endif //HOBBITREWRITE_TIMESTAMP_QUEUE_H
