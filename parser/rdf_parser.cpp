//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "rdf_parser.h"


rdf_parser::rdf_parser(metadata_parser *mp, function<void(size_t, size_t, size_t, double)> callback) : callback(
        callback), mp(mp), last_timestamp_idx(0), last_timestamp_hash(-1), current_hash(-1) {

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

    // we use this to skip to the value of the date
    dateValue1 = strlen(DATE_VALUE_1);
    dateValue2 = strlen(DATE_VALUE_2);

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
        i = find_character(data, length, '\n', i);
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


size_t rdf_parser::find_character(char *line, size_t n, char c, size_t i) {
    // while we don't find the character
    while (line[i] != c && i != n) {
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

    // this is to grab the timestamp index
    if (is_timestamp) {
        timestamp_idx = fast_atoi(line + i, j - i);
    }

    // skip "> "
    i = j + 2;

    // this is to check the timestamp value
    if(is_timestamp){

        if (line[i + valueSkip1] != 'v') {
            return i + valueSkip1;
        }

        i += valueSkip2 + dateValue1;

        // grab the day
        int day = (line[i] - '0') * 10 + (line[i+1] - '0');

        i += 2 + dateValue2;

        // grab the hours
        int hours = (line[i] - '0') * 10 + (line[i+1] - '0');

        i += 3;

        // grab the minutes
        int minutes = (line[i] - '0') * 10 + (line[i+1] - '0');

        // figure out the hash
        current_hash = (24 * 60) * day + 60 * hours + minutes;
    }

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
            if (mp->get_cluster_no((size_t) machine_idx)[dimension] != 0) {
                callback((size_t) machine_idx, (size_t) dimension, check_timestamp(current_hash, timestamp_idx), value);
            }
        }
    }

    return i;
}

int rdf_parser::fast_atoi(const char *str, size_t len) {
    int val = 0;
    while (len--) {
        val = val * 10 + (*str++ - '0');
    }
    return val;
}

double rdf_parser::atof_ninja(char *str, size_t len) {
    char c = str[len];
    str[len] = '\0';

    std::stringstream ss;
    double r;

    ss << str;
    std::setprecision(100);
    ss >> r;

    str[len] = c;
    return r;
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
        r = (r * 10.0) + (*str - '0');
        ++str;
    }
    if (*str == '.') {
        double f = 0.0;
        int n = 0;
        ++str;
        while (*str >= '0' && *str <= '9') {
            f = (f * 10.0) + (*str - '0');
            ++str;
            ++n;
        }
        r += f / std::pow(10.0, n);
    }
    if (neg) {
        r = -r;
    }

    str[len] = c;

    r = round(r * 100.0) / 100;

    return r;
}

double rdf_parser::fast_atof_2(char *str, size_t len) {
    char c = str[len];
    str[len] = '\0';

    int32_t y = 0;
    char x = 0;

    bool neg = false;
    if (*str == '-') {
        neg = true;
        ++str;
    }
    while (*str >= '0' && *str <= '9') {
        y = (y * 10) + (*str - '0');
        ++str;
    }
    if (*str == '.') {

        x = (*(str + 1) - '0');

        if (*(str + 2) >= '0' && *(str + 2) <= '9') {
            x += 10 * (*(str + 2) - '0');
        }
    }
    if (neg) {
        y = -y;
    }

    str[len] = c;

    return y + (x / 100.0);
}

size_t rdf_parser::check_timestamp(int hash, int timestamp_idx) {

    // if this is the first time
    if (last_timestamp_hash == -1) {

        // assign the values we got
        last_timestamp_idx = (size_t)timestamp_idx;
        last_timestamp_hash = hash;

        // return timestamp index
        return last_timestamp_idx;
    }

    // if the the last value doesn't match it's a new timestamp...
    if (last_timestamp_hash != hash) {
        last_timestamp_hash = hash;
        last_timestamp_idx++;
    }

    return last_timestamp_idx;
}
