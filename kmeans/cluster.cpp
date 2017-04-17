//
// Created by dimitrije on 17/04/17.
//

#include "cluster.h"

cluster::cluster(size_t n) : n(n) {
    points = new point[n];
}

cluster::~cluster() {
    delete[] points;
}

int &cluster::get_id() {
    return id;
}

point &cluster::get_point(size_t &idx) {
    return points[idx];
}

point &cluster::get_centroid() {
    return centroid;
}
