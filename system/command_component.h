//
// Created by dimitrije on 15/04/17.
//

#ifndef HOBBITREWRITE_COMMAND_RECEIVER_H
#define HOBBITREWRITE_COMMAND_RECEIVER_H

#include <set>
#include "component.h"

// command list
const char SYSTEM_READY_SIGNAL = 1;
const char BENCHMARK_READY_SIGNAL = 2;
const char DATA_GENERATOR_READY_SIGNAL = 3;
const char TASK_GENERATOR_READY_SIGNAL = 4;
const char EVAL_STORAGE_READY_SIGNAL = 5;
const char EVAL_MODULE_READY_SIGNAL = 6;
const char DATA_GENERATOR_START_SIGNAL = 7;
const char TASK_GENERATOR_START_SIGNAL = 8;
const char EVAL_MODULE_FINISHED_SIGNAL = 9;
const char EVAL_STORAGE_TERMINATE = 10;
const char BENCHMARK_FINISHED_SIGNAL = 11;
const char DOCKER_CONTAINER_START = 12;
const char DOCKER_CONTAINER_STOP = 13;
const char DATA_GENERATION_FINISHED = 14;
const char TASK_GENERATION_FINISHED = 15;
const char DOCKER_CONTAINER_TERMINATED = 16;
const char START_BENCHMARK_SIGNAL = 17;

class command_component : public component {
protected:
    // the response channel
    channel response_channel;

    // the response queue
    queue response_queue;

    // session id
    string session_id;

    // queue name
    string response_queue_name;

    // exchange name
    string exchange_name;

    // accepted command headers ids
    set<string> accepted_command_headers;

    // is system ready
    bool task_generation_finished;

public:

    // constructor
    command_component();

    // destructor
    ~command_component();

    // receive command
    void receive_command(char command, char* remainingData, size_t length);

    // handle command
    void handle_command(char* bytes, size_t length, amqp_basic_properties_t props);

    // send bytes to the command queue
    void send_to_cmd_queue(char command);

    // send command and bytes to the command queue
    void send_to_cmd_queue(char command, char *data, size_t length);

    // send the command and bytes to the command queue with properties
    void send_to_cmd_queue(char command, char *data, size_t length, amqp_basic_properties_t_ const *props);

    // swap endian
    inline void swap_endian(int32_t &value);

    void run();
};


#endif //HOBBITREWRITE_COMMAND_RECIEVER_H
