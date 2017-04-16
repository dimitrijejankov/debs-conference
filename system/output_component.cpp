//
// Created by dimitrije on 16/04/17.
//

#include "output_component.h"
#include "parameters.h"
#include "utils.h"


string output_component::output_queue_name() {
    return SYSTEM_2_EVAL_STORAGE_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

output_component::output_component() {
    out_channel.socket = NULL;

    // init the input channel
    init_channel(out_channel);

    // init the input queue
    init_queue(out_channel, out_queue, output_queue_name());
}

output_component::~output_component() {
    // close the channel
    die_on_amqp_error(amqp_channel_close(out_channel.conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");

    // close the connection
    die_on_amqp_error(amqp_connection_close(out_channel.conn, AMQP_REPLY_SUCCESS), "Closing connection");

    // destroy the connection
    die_on_error(amqp_destroy_connection(out_channel.conn), "Ending connection");
}

void output_component::send(const char *bytes, size_t length) {

    // the mutex to sync the system
    unique_lock<mutex> lk(m);

    // make space on for the props
    amqp_basic_properties_t props;

    // set the flags for data we have
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG | AMQP_BASIC_PRIORITY_FLAG;

    // set the content type
    props.content_type = amqp_cstring_bytes("application/octet-stream");

    // set the delivery mode
    props.delivery_mode = 2; // persistent delivery mode

    // set the priority
    props.priority = 0;

    // make a message bytes struct
    amqp_bytes_t message_bytes;

    // fill it up
    message_bytes.len = length;
    message_bytes.bytes = (void *) bytes;

    // send the message...
    die_on_error(amqp_basic_publish(out_channel.conn,
                                    1,
                                    amqp_empty_bytes,
                                    amqp_cstring_bytes(output_queue_name().c_str()),
                                    0,
                                    0,
                                    &props,
                                    message_bytes), "Publishing output");
}

void output_component::send(const string value) {
    send(value.c_str(), value.size());
}
