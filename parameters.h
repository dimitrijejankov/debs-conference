#ifndef HOBBITREWRITE_PARAMETERS_H
#define HOBBITREWRITE_PARAMETERS_H

#define ENVIRONMENT LOCAL
#define LOCAL  1
#define HOBBIT 2

#include <iostream>

// get the system URI
const char * SYSTEM_URI() {
#if ENVIRONMENT == LOCAL
    return "http://project-hobbit.eu/resources/debs2017/debsparrotsystemexample";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_SYSTEM_URI");
#endif
}

// get the rabbit mq host name
const char *RABBIT_MQ_HOST_NAME() {
#if ENVIRONMENT == LOCAL
    return "localhost";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_SYSTEM_URI");
#endif
}

// get the hobbit session id
const char *HOBBIT_SESSION_ID() {
#if ENVIRONMENT == LOCAL
    return "exp1";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_SESSION_ID");
#endif
}

// get the system parameters model
const char *SYSTEM_PARAMETERS_MODEL() {
#if ENVIRONMENT == LOCAL
    return "{}";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("SYSTEM_PARAMETERS_MODEL");
#endif
}

// get the experiment uri
const char *HOBBIT_EXPERIMENT_URI() {
#if ENVIRONMENT == LOCAL
    return "http://example.com/exp1";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_EXPERIMENT_URI");
#endif
}

#endif //HOBBITREWRITE_PARAMETERS_H
