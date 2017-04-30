//
// Created by dimitrije on 14/04/17.
//

#ifndef HOBBITREWRITE_PARSER_H
#define HOBBITREWRITE_PARSER_H

#include <functional>
#include "metadata_parser.h"

using namespace std;

// string constants for skips
#define LINE_START "<http://project-hobbit.eu/resources/debs2017#"
#define MACHINE "<http://www.agtinternational.com/ontologies/I4.0#"
#define MACHINE_2 "<http://www.agtinternational.com/ontologies/I4.0#machine> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#Machine_"
#define OBSERVATION_1 "<http://purl.oclc.org/NET/ssnx/ssn#"
#define OBSERVATION_2 "<http://purl.oclc.org/NET/ssnx/ssn#observedProperty> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#_"
#define VALUE_1 "<http://www.agtinternational.com/ontologies/IoTCore#"
#define VALUE_2 "<http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> \""
#define DATE_VALUE_1 "2017-01-"
#define DATE_VALUE_2 "T01:"

// string constants we use to figure out what line is it...
const char OBSERVATION[] = { 'O', 'b', 's', 'e', 'r', 'v', 'a', 't', 'i', 'o', 'n', '\0' };
const char OBSERVATION_GROUP[] = { 'O', 'b', 's', 'e', 'r', 'v', 'a', 't', 'i', 'o', 'n', 'G', 'r', 'o', 'u', 'p', '\0' };
const char TIMESTAMP[] = { 'T', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', '\0' };
const char VALUE[] = { 'V', 'a', 'l', 'u', 'e', '\0' };

// timestamp history

class rdf_parser {

private:

    static const size_t HISTORY_SIZE = 31 * 24 * 60;

    // the current machine index
    int machine_idx;

    // the current dimension
    int dimension;

    // the current value
    double value;

    // the current timestamp
    int timestamp_idx;

    // history for all timestamps
    int32_t timestamp_history[HISTORY_SIZE];

    // skip line parameters
    size_t lineStartSkip;
    size_t machineSkip;
    size_t machineSkip2;
    size_t observationSkip1;
    size_t observationSkip2;
    size_t observationSkip3;
    size_t valueSkip1;
    size_t valueSkip2;
    size_t dateValue1;
    size_t dateValue2;

    // callback for parsing (machine_idx, dimension, timestamp_idx, value)
    function<void(size_t, size_t, size_t, double)> callback;

    // metadata to check for stateful dimensions
    metadata_parser *mp;

public:

    // constructor for the rdf_parser
    rdf_parser(metadata_parser *mp, function<void(size_t, size_t, size_t, double)> callback);

    // parse
    void parse(char* data, size_t length);

    // parse the line
    size_t parse_line(char* line);

    // find the character
    size_t find_character(char *line, char c, size_t i);

    // find the character constrained
    size_t find_character(char *line, size_t n, char c, size_t i);

    // integer conversion
    inline int fast_atoi(const char *str, size_t len);

    inline double fast_atof(char *str, size_t len);

    inline double fast_atof_2(char *str, size_t len);

    double atof_ninja(char *str, size_t len);
};


#endif //HOBBITREWRITE_PARSER_H
