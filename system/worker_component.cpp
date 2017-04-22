//
// Created by dimitrije on 21/04/17.
//

#include "worker_component.h"

void worker_component::queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w) {

    // lock the mutex
    unique_lock<mutex> lq;

    static task tmp;

    // set the index
    tmp.idx = idx;

    // set the machine number
    tmp.machine_no = machine_no;

    // set the dimension
    tmp.dimension_no = dimension_no;

    // set the timestamp
    tmp.timestamp = timestamp;

    // copy the window
    tmp.w = new circular_queue(*w);

    // push the task
    tasks.push_back(tmp);

    // notify the worker that it a new task
    c.notify_one();
}

void worker_component::run() {

    // lock the mutex
    unique_lock<mutex> lq(m);

    for(;;) {

        // wait until we have something in the task queu
        c.wait(lq, [this]{return !this->tasks.empty();});

        // grab the first task
        task tmp = tasks.front();

        // remove the first task
        tasks.pop_front();

        // we grabbed the task
        lq.unlock();

        // preform calculations
        bool detected = detector.perform_all_calculation(tmp.w, num_clusters);

        // if we have detected an anomaly
        if(detected) {
            oc->output_anomaly(tmp.idx, tmp.machine_no, tmp.dimension_no, detector.get_result_threshold(), tmp.timestamp);
        }

        /// TODO update so we don't have to do this
        delete tmp.w;

        // lock the thing again
        lq.lock();
    }
}

worker_component::worker_component(output_component *oc) : oc(oc), detector(10, 50, 0.00001, 5, 0.005) {}
