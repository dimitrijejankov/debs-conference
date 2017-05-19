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

struct task_package {
    vector<task> tasks;
};

class worker_component {
private:

    // timeout
    static const int TIMEOUT = 1000000;

    // the number of preallocate windows
    static const size_t NUM_OF_FREE_WINDOWS = 1000;

    // the package size
    static const size_t PACKAGE_SIZE = 20;

    // number of k-means instances we are running
    static const size_t NUMBER_OF_KMEANS = 4;

    // id of the worker
    int id;

    // tasks
    blocking_reader_writer_queue<task_package> package_tasks;

    // kmeans
    kmeans **detectors;

    // metadata parser
    metadata_parser *mp;

    // output component
    output_component *oc;

    // the current task package
    task_package package;

    // counter for workers
    static int counter;

    // is the system finished
    atomic_bool is_finished;

    // list of free windows we preallocate
    blocking_reader_writer_queue<circular_queue*> free_windows;


public:

    // constructor
    worker_component(output_component *oc, metadata_parser *mp);

    // destructor
    ~worker_component();

    // add the window to the task
    void queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w);

    // flush the task
    void flush();

    // run the worker
    void run();

    // setter for is finished
    void set_is_finished(bool);
};


#endif //HOBBITREWRITE_WORKER_COMPONENT_H
