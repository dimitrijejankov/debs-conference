//
// Created by dimitrije on 14/04/17.
//

#ifndef HOBBITREWRITE_PARSER_H
#define HOBBITREWRITE_PARSER_H

#include <functional>

using namespace std;

// string constants for skips
#define LINE_START "<http://project-hobbit.eu/resources/debs2017#"
#define MACHINE "<http://www.agtinternational.com/ontologies/I4.0#"
#define MACHINE_2 "<http://www.agtinternational.com/ontologies/I4.0#machine> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#Machine_"
#define OBSERVATION_1 "<http://purl.oclc.org/NET/ssnx/ssn#"
#define OBSERVATION_2 "<http://purl.oclc.org/NET/ssnx/ssn#observedProperty> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#_"
#define VALUE_1 "<http://www.agtinternational.com/ontologies/IoTCore#"
#define VALUE_2 "<http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> \""

// string constants we use to figure out what line is it...
const char OBSERVATION[] = { 'O', 'b', 's', 'e', 'r', 'v', 'a', 't', 'i', 'o', 'n', '\0' };
const char OBSERVATION_GROUP[] = { 'O', 'b', 's', 'e', 'r', 'v', 'a', 't', 'i', 'o', 'n', 'G', 'r', 'o', 'u', 'p', '\0' };
const char TIMESTAMP[] = { 'T', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', '\0' };
const char VALUE[] = { 'V', 'a', 'l', 'u', 'e', '\0' };

class rdf_parser {

private:

    // the current machine index
    int machine_idx;

    // the current dimension
    int dimension;

    // the current value
    double value;

    // the current timestamp
    int timestamp_idx;

    // skip line parameters
    size_t lineStartSkip;
    size_t machineSkip;
    size_t machineSkip2;
    size_t observationSkip1;
    size_t observationSkip2;
    size_t observationSkip3;
    size_t valueSkip1;
    size_t valueSkip2;

    // callback for parsing (machine_idx, dimension, timestamp_idx, value)
    function<void(int, int, int, double)> callback;

public:

    // constructor for the rdf_parser
    rdf_parser(function<void(int, int, int, double)> callback);

    // parse
    void parse(char* data, size_t length);

    // parse the line
    size_t parse_line(char* line);

    // find the character
    size_t find_character(char *line, char c, size_t i);

    // integer conversion
    inline int fast_atoi(const char *str, size_t len);

    inline double fast_atof(char *str, size_t len);
};


#endif //HOBBITREWRITE_PARSER_H
