//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include <cmath>
#include "rdf_parser.h"


// constants


rdf_parser::rdf_parser(function<void(int, int, int, double)> callback) : callback(callback) {

    // we use this to skip every line
    lineStartSkip = strlen(LINE_START);

    // we use these skips on the machine line junk
    machineSkip = strlen(MACHINE);
    machineSkip2 = strlen(MACHINE_2);

    // we use this to skip the observation line junk
    observationSkip1 = strlen(OBSERVATION_1);
    observationSkip2 = strlen(OBSERVATION_1) + 8;
    observationSkip3 = strlen(OBSERVATION_2);

    // we use this to skip the value line junk
    valueSkip1 = strlen(VALUE_1);
    valueSkip2 = strlen(VALUE_2);

    // the current machine index
    machine_idx = 0;

    // the current dimension
    dimension = 0;

    // the current value
    value = 0;

    // the current timestamp
    timestamp_idx = 0;
}


void rdf_parser::parse(char *data, size_t length) {

    // we begin from the start of the string
    size_t i = 0;

    // while we are not a end of the string
    while (i < length) {
        i += parse_line(data + i);
        i = find_character(data, '\n', i);
        i += 1;
    }
}

size_t rdf_parser::find_character(char *line, char c, size_t i) {

    // while we don't find the character
    while (line[i] != c) {
        i++;
    }

    return i;
}

size_t rdf_parser::parse_line(char *line) {
    // here we check the
    size_t i = lineStartSkip;

    bool is_observation = true;
    bool is_observation_group = true;
    bool is_value = true;
    bool is_timestamp = true;

    size_t j = 0;

    // figure out what we are dealing with....
    while (line[i] != '_') {

        if (is_observation_group) {
            is_observation_group = OBSERVATION_GROUP[j] == line[i];
        }

        if (is_observation) {
            is_observation = OBSERVATION[j] == line[i];
        }

        if (is_value) {
            is_value = VALUE[j] == line[i];
        }

        if (is_timestamp) {
            is_timestamp = TIMESTAMP[j] == line[i];
        }

        // we don't care about the others.
        if (!is_observation && !is_value && !is_observation_group && !is_timestamp) {
            return i;
        }

        j++;
        i++;
    }

    // skip the '_'
    i++;
    j = find_character(line, '>', i);

    if(is_timestamp) {
        timestamp_idx = fast_atoi(line + i, j - i);
    }

    // skip "> "
    i = j + 2;

    // we ware dealing wit an observation
    if (is_observation) {
        // here we only care about the dimension...
        if (line[i + observationSkip1] == 'o' && line[i + observationSkip2] == 'P') {
            i = find_character(line, '_', i + observationSkip3) + 1;
            j = find_character(line, '>', i);

            dimension = fast_atoi(line + i, j - i);
        }
    }

        // if we are dealing with an observation group
    else if (is_observation_group) {
        // in observation we only are interested in the machine...
        if (line[i + machineSkip] != 'm') {
            return i + machineSkip;
        }

        j = find_character(line, '>', i + machineSkip2);
        i += machineSkip2;

        machine_idx = fast_atoi(line + i, j - i);
    }

        // it's a value
    else if (is_value) {
        if (line[i + valueSkip1] != 'v') {
            return i + valueSkip1;
        }

        j = find_character(line, '"', i + valueSkip2);
        i += valueSkip2;

        if (line[i + 1] != 'A') {
            value = fast_atof(line + i, j - i);

            // call the callback
            //printf("Machine index %d, dimension %d, timestamp %d, value %f\n", machine_idx, dimension, timestamp_idx, value);
            callback(machine_idx, dimension, timestamp_idx, value);
        }
    }

    return i;
}

int rdf_parser::fast_atoi(const char *str, size_t len) {
    int val = 0;
    while(len--) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

double rdf_parser::fast_atof(char *str, size_t len) {
    char c = str[len];
    str[len] = '\0';

    double r = 0.0;
    bool neg = false;
    if (*str == '-') {
        neg = true;
        ++str;
    }
    while (*str >= '0' && *str <= '9') {
        r = (r*10.0) + (*str - '0');
        ++str;
    }
    if (*str == '.') {
        double f = 0.0;
        int n = 0;
        ++str;
        while (*str >= '0' && *str <= '9') {
            f = (f*10.0) + (*str - '0');
            ++str;
            ++n;
        }
        r += f / std::pow(10.0, n);
    }
    if (neg) {
        r = -r;
    }

    str[len] = c;
    return r;
}