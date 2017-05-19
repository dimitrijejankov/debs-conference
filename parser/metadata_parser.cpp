//
// Created by dimitrije on 4/22/17.
//

#include <cstring>
#include <fstream>
#include <string>
#include <regex>
#include <iostream>
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
        size_t *dimensions = new size_t[121];

        // set the values to zero
        memset(dimensions, 0, sizeof(size_t) * 121);

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

    // parse the system ttl
    parse_sys_ttl();
}

metadata_parser::~metadata_parser() {
    // free the memory
    for(auto it = number_of_clusters.begin(); it != number_of_clusters.end(); it++) {
        delete[] it->second;
    }
}

void metadata_parser::parse_sys_ttl() {

    std::ifstream t("system.ttl");
    std::string s((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    std::smatch match;

    // regex for the probability
    std::regex re("<http://www.debs2017.org/gc/probabilityThreshold>\\s+\"(.+)\"");

    // parse the threshold if present
    if (std::regex_search(s, match, re)) {
        threshold = std::stod(match[1]);
    }

    // regex for the window size
    re = std::regex("<http://www.debs2017.org/gc/windowSize>\\s+\"(.+)\"");

    // parse the window size if present
    if (std::regex_search(s, match, re)) {
        window_size = std::stoul(match[1]);
    }

    // regex for the small window
    re = std::regex("<http://www.debs2017.org/gc/transitionsCount>\\s+\"(.+)\"");

    // parse the window size if present
    if (std::regex_search(s, match, re)) {
        transitions_amount = std::stoul(match[1]);
    }

    // regex for the small window
    re = std::regex("<http://www.debs2017.org/gc/maxClusterIterations>\\s+\"(.+)\"");

    // parse the window size if present
    if (std::regex_search(s, match, re)) {
        max_clustering_iterations = std::stoul(match[1]);
    }
}
