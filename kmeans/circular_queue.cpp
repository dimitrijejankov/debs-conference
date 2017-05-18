//
// Created by dimitrije on 16/04/17.
//

#include "circular_queue.h"
#include "memory.h"

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


void circular_queue::display() {
    for (int i = 0; i < capacity; i++) {
        std::cout << data[(i + next) % capacity].x << "\t";
    }
    std::cout << std::endl;
}


circular_queue::~circular_queue() {
    delete[] data;
}

circular_queue::circular_queue(circular_queue &old)  : capacity(old.capacity), next(old.next), full(old.full) {

    // make new new points
    data = new point[capacity];

    // copy the memory
    memcpy(data, old.data, capacity * sizeof(point));
}

uint64_t circular_queue::get_hash() {

    uint64_t ret = 0;

    // hash the data...
    for(uint64_t i = 0; i < capacity; ++i) {
        ret += ((*(uint64_t*)&get_point(i).x) * (6*i+1));
    }

    return ret;
}

bool circular_queue::operator==(circular_queue &rhs) {

    // check it point by point
    for(size_t i = 0; i < capacity; ++i) {
        if(*((uint64_t*)&get_point(i).x) != *((uint64_t*)&rhs.get_point(i).x)) {
            return false;
        }
    }

    return true;
}

bool circular_queue::operator!=(circular_queue &rhs) {
    return !(rhs == *this);
}

