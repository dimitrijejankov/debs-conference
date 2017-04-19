#include "parameters.h"

// get the rice_system URI
std::string SYSTEM_URI() {
#if ENVIRONMENT == LOCAL
    return "http://project-hobbit.eu/resources/debs2017/debsparrotsystemexample";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_SYSTEM_URI");
#endif
}

// get the rabbit mq host name
std::string RABBIT_MQ_HOST_NAME() {
#if ENVIRONMENT == LOCAL
    return "localhost";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_RABBIT_HOST");
#endif
}

int RABBIT_MQ_PORT() {
    return 5672;
}

// get the hobbit session id
std::string HOBBIT_SESSION_ID() {
#if ENVIRONMENT == LOCAL
    return "exp1";
#elif ENVIRONMENT == HOBBIT
    if (std::getenv("HOBBIT_SESSION_ID") != NULL) {
        return std::getenv("HOBBIT_SESSION_ID");
    }

    return "SYSTEM";
#endif
}

// get the system parameters model
std::string SYSTEM_PARAMETERS_MODEL() {
#if ENVIRONMENT == LOCAL
    return "{}";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("SYSTEM_PARAMETERS_MODEL");
#endif
}

// get the experiment uri
std::string HOBBIT_EXPERIMENT_URI() {
#if ENVIRONMENT == LOCAL
    return "http://example.com/exp1";
#elif ENVIRONMENT == HOBBIT
    return std::getenv("HOBBIT_EXPERIMENT_URI");
#endif
}

// get the input queue name
std::string DATA_GEN_2_SYSTEM_QUEUE_NAME() {
    return "hobbit.datagen-system";
}

// get the output queue name
std::string SYSTEM_2_EVAL_STORAGE_QUEUE_NAME() {
    return "hobbit.system-evalstore";
}

// get hobbit command exchange name
std::string HOBBIT_COMMAND_EXCHANGE_NAME() {
    return "hobbit.command";
}