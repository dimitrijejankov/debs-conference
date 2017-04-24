//
// Created by dimitrije on 4/24/17.
//

#include "timestamp_queue.h"
#include "memory.h"

timestamp_queue::timestamp_queue(size_t n) : capacity(n), next(0), full(false) {
    data = new size_t[n];
}

size_t &timestamp_queue::get_point(size_t idx) {
    return data[(idx + next) % capacity];
}

size_t &timestamp_queue::next_point() {
    return data[next];
}

bool &timestamp_queue::is_full() {
    return full;
}

size_t &timestamp_queue::get_capacity() {
    return capacity;
}

void timestamp_queue::point_inserted() {
    // set the next point
    next = (next + 1) % capacity;

    // did we fill up the buffer
    full = full || next == 0;
}

timestamp_queue::~timestamp_queue() {
    delete[] data;
}

timestamp_queue::timestamp_queue(timestamp_queue &old)  : capacity(old.capacity), next(old.next), full(old.full) {

    // make new new points
    data = new size_t[capacity];

    // copy the memory
    memcpy(data, old.data, capacity * sizeof(size_t));
}
