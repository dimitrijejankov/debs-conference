//
// Created by dimitrije on 17/04/17.
//

#include "cluster.h"

cluster::cluster() : n(0) {}

cluster::~cluster() {
    delete[] points;
}

void cluster::init(size_t capacity) {
    this->points = new point[capacity];
    this->capacity = capacity;
}

size_t &cluster::get_id() {
    return id;
}

point &cluster::get_point(size_t &idx) {
    return points[idx];
}

point &cluster::get_centroid() {
    return centroid;
}

size_t &cluster::get_points_num() {
    return n;
}

point &cluster::next_point() {
    return points[n + 1];
}

void cluster::point_added() {
    n = n + 1;
}

