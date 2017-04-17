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
const int NUMPOINTS = -1;

// maximum iterations for kmeans:
const int MAXITERATIONS = -1;

// terminal condition for kmeans:
const double CLUSTERINGPRECISION = 0.0;

// parameters for anomaly detection:
const int SMALLERWINDOW = -1;

// threshold
const double THRESHOLD = 0.0;

class kmeans {
private:

    // results
    double result_threshold;

    // points
    circular_queue *points;

    cluster *clusters;

    bool perform_anomaly_detection();

    void perform_clustering();

    void clear_clusters();

    void assign_cluster();

    void calculate_centroids();

    void get_centroids();

public:

    bool perform_all_calculation(circular_queue *window);

    kmeans(double result_threshold);
};


#endif //HOBBITREWRITE_KMEANS_H
