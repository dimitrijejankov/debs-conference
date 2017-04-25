#include "data/java_double_parser.h"
/* This is the program's "main" routine. */


int main (int argc, char *argv[]) {

    java_double_parser jdp;

    for(int i = 0; i < 10000; i++){
        printf(jdp.toString(i + 0.22));
    }

    return 0;
}