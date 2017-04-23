//
// Created by dimitrije on 4/22/17.
//

#include "metadata_parser.h"

void metadata_parser::parse() {

    // open the file
    FILE *f = fopen(METADATA_FILE, "rb");

    // the number of machines
    int32_t number_of_machines;

    // read the number of machines
    fread(&number_of_machines, sizeof(int32_t), 1, f);

    // the index of the current machine
    int32_t machine_idx;

    for(int i = 0; i < number_of_machines; i++) {

        // read the machine index
        fread(&machine_idx, sizeof(int32_t), 1, f);

        // allocate the dimensions
        size_t *dimensions = new size_t[120];

        // the number of dimensions
        int32_t dimension_number;

        // the number of clusters
        int32_t cluster_num;

        // read the dimensions
        for(int j = 0; j < 55; j++) {

            // read the dimension number
            fread(&dimension_number, sizeof(int32_t), 1, f);

            // read the number of clusters
            fread(&cluster_num, sizeof(int32_t), 1, f);

            // write it to dimensions
            dimensions[dimension_number] = (size_t) cluster_num;
        }

        // grab the dimensions
        number_of_clusters[machine_idx] = dimensions;
    }

    // close the file
    fclose(f);
}

metadata_parser::metadata_parser() : threshold(0.005), window_size(10), transitions_amount(5),
                                     max_clustering_iterations(50), clustering_precision(0.00001)   {
    // run the parsing
    parse();
}

metadata_parser::~metadata_parser() {
    // free the memory
    for(auto it = number_of_clusters.begin(); it != number_of_clusters.end(); it++) {
        delete[] it->second;
    }
}



