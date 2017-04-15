//
// Created by dimitrije on 14/04/17.
//

#include <cstring>
#include "rice_system.h"
#include "parameters.h"

void rice_system::init_channel(channel &c) {
    // create a connection object
    in_channel.conn  = amqp_new_connection();

    // create a socket
    in_channel.socket = amqp_tcp_socket_new(in_channel.conn);
    if (!socket) {
        die("creating TCP socket");
    }

    // open the socket
    in_channel.status = amqp_socket_open(in_channel.socket, hostname.c_str(), port);
    if (in_channel.status) {
        die("opening TCP socket");
    }

    // login to the rabbitMQ if fail exit
    die_on_amqp_error(amqp_login(in_channel.conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"), "Logging in");

    // open a channel to the rabbitMQ
    amqp_channel_open(in_channel.conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(in_channel.conn), "Opening channel");
}


void rice_system::init_queue(channel &c, queue &q, string queue_name) {
    // declare queue
    amqp_queue_declare_ok_t *r = amqp_queue_declare(c.conn, 1, amqp_cstring_bytes(queue_name.c_str()), 0, 0, 0, 1, amqp_empty_table);

    // TODO Set the prefetch count to 1

    // if we failed to create a queue exit
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Declaring queue");
}

string rice_system::input_queue_name() {
    return DATA_GEN_2_SYSTEM_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

string rice_system::output_queue_name() {
    return SYSTEM_2_EVAL_STORAGE_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

rice_system::rice_system() {

    // init the host name
    hostname = RABBIT_MQ_HOST_NAME();

    // init the port
    port = RABBIT_MQ_PORT();

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

void rice_system::run() {

    // grab the input queue name
    string in_name = input_queue_name();

    // start the consuming
    amqp_basic_consume(in_channel.conn, 1, amqp_cstring_bytes(in_name.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(in_channel.conn), "Consuming");

    // TODO add the command queue here and move the input queue to another thread
    {
        for (;;) {
            amqp_rpc_reply_t res;
            amqp_envelope_t envelope;

            amqp_maybe_release_buffers(in_channel.conn);

            res = amqp_consume_message(in_channel.conn, &envelope, NULL, 0);

            if (AMQP_RESPONSE_NORMAL != res.reply_type) {
                break;
            }

            // parse the input..
            parser->parse((char*)envelope.message.body.bytes, envelope.message.body.len);

            amqp_destroy_envelope(&envelope);
        }
    }
}

rice_system::~rice_system() {
    // close the channel
    die_on_amqp_error(amqp_channel_close(in_channel.conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");

    // close the connection
    die_on_amqp_error(amqp_connection_close(in_channel.conn, AMQP_REPLY_SUCCESS), "Closing connection");

    // destroy the connection
    die_on_error(amqp_destroy_connection(in_channel.conn), "Ending connection");

    // free the parser
    delete parser;
}
