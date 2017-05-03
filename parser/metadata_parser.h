//
// Created by dimitrije on 4/22/17.
//

#ifndef HOBBITREWRITE_METADATA_PARSER_H
#define HOBBITREWRITE_METADATA_PARSER_H

#include <unordered_map>

using namespace std;

#define METADATA_FILE "files/10molding_machine_5000dp.mdata"

class metadata_parser {
private:
    // holds the cluster information
    unordered_map<size_t , size_t*> number_of_clusters;

    // the threshold for the kmeans
    double threshold;

    // the windows size
    size_t window_size;

    // the transition amount
    size_t transitions_amount;

    // kmeans max iterations
    size_t max_clustering_iterations;

    // the precision of the clustering
    double clustering_precision;

    // parses the file
    void parse();

public:

    // constructor
    metadata_parser();

    // destructor
    ~metadata_parser();

    //  returns the numbers of clusters for machine
    inline size_t *get_cluster_no(size_t machine_no) {
        return number_of_clusters[machine_no];
    }

    inline double get_threshold() const {
        return threshold;
    }

    inline size_t get_window_size() const {
        return window_size;
    }

    inline size_t get_transitions_amount() const {
        return transitions_amount;
    }

    inline size_t get_max_clustering_iterations() const {
        return max_clustering_iterations;
    }

    inline double get_clustering_precision() const {
        return clustering_precision;
    }
};



#endif //HOBBITREWRITE_METADATA_PARSER_H
