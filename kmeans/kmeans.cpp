//
// Created by dimitrije on 16/04/17.
//

#include "kmeans.h"
#include <set>
#include <cfloat>

using namespace std;

kmeans::kmeans(size_t window_size, size_t maximum_iterations, double clustering_precision, size_t smaller_window, double threshold, size_t num_clusters) :
        window_size(window_size), maximum_iterations(maximum_iterations),
        clustering_precision(clustering_precision), smaller_window(smaller_window), threshold(threshold), num_clusters(num_clusters) {

    // allocate the memory for the clusters
    clusters = new cluster[window_size];

    // allocate the memory for the old clusters
    old_clusters = new cluster[window_size];

    for(int i = 0; i < window_size; i++) {
        // allocate the memory for the clusters
        clusters[i].init(window_size);

        // allocate the memory for old clusters
        old_clusters[i].init(window_size);
    }

    // allocate the memory
    row_sum = new int[window_size];

    // allocate the count
    count = new int[window_size * window_size];

    // allocate the transition
    transition = new double[window_size * window_size];

    // allocate store transition
    store_transition = new double[window_size];
}

bool kmeans::perform_anomaly_detection() {
    // Clear previous results:
    result_threshold = -1;


    // for each cluster init row sum
    for (int i = 0; i < num_clusters; ++i) {
        row_sum[i] = 0;
    }

    // Count pairwise occurrences:
    int firstCluster, secondCluster;
    for (size_t i = 0; i < points->get_capacity() - 1; i++) {

        firstCluster = points->get_point(i).center;

        secondCluster = points->get_point(i + 1).center;

        count[firstCluster * num_clusters + secondCluster] += 1;
        row_sum[firstCluster] += 1;
    }


    // Create transition matrix:
    for (int i = 0; i < num_clusters; i++) {
        for (int j = 0; j < num_clusters; j++) {

            // init to zero
            transition[i * num_clusters + j] = 0;

            if (count[i * num_clusters + j] > 0) {
                transition[i * num_clusters + j] = ((double) count[i * num_clusters + j]) / row_sum[i];
            }
        }
    }

    // New anomaly detection code: 2.1
    double curThreshold = 1.0;
    for (size_t i = (window_size - smaller_window - 1) ; i < window_size - 1; i++) {
        firstCluster = points->get_point(i).center;
        secondCluster = points->get_point(i + 1).center;

        curThreshold *= transition[firstCluster * num_clusters + secondCluster];
    }

    if (curThreshold < threshold) {
        result_threshold = curThreshold;
        return true;	// Anomaly detected
    }

    return false;	// Anomaly not found.
}

void kmeans::perform_clustering() {
    bool finish = false;
    int iteration = 0;

    // Add in new data, one at a time, recalculating centroids with each new one.
    while (!finish) {

        // clear the cluster points (the centroids are left in place...)
        clear_clusters();

        // assign points to the closer cluster
        assign_cluster();

        // calculate new centroids.
        calculate_centroids();

        iteration++;

        // Calculates total distance between new and old Centroids
        double distance = 0;

        for (int i = 0; i < num_clusters; i++) {
            distance += point_distance(old_clusters->get_centroid(), clusters->get_centroid());
        }

        // Take into account clustering precision:
        if (distance < clustering_precision) {
            finish = true;
        }

        // DEBS: Do not run more than maximum iterations:
        if (iteration == maximum_iterations) {
            finish = true;
        }
    }
}

void kmeans::clear_clusters() {
    for (int i = 0; i < num_clusters; i++) {
        clusters[i].get_points_num() = 0;
    }
}

void kmeans::assign_cluster() {

    double min;

    int cluster_idx = 0;
    double distance;

    //new logic:
    double cluster_centroid = 0.0;

    // Check each point against each cluster:
    for (size_t j = 0; j < points->get_capacity(); j++) {

        // grab the point
        point &pt = points->get_point(j);

        // set the minimum distance to max double
        min = DBL_MAX;

        for (int i = 0; i < num_clusters; i++) {

            // grab the cluster
            cluster &c = clusters[i];

            // figure out the distance from the centroid
            distance = point_distance(pt, c.get_centroid());

            // if the distance is smaller
            if (distance < min) {
                min = distance;
                cluster_idx = i;
                cluster_centroid = c.get_centroid().x;
            }
            // if a point is equi-distant from two clusters put it in the higher cluster.
            else if (distance == min) {
                if (c.get_centroid().x > cluster_centroid) {
                    min = distance;
                    cluster_idx = i;
                    cluster_centroid = c.get_centroid().x;
                }
            }
        }

        // assign the center of the point
        pt.center = cluster_idx;

        // add the point
        clusters[cluster_idx].current_point() = pt;

        // notify that the point was added
        clusters[cluster_idx].point_added();
    }
}

void kmeans::calculate_centroids() {

    // store the pointer to the old clusters
    cluster* tmp = old_clusters;

    // old clusters become the current clusters
    old_clusters = clusters;

    // the current clusters have the memory of the old clusters.
    clusters = tmp;

    for(size_t i = 0; i < num_clusters; i++) {

        // grab the cluster
        cluster &c = old_clusters[i];

        // sum of the points
        double sum_x = 0;

        // sum the points of cluster
        for(size_t j = 0; j < c.get_points_num(); j++) {
            sum_x += c.get_point(j).x;
        }

        // grab the centroid
        point &centroid = c.get_centroid();

        // copy the id
        clusters[i].get_id() = (size_t)c.get_id();

        // if we have some points on this cluster
        if (c.get_points_num() > 0) {
            double new_x = sum_x / c.get_points_num();
            clusters[i].get_centroid().x = new_x;
        }
        else {
            clusters[i].get_centroid().x = c.get_centroid().x;
        }
    }
}

bool kmeans::perform_all_calculation(circular_queue *window) {
    // set the points:
    points = window;

    // The first "NUMCLUSTERS" unique points are the cluster centers:
    set<double> unique_points;
    size_t count_unique = 0;

    // current value:
    double curr_value;
    for (size_t i = 0; i < points->get_capacity(); i++) {
        curr_value = points->get_point(i).x;

        // check if we have found all the clusters
        if (count_unique >= num_clusters) {
            break;
        }

        // if we haven't check if this is a new cluster
        if (unique_points.find(curr_value) == unique_points.end()) {
            unique_points.insert(curr_value);

            // init the clusters
            clusters[count_unique].get_id() = count_unique;

            // set the value of the centroid
            clusters[count_unique].get_centroid().x = curr_value;

            // reset the number of points
            clusters[count_unique].get_points_num() = 0;

            //Increment unique points count:
            count_unique++;
        }
    }

    // if a given window has less than K distinct values than the number of clusters to be computed must be equal to the number of distinct values in the window.
    num_clusters = count_unique < num_clusters ? count_unique : num_clusters;

    // perform the clustering:
    perform_clustering();

    // perform anomaly detection:
    return perform_anomaly_detection();
}

kmeans::~kmeans() {

    // free the clusters
    delete[] clusters;

    // free the old clusters
    delete[] old_clusters;

    // free the memory
    delete[] row_sum;

    // free the count
    delete[] count;

    // free the transition
    delete[] transition;

    // free store transition
    delete[] store_transition;
}

double kmeans::get_result_threshold() const {
    return result_threshold;
}
