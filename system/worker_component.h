//
// Created by dimitrije on 21/04/17.
//

#ifndef HOBBITREWRITE_WORKER_COMPONENT_H
#define HOBBITREWRITE_WORKER_COMPONENT_H


#include <cstddef>
#include <list>
#include <kmeans/circular_queue.h>
#include <mutex>
#include <condition_variable>
#include <kmeans/kmeans.h>
#include <metadata_parser.h>
#include "output_component.h"

using namespace std;

struct task {
    size_t idx;
    size_t machine_no;
    size_t dimension_no;
    size_t timestamp;
    circular_queue *w;
};

class worker_component {
private:

    // tasks
    list<task> tasks;

    // the mutex to sync the tasks
    mutex m;

    // conditional variable
    condition_variable c;

    // kmeans
    kmeans detector;

    // metadata parser
    metadata_parser *mp;

    // output component
    output_component *oc;

public:

    // constructor
    worker_component(output_component *oc, metadata_parser *mp);

    // add the window to the task
    void queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w);

    // run the worker
    void run();
};


#endif //HOBBITREWRITE_WORKER_COMPONENT_H
