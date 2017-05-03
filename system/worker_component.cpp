//
// Created by dimitrije on 21/04/17.
//

#include "worker_component.h"

int worker_component::counter = 0;

void worker_component::queue_task(size_t idx, size_t machine_no, size_t dimension_no, size_t timestamp, circular_queue *w) {

    task tmp;

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
    tasks.enqueue(tmp);
}

void worker_component::run() {

    // for task grabbing
    task tmp;

    for(;;) {

        // grab the first task with timeout of 1 seconds
        bool grabbed = tasks.wait_dequeue_timed(tmp, TIMEOUT);

        // if the system is finished and there is nothing in the queue finish the thread
        if(!grabbed && is_finished) {
            break;
        }

        // if we don't have an anomaly continue
        if(!grabbed) {
            continue;
        }

        // preform calculations
        bool detected = detector.perform_all_calculation(tmp.w, mp->get_cluster_no(tmp.machine_no)[tmp.dimension_no]);

        // if we have detected an anomaly
        if(detected) {
            oc->output_anomaly(tmp.idx, tmp.machine_no, tmp.dimension_no, detector.get_result_threshold(), tmp.timestamp);
        }

        delete tmp.w;
    }
}

worker_component::worker_component(output_component *oc, metadata_parser *mp) : oc(oc), mp(mp), tasks(300), is_finished(false), detector(mp->get_window_size(),
                                                                                                                                mp->get_max_clustering_iterations(),
                                                                                                                                mp->get_clustering_precision(),
                                                                                                                                mp->get_transitions_amount(),
                                                                                                                                mp->get_threshold()) { id = counter++; }

void worker_component::set_is_finished(bool value) {
    is_finished = value;
}
