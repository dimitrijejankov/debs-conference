//
// Created by dimitrije on 17/04/17.
//

#include "cluster.h"

cluster::cluster() : n(0) {}

cluster::~cluster() {
    delete[] points;
}

void cluster::init(size_t capacity) {
    this->capacity = capacity;
    this->points = new point[capacity];
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

point &cluster::current_point() {
    return points[n];
}

void cluster::point_added() {
    n = n + 1;
}
