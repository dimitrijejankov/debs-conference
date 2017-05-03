//
// Created by dimitrije on 5/1/17.
//

#ifndef HOBBITREWRITE_RDF_PARALLEL_PARSER_H
#define HOBBITREWRITE_RDF_PARALLEL_PARSER_H


#include <cstddef>
#include <functional>
#include "metadata_parser.h"
#include "rdf_constants.h"

#define NUMBER_OF_SPLITS 2

class rdf_parallel_parser {
private:
    // the current machine index
    int machine_idx;

    // the current timestamp
    size_t timestamp_idx;

    // last index
    size_t last_timestamp_idx;

    // last timestamp hash
    int last_timestamp_hash;

    // average cycle
    long x;

    long y;

    // callback for parsing (machine_idx, dimension, timestamp_idx, value)
    function<void(size_t, size_t, size_t, double)> callback;

    // check timestamp
    size_t check_timestamp(int hash, size_t timestamp_idx);

    // parses the integer
    int fast_atoi(const char *str, size_t len);

    // parses the float
    double fast_atof(char *str, size_t len);

    // metadata to check for stateful dimensions
    metadata_parser *mp;

    // skips for header
    size_t HEADER_SKIP_1;
    size_t MACHINE_SKIP;
    size_t HEADER_SKIP_2;
    size_t TIMESTAMP_SKIP_1;
    size_t TIMESTAMP_SKIP_2;
    size_t HEADER_SKIP_3;
    size_t DATE_VALUE_SKIP_1;
    size_t DATE_VALUE_SKIP_2;

    // sizes
    size_t DATA_POINT_SIZE;

    // skips for data point
    size_t DIMENSION_SKIP;
    size_t VALUE_SKIP;
    size_t DATA_POINT_SKIP;

public:

    // constructor for the rdf_parser
    rdf_parallel_parser(metadata_parser *mp, function<void(size_t, size_t, size_t, double)> callback);

    // parse
    void parse(char* data, size_t length);

    // parse the header
    size_t parser_header(char* data);

    // parse value
    size_t parse_data_point(char *data, size_t i);

    // find the character
    size_t find_character(char *line, char c, size_t i);

};


#endif //HOBBITREWRITE_RDF_PARALLEL_PARSER_H
