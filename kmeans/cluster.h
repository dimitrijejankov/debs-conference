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
    cluster(size_t n);

    // destructor
    ~cluster();

    // get the id of the cluster
    inline size_t &get_id();

    // get the point
    inline point &get_point(size_t &idx);

    // get the centroid
    inline point &get_centroid();

    // get the current number of points
    inline size_t &get_points_num();

    // increase the current number of points
    inline point &next_point();

    // call after point is added
    inline void point_added();
};


#endif //HOBBITREWRITE_CLUSTER_H
