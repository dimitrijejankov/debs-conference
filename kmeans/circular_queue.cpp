//
// Created by dimitrije on 16/04/17.
//

#include "circular_queue.h"

circular_queue::circular_queue(size_t n) : capacity(n), next(0), full(false) {
    data = new point[n];
}

point &circular_queue::get_point(size_t idx) {
    return data[(idx + next) % capacity];
}

point &circular_queue::next_point() {
    return data[next];
}

bool &circular_queue::is_full() {
    return full;
}

size_t &circular_queue::get_capacity() {
    return capacity;
}

void circular_queue::point_inserted() {
    // set the next point
    next = (next + 1) % capacity;

    // did we fill up the buffer
    full = full || next == 0;
}

circular_queue::~circular_queue() {
    delete[] data;
}
