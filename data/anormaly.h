//
// Created by dimitrije on 21/04/17.
//

#ifndef HOBBITREWRITE_ANORMALY_H
#define HOBBITREWRITE_ANORMALY_H

struct anomaly {

    size_t idx;

    size_t machine_no;
    size_t dimension_no;
    double final_threshold;
    size_t timestamp;
};

#endif //HOBBITREWRITE_ANORMALY_H
