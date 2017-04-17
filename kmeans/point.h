//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_POINT_H
#define HOBBITREWRITE_POINT_H

#include <cmath>

// represents one point
struct point {
    double x;
    int center;
};


// get the euclidean distance of two points
inline double x (point &a, point &b) {
    return fabs(a.x - b.x);
};



#endif //HOBBITREWRITE_POINT_H
