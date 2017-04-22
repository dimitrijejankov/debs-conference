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

    circular_queue(size_t n);

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

    // display the amqp_queue
    void display();


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
