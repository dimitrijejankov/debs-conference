//
// Created by dimitrije on 21/04/17.
//

#ifndef HOBBITREWRITE_WINDOW_MANAGER_H
#define HOBBITREWRITE_WINDOW_MANAGER_H

#define DIMENSIONS_PER_MACHINE 1000

#include <unordered_map>
#include <vector>
#include <list>
#include <map>

#include "system/worker_component.h"
#include "kmeans/circular_queue.h"
#include "data/timestamp_queue.h"

using namespace std;

class window_manager {
private:
    // window size for all the windows
    size_t window_size;

    // smaller window size
    size_t smaller_window_size;

    // all the windows
    unordered_map<uint64_t, circular_queue*> windows;

    // timestamps
    map<size_t, timestamp_queue*> timestamps;

    // current order index
    uint64_t idx;

    // workers
    vector<worker_component*> workers;

    // figure out the hash for the anomaly
    inline uint64_t calculate_hash(uint64_t &machine, uint64_t &dimension) {
        return DIMENSIONS_PER_MACHINE * machine + dimension;
    }

public:

    // constructor
    window_manager(size_t window_size, size_t smaller_window_size, vector<worker_component*> &workers);

    // destructor
    ~window_manager();

    // add a new anomaly
    void push_data(size_t machine_no, size_t dimension_no, size_t timestamp, double value);

    // saves the timestamp
    void save_timestamp(size_t hash, size_t timestamp);
};


#endif //HOBBITREWRITE_WINDOW_MANAGER_H
