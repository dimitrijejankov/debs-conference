//
// Created by dimitrije on 14/04/17.
//

#ifndef HOBBITREWRITE_PARSER_H
#define HOBBITREWRITE_PARSER_H

#include <functional>

using namespace std;

class parser {

private:

    // callback for parsing (machineIndex, dimension, timestampIndex, value)
    function<void(int, int, int, double)> callback;

public:

    // constructor for the parser
    parser(function<void(int, int, int, double)> callback) : callback(callback) {};

    // parse

};


#endif //HOBBITREWRITE_PARSER_H
