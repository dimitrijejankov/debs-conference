//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_KMEANS_H
#define HOBBITREWRITE_KMEANS_H

#include "circular_queue.h"
#include "cluster.h"

class kmeans {
private:

    // number of points:
    size_t window_size;

    // maximum iterations for kmeans:
    size_t maximum_iterations;

    // terminal condition for kmeans:
    double clustering_precision;

    // parameters for anomaly detection:
    size_t smaller_window;

    // maximum number of clusters
    size_t cluster_max;

    // threshold
    double threshold;

    // results
    double result_threshold;

    // points
    circular_queue *points;

    // clusters
    cluster *clusters;

    // old clusters
    cluster *old_clusters;

    // number of clusters
    size_t num_clusters;

    // row sum
    int *row_sum;

    // count
    int *count;

    // transition
    double *transition;

    // store transition
    double * store_transition;

    // performs the anomaly detection
    bool perform_anomaly_detection();

    // performs the kmeans clustering
    void perform_clustering();

    // removes all the points from the clusters
    void clear_clusters();

    // assigns points to the clusters
    void assign_cluster();

    // calculate the centroids
    void calculate_centroids();

public:

    // constructor
    kmeans(size_t cluster_max, size_t window_size, size_t maximum_iterations,
           double clustering_precision, size_t smaller_window, double threshold);

    // destructor
    ~kmeans();

    // do the clustering on the window
    bool perform_all_calculation(circular_queue *window);
};


#endif //HOBBITREWRITE_KMEANS_H
