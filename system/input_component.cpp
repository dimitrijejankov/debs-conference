//
// Created by dimitrije on 15/04/17.
//

#include <cstring>
#include <rdf_parallel_parser.h>
//#include "rdf_parser.h"
#include "input_component.h"
#include "parameters.h"
#include "utils.h"

input_component::input_component(metadata_parser *mp, vector<worker_component*> workers) : wm(mp->get_window_size(), mp->get_transitions_amount(), workers) {
    // set the input socket to NULL
    in_channel.socket = NULL;

    // init the input channel
    init_channel(in_channel);

    // init the input amqp_queue
    init_queue(in_channel, in_queue, input_queue_name());

    // init the parser
    parser = new rdf_parallel_parser(mp, [this](size_t machine_idx, size_t dimension, size_t timestamp_idx, double value) {
        this->wm.push_data(machine_idx, dimension, timestamp_idx, value);
    }, [this]() {this->wm.flush();});

    // set the finished
    finished = false;
}

string input_component::input_queue_name() {
    return DATA_GEN_2_SYSTEM_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

input_component::~input_component() {
    // close the channel
    die_on_amqp_error(amqp_channel_close(in_channel.conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");

    // close the connection
    die_on_amqp_error(amqp_connection_close(in_channel.conn, AMQP_REPLY_SUCCESS), "Closing connection");

    // destroy the connection
    die_on_error(amqp_destroy_connection(in_channel.conn), "Ending connection");

    // free the parser
    delete parser;
}

void input_component::run(condition_variable &cv, mutex &m) {
    // grab the input amqp_queue name
    string in_name = input_queue_name();

    // start the consuming
    amqp_basic_consume(in_channel.conn, 1, amqp_cstring_bytes(in_name.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(in_channel.conn), "Consuming");

    // start fetching from the input amqp_queue
    while (!finished) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        // release buffers if needed
        amqp_maybe_release_buffers(in_channel.conn);

        // wait for a message and consume it
        res = amqp_consume_message(in_channel.conn, &envelope, NULL, 0);

        // if the message is not good stop fetching
        if (AMQP_RESPONSE_NORMAL != res.reply_type) {
            // free the bytes in the envelope
            break;
        }

        // if we got a termination message we notify the main thread about it...
        if(strncmp(TERMINATION_MESSAGE, (char*)envelope.message.body.bytes, TERMINATION_MESSAGE_SIZE) == 0) {
            m.lock();

            // set the finished flag.
            finished = true;

            m.unlock();
            cv.notify_one();
        }
        // otherwise we parse the input
        else {
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            parser->parse((char*)envelope.message.body.bytes, envelope.message.body.len);
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        }

        // destroy the envelope that was holding the message
        amqp_destroy_envelope(&envelope);
    }
}

bool input_component::is_finished() {
    return finished;
}

