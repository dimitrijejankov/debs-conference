//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_KMEANS_H
#define HOBBITREWRITE_KMEANS_H

#include "circular_queue.h"
#include "cluster.h"

// number of clusters. this metric should be related to the number of points
const size_t NUMCLUSTERS = 0;

// number of points:
const size_t NUMPOINTS = 0;

// maximum iterations for kmeans:
const size_t MAXITERATIONS = 0;

// terminal condition for kmeans:
const double CLUSTERINGPRECISION = 0.0;

// parameters for anomaly detection:
const size_t SMALLERWINDOW = 0;

// threshold
const double THRESHOLD = 0.0;

class kmeans {
private:

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
    kmeans();

    // destructor
    ~kmeans();

    // do the clustering on the window
    bool perform_all_calculation(circular_queue *window);
};


#endif //HOBBITREWRITE_KMEANS_H
