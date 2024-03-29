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

void circular_queue::copy(circular_queue *rhs) {

    // copy the points
    memcpy(data, rhs->data, sizeof(point) * capacity);

    // next position
    next = rhs->next;

    // length
    capacity = rhs->capacity;

    // full
    full = rhs->full;
}
