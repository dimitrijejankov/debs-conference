//
// Created by dimitrije on 21/04/17.
//

#include <worker_component.h>
#include <vector>
#include "window_manager.h"

window_manager::window_manager(size_t window_size, size_t smaller_window_size, vector<worker_component*> &workers) : window_size(window_size), smaller_window_size(smaller_window_size), workers(workers) {}

window_manager::~window_manager() {

    // remove all the windows...
    for(auto it = windows.begin(); it != windows.end(); it++) {
        delete it->second;
    }

    // remove all the timestamps...
    for(auto it = timestamps.begin(); it != timestamps.end(); it++) {
        delete it->second;
    }
}

void window_manager::push_data(size_t machine_no, size_t dimension_no, size_t timestamp, double value) {

    // calculate the hash
    uint64_t hash = calculate_hash(machine_no, dimension_no);

    // store the incoming timestamp
    save_timestamp(hash, timestamp);

    // check if we have a window for this guy
    if(windows.find(hash) == windows.end()){

        // make a new window
        circular_queue *w = new circular_queue(window_size);

        // set the next point
        w->next_point().x = value;

        // inform the windows that we have inserted
        w->point_inserted();

        // add the windows to the queue
        windows[hash] = w;
    }
    else {

        // grab the queue
        circular_queue *w = windows[hash];

        // set the next point
        w->next_point().x = value;

        // inform the windows that we have inserted
        w->point_inserted();

        if(w->is_full()) {
            // run the kmeans on one of the workers
            workers[hash % workers.size()]->queue_task(idx++, machine_no, dimension_no, timestamps[hash]->get_point(window_size- smaller_window_size - 1), w);
        }
    }
}

void window_manager::save_timestamp(size_t hash, size_t timestamp) {
    if(timestamps.find(hash) == timestamps.end()) {
        // make a new timestamp queue
        timestamp_queue *tq = new timestamp_queue(window_size);

        // set the next point
        tq->next_point() = timestamp;

        // inform the windows that we have inserted
        tq->point_inserted();

        // add the windows to the queue
        timestamps[hash] = tq;
    }
    else {
        // grab the queue
        timestamp_queue *tq = timestamps[hash];

        // set the next point
        tq->next_point() = timestamp;

        // inform the windows that we have inserted
        tq->point_inserted();
    }
}
