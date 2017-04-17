//
// Created by dimitrije on 17/04/17.
//

#include "cluster.h"

cluster::cluster(size_t capacity) : capacity(capacity), n(0) {
    points = new point[capacity];
}

cluster::~cluster() {
    delete[] points;
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

