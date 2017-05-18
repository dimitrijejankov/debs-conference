//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_CIRCULAR_QUEUE_H
#define HOBBITREWRITE_CIRCULAR_QUEUE_H

#include <cstdio>
#include <iostream>
#include "point.h"

class circular_queue {
public:

    // constructor
    circular_queue(size_t n);

    // copy constructor
    circular_queue(circular_queue &old);

    ~circular_queue();

    // get the entire data
    point & get_point(size_t idx);

    // get the entire data
    point &next_point();

    // is full
    bool &is_full();

    // get the capacity
    size_t &get_capacity();

    // goes to the next point
    void point_inserted();

    // display the queue
    void display();

    // returns the hash of this window
    uint64_t get_hash();

    bool operator==(circular_queue &rhs);

    bool operator!=(circular_queue &rhs);

private:

    // amqp_queue data
    point* data;

    // next position
    size_t next;

    // length
    size_t capacity;

    // full
    bool full;
};


#endif //HOBBITREWRITE_CIRCULAR_QUEUE_H
