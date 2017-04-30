#ifndef HOBBITREWRITE_PARAMETERS_H
#define HOBBITREWRITE_PARAMETERS_H

#include <string>

#define ENVIRONMENT HOBBIT
#define LOCAL  1
#define HOBBIT 2

#include <iostream>

// get the system URI
std::string SYSTEM_URI();

// get the rabbit mq host name
std::string RABBIT_MQ_HOST_NAME();

// the default port for the connection
int RABBIT_MQ_PORT();

// get the hobbit session id
std::string HOBBIT_SESSION_ID();

// get the system parameters model
std::string SYSTEM_PARAMETERS_MODEL();

// get the experiment uri
std::string HOBBIT_EXPERIMENT_URI();

// get the input amqp_queue name
std::string DATA_GEN_2_SYSTEM_QUEUE_NAME();

// get the output amqp_queue name
std::string SYSTEM_2_EVAL_STORAGE_QUEUE_NAME();

// get hobbit command exchange name
std::string HOBBIT_COMMAND_EXCHANGE_NAME();

// the termination message
const char TERMINATION_MESSAGE[] = "~~Termination Message~~";

// size of the termination message
const int TERMINATION_MESSAGE_SIZE = 23;

#endif //HOBBITREWRITE_PARAMETERS_H
