//
// Created by dimitrije on 16/04/17.
//

#include "kmeans.h"
#include <set>

using namespace std;


kmeans::kmeans(double result_threshold) : result_threshold(-1) {

    // allocate the memory for the clusters
    clusters = new cluster[NUMCLUSTERS](NUMCLUSTERS);
}


bool kmeans::perform_anomaly_detection() {
    return false;
}

void kmeans::perform_clustering() {

}

void kmeans::clear_clusters() {

}

void kmeans::assign_cluster() {

}

void kmeans::calculate_centroids() {

}

void kmeans::get_centroids() {

}

bool kmeans::perform_all_calculation(circular_queue *window) {
    // Points:
    points = window;
    
    // The first "NUMCLUSTERS" unique points are the cluster centers:
    set<double> unique_points;
    int countUnique = 0;

    // Cuurent value:
    double curValue;
    for (int i = 0; i < _points.size(); i++) {
        if (countUnique < NUMCLUSTERS) {
            curValue = _points.get(i).getX();
            if(!unique_points.contains(curValue)) {
                unique_points.add(curValue);

                //Create clusters here:
                cluster cluster = new Cluster(countUnique);  // Cluster ID
                point centroid = new Point(curValue);
                cluster.setCentroid(centroid);
                clusters.add(cluster);

                //Increment unique points count:
                countUnique++;
            }
        }
    }

    // If a given window has less than K distinct values than the number of clusters to be computed
    // must be equal to the number of distinct values in the window.
    if (countUnique < NUMCLUSTERS) {
        NUMCLUSTERS = countUnique;
    }

    // Perform the clustering:
    perform_clustering();


    // Perform anomaly detection:
    return perform_anomaly_detection();
}
