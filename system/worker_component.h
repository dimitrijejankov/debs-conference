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

    // number of clusters
    size_t num_clusters = 5;

    // Other Fixed Parameters:
    size_t window_size = 10;

    // maximum number of iterations
    size_t maximum_iterations = 50;

    // cluster precision
    double clustering_precision = 0.00001;

    // smaller window size
    size_t smaller_window_size = 5;

    // threshold probability
    double threshold_probability = 0.005;

    // output component
    output_component *oc;

public:

    // constructor
    worker_component(output_component *oc);

    // add the window to the task
    void queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w);

    // run the worker
    void run();
};


#endif //HOBBITREWRITE_WORKER_COMPONENT_H
