//
// Created by dimitrije on 15/04/17.
//

#include <cstring>
#include "input_component.h"
#include "parameters.h"
#include "utils.h"

input_component::input_component() {
    // set the input socket to NULL
    in_channel.socket = NULL;

    // init the input channel
    init_channel(in_channel);

    // init the input queue
    init_queue(in_channel, in_queue, input_queue_name());

    // init the parser
    parser = new rdf_parser([](int machine_idx, int dimension, int timestamp_idx, double value) {
        printf("Machine index %d, dimension %d, timestamp %d, value %f\n", machine_idx, dimension, timestamp_idx, value);
    });
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
    // grab the input queue name
    string in_name = input_queue_name();

    // start the consuming
    amqp_basic_consume(in_channel.conn, 1, amqp_cstring_bytes(in_name.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(in_channel.conn), "Consuming");

    // start fetching from the input queue
    for (;;) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        // release buffers if needed
        amqp_maybe_release_buffers(in_channel.conn);

        // wait for a message and consume it
        res = amqp_consume_message(in_channel.conn, &envelope, NULL, 0);

        // if the message is not good stop fetching
        if (AMQP_RESPONSE_NORMAL != res.reply_type) {
            break;
        }

        // if we got a termination message we notify the main thread about it...
        if(strcmp(TERMINATION_MESSAGE, (char*)envelope.message.body.bytes) == 0) {
            std::unique_lock<std::mutex> lk(m);
            finished = true;
            lk.unlock();
            cv.notify_one();
        }
        // otherwise we parse the input
        else {
            parser->parse((char*)envelope.message.body.bytes, envelope.message.body.len);
        }

        // destroy the envelope that was holding the message
        amqp_destroy_envelope(&envelope);
    }
}

bool input_component::is_finished() {
    return finished;
}

