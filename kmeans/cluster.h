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
    int id;

    // the points of the cluster
    point* points;

    // the centroid
    point centroid;

    // maximum number of points
    size_t n;

public:

    // constructor
    cluster(size_t n);

    // destructor
    ~cluster();

    // get the id of the cluster
    int &get_id();

    // get the point
    point &get_point(size_t &idx);

    // get the centroid
    point &get_centroid();
};


#endif //HOBBITREWRITE_CLUSTER_H
