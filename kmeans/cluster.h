//
// Created by dimitrije on 17/04/17.
//

#ifndef HOBBITREWRITE_CLUSTER_H
#define HOBBITREWRITE_CLUSTER_H


#include <cstdio>
#include "point.h"

class cluster {
private:
    // the identifier
    size_t id;

    // the points of the cluster
    point* points;

    // the centroid
    point centroid;

    // current number of points
    size_t n;

    // maximum number of points
    size_t capacity;

public:

    // constructor
    cluster();

    // destructor
    ~cluster();

    // init the cluster with capacity
    void init(size_t capacity);

    // get the id of the cluster
    size_t &get_id();

    // get the point
    point &get_point(size_t &idx);

    // get the centroid
    point &get_centroid();

    // get the current number of points
    size_t &get_points_num();

    // increase the current number of points
    point &current_point();

    // call after point is added
    void point_added();
};


#endif //HOBBITREWRITE_CLUSTER_H
