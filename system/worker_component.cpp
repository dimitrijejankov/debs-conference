//
// Created by dimitrije on 21/04/17.
//

#include <omp.h>
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
    free_windows.wait_dequeue(tmp.w);

    // copy the window to the acquired free space
    tmp.w->copy(w);

    // add the task to the package
    package.tasks.push_back(tmp);

    // flush the package if it's size is big enough
    if(package.tasks.size() == PACKAGE_SIZE) {
        flush();
    }
}

void worker_component::run() {

    // for task grabbing
    task_package tmp;

    for(;;) {

        // grab the first task with timeout of 1 seconds
        bool grabbed = package_tasks.wait_dequeue_timed(tmp, TIMEOUT);

        // if the system is finished and there is nothing in the queue finish the thread
        if(!grabbed && is_finished) {
            break;
        }

        // if we don't have an anomaly continue
        if(!grabbed) {
            continue;
        }

        #pragma omp parallel for ordered schedule(dynamic) num_threads(NUMBER_OF_KMEANS)
        for(size_t i = 0; i < tmp.tasks.size(); ++i) {

            // grab the task
            task &t = tmp.tasks[i];

            // find k-means for this thread
            kmeans & detector = *detectors[omp_get_thread_num()];

            // preform calculations
            bool detected = detector.perform_all_calculation(t.w, mp->get_cluster_no(t.machine_no)[t.dimension_no]);

            // if we have detected an anomaly
            #pragma omp ordered
            if (detected) {
                oc->output_anomaly(t.idx, t.machine_no, t.dimension_no, detector.get_result_threshold(),
                                   t.timestamp);
            }
        }

        // free all tasks in the package
        for(task t : tmp.tasks) {
            // return the window
            free_windows.enqueue(t.w);
        }
    }
}

worker_component::worker_component(output_component *oc, metadata_parser *mp) : oc(oc), mp(mp), package_tasks(300), is_finished(false) {

    // allocate the array of k-means pointers
    detectors = new kmeans*[NUMBER_OF_KMEANS];

    // allocate the k-means
    for(int i = 0; i < NUMBER_OF_KMEANS; ++i) {
        detectors[i] = new kmeans(mp->get_window_size(),
                                  mp->get_max_clustering_iterations(),
                                  mp->get_clustering_precision(),
                                  mp->get_transitions_amount(),
                                  mp->get_threshold());
    }

    // increase the id
    id = counter++;

    // preallocate all windows
    for(int i = 0; i < NUM_OF_FREE_WINDOWS; ++i) {
        free_windows.enqueue(new circular_queue(mp->get_window_size()));
    }

}

void worker_component::set_is_finished(bool value) {
    is_finished = value;
}

worker_component::~worker_component() {

    circular_queue* tmp;

    // deallocate all free windows
    while(free_windows.try_dequeue(tmp)) {
        delete tmp;
    }

    // deallocate all the k-means
    for(int i = 0; i < NUMBER_OF_KMEANS; ++i) {
        delete detectors[i];
    }

    // free all kmeans pointers
    delete[] detectors;
}

void worker_component::flush() {

    // if the package is not empty
    if(!package.tasks.empty()) {
        // enqueue the package
        package_tasks.enqueue(package);

        // clear the package
        package.tasks.clear();
    }
}
