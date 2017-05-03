//
// Created by dimitrije on 5/1/17.
//

#include <cstring>
#include <cmath>
#include "rdf_parallel_parser.h"

size_t rdf_parallel_parser::check_timestamp(int hash, size_t timestamp_idx) {

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

rdf_parallel_parser::rdf_parallel_parser(metadata_parser *mp, function<void(size_t, size_t, size_t, double)> callback) : callback(
callback), mp(mp), last_timestamp_idx(0), last_timestamp_hash(-1) {

    // the current machine index
    machine_idx = 0;

    // the current timestamp
    timestamp_idx = 0;

    // header skips
    HEADER_SKIP_1 = strlen(HEADER_SKIP_1_STR);
    MACHINE_SKIP = strlen(MACHINE_SKIP_STR);
    HEADER_SKIP_2 = strlen(HEADER_SKIP_2_STR);
    TIMESTAMP_SKIP_1 = strlen(TIMESTAMP_SKIP_1_STR);
    TIMESTAMP_SKIP_2 = strlen(TIMESTAMP_SKIP_2_STR);
    HEADER_SKIP_3 = strlen(HEADER_SKIP_3_STR);
    DATE_VALUE_SKIP_1 = strlen(DATE_VALUE_SKIP_1_STR);
    DATE_VALUE_SKIP_2 = strlen(DATE_VALUE_SKIP_2_STR);

    // sizes
    DATA_POINT_SIZE = strlen(DATA_POINT_CONTENT);

    // data point skips
    DIMENSION_SKIP = strlen(DIMENSION_SKIP_STR);
    VALUE_SKIP = strlen(VALUE_SKIP_STR);
    DATA_POINT_SKIP = strlen(DATA_POINT_SKIP_STR);
}

void rdf_parallel_parser::parse(char *data, size_t length) {

    // parse the fucking header
    size_t i = parser_header(data);

    // parse the fucking data points
    while(length - i >= DATA_POINT_SIZE) {
        i = parse_data_point(data, i);
    }

}

size_t rdf_parallel_parser::find_character(char *line, char c, size_t i) {

    // while we don't find the character
    while (line[i] != c) {
        i++;
    }

    return i;
}

size_t rdf_parallel_parser::parser_header(char *data) {

    size_t i = HEADER_SKIP_1;
    size_t j;

    // found the beginning of the machine
    i = find_character(data, '\n', i);

    // skip to the potential start of the machine value
    i += MACHINE_SKIP;

    // go to the actual beginning of the machine value
    i = find_character(data, '_', i);

    // go to the first character
    i++;

    // grab the machine number
    j = find_character(data, '>', i);

    // grab the machine index
    machine_idx = fast_atoi(data + i, j - i);

    // this is to skip to the new line
    i = j + HEADER_SKIP_2;

    // found the beginning of the timestamp line
    i = find_character(data, '\n', i);

    // skip to the timestamp index
    i += TIMESTAMP_SKIP_1;

    // skip to the fist character..
    i++;

    // grab the index of the timestamp
    j = find_character(data, '>', i);

    // grab the timestamp index
    timestamp_idx = (size_t)fast_atoi(data + i, j - i);

    // skip to the date value
    i = j + TIMESTAMP_SKIP_2 + DATE_VALUE_SKIP_1;

    // skip the the first character of the day
    i++;

    // grab the day
    int day = (data[i] - '0') * 10 + (data[i+1] - '0');

    // skip to the hours
    i += 2 + DATE_VALUE_SKIP_2;

    // grab the hours
    int hours = (data[i] - '0') * 10 + (data[i+1] - '0');

    // skip ':' to to to minutes
    i += 3;

    // grab the minutes
    int minutes = (data[i] - '0') * 10 + (data[i+1] - '0');

    // figure out the hash
    int current_hash = (24 * 60) * day + 60 * hours + minutes;

    // figure out the timestamp.
    timestamp_idx = check_timestamp(current_hash, timestamp_idx);

    // skip the rest of the header
    i += HEADER_SKIP_3;

    // found the beginning of the data
    i = find_character(data, '\n', i);

    y = 0;

    return i;
}

int rdf_parallel_parser::fast_atoi(const char *str, size_t len) {
    int val = 0;
    while (len--) {
        val = val * 10 + (*str++ - '0');
    }
    return val;
}

double rdf_parallel_parser::fast_atof(char *str, size_t len) {
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

size_t rdf_parallel_parser::parse_data_point(char *data, size_t i) {

    size_t j;

    // skip to the approximate position of machine dimension value
    i += DIMENSION_SKIP;

    // skip to the real position
    i = find_character(data, '_', i);

    // skip the '_'
    i++;

    // skip the machine
    i = find_character(data, '_', i) + 1;

    // figure out where the '>'
    j = find_character(data, '>', i);

    // grab the dimension
    int dimension_number = fast_atoi(data + i, j - i);

    // skip to the approximate position of the value
    i = j + VALUE_SKIP;

    // skip to the real position of the value
    i = find_character(data, '"', i);

    // skip the '"'
    i++;

    // find the end
    j = find_character(data, '"', i);

    // parse the double
    if (data[i + 1] != 'A') {
        double value = fast_atof(data + i, j - i);

        // call the callback
        if (mp->get_cluster_no((size_t) machine_idx)[dimension_number] != 0) {
            callback((size_t) machine_idx, (size_t) dimension_number, timestamp_idx, value);
        }

        y++;
    }

    i = j + DATA_POINT_SKIP;

    return i;
}
