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

    //static int cache_miss = 0;
    //static int cache_hit = 0;
    //static int noneq_miss = 0;

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

        // true if this is an anomaly;
        bool detected;

        // the probability if detected
        double prob;

        // cache result to grab if there is one
        lru::cache_result r;

        lru::cache* cache = caches[tmp.dimension_no + tmp.machine_no * 10000];

        auto m = tmp.w->get_hash();

        // check if the result is cached
        bool is_cached = cache->tryGet(tmp.w->get_hash(), r);

        // if we have a cached result
        if(is_cached) {

            // if the cached result matches this one exactly
            if(*tmp.w == *r.w) {
                detected = r.is_anomaly;
                prob = r.prob;
                //cache_hit++;
            }
            else {
                detected = detector.perform_all_calculation(tmp.w, mp->get_cluster_no(tmp.machine_no)[tmp.dimension_no]);
                prob = detector.get_result_threshold();
                //noneq_miss++;
            }
        }
        else {
            // preform calculations
            detected = detector.perform_all_calculation(tmp.w, mp->get_cluster_no(tmp.machine_no)[tmp.dimension_no]);
            prob = detector.get_result_threshold();
            //cache_miss++;
        }

        // if we have detected an anomaly
        if(detected) {
            oc->output_anomaly(tmp.idx, tmp.machine_no, tmp.dimension_no, prob, tmp.timestamp);
        }

        // set the hash result values
        r.w = tmp.w;
        r.is_anomaly = detected;
        r.prob = prob;

        auto x = r.w->get_hash();

        // insert the hashed value
        cache->insert(x, r);
    }

   //cout << "cache_miss : " << cache_miss << "Non equal misses" << noneq_miss << " cache_hit " << cache_hit << endl;
}

worker_component::worker_component(output_component *oc, metadata_parser *mp) : oc(oc), mp(mp), tasks(300), is_finished(false), detector(mp->get_window_size(),
                                                                                                                                mp->get_max_clustering_iterations(),
                                                                                                                                mp->get_clustering_precision(),                                                                                                              mp->get_transitions_amount(),
                                                                                                                                mp->get_threshold()) {
    id = counter++;

    for(auto it : mp->getNumber_of_clusters()) {
        for(int i = 0; i < 121; i++) {
            if(it.second[i] != 0) {
                caches[i + it.first * 10000] = new lru::cache();
            }
        }
    }

}

void worker_component::set_is_finished(bool value) {
    is_finished = value;
}

worker_component::~worker_component() {
    for(auto it : caches) {
        delete it.second;
    }
}
