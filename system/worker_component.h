//
// Created by dimitrije on 21/04/17.
//

#ifndef HOBBITREWRITE_WORKER_COMPONENT_H
#define HOBBITREWRITE_WORKER_COMPONENT_H

#define NUMBER_OF_WORKERS 1

#include <cstddef>
#include <list>
#include <kmeans/circular_queue.h>
#include <mutex>
#include <condition_variable>
#include <kmeans/kmeans.h>
#include <metadata_parser.h>
#include <data/readerwriterqueue.h>
#include "output_component.h"

using namespace std;
using namespace concurent;

struct task {
    size_t idx;
    size_t machine_no;
    size_t dimension_no;
    size_t timestamp;
    circular_queue *w;
};

class worker_component {
private:

    // timeout
    static const int TIMEOUT = 5000000;

    // id of the worker
    int id;

    // tasks
    blocking_reader_writer_queue<task> tasks;

    // kmeans
    kmeans detector;

    // metadata parser
    metadata_parser *mp;

    // output component
    output_component *oc;

    // counter for workers
    static int counter;

    // is the system finished
    atomic_bool is_finished;

public:

    // constructor
    worker_component(output_component *oc, metadata_parser *mp);

    // add the window to the task
    void queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w);

    // run the worker
    void run();

    // setter for is finished
    void set_is_finished(bool);
};


#endif //HOBBITREWRITE_WORKER_COMPONENT_H
