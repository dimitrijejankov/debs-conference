//
// Created by dimitrije on 15/04/17.
//

#include <cstring>
#include "command_component.h"
#include "parameters.h"
#include "utils.h"

command_component::command_component() {
    // initialize the response channel
    init_channel(response_channel);

    // initialize the response queue
    response_queue_name = init_queue(response_channel, response_queue);

    // grab the exchange name
    exchange_name = HOBBIT_COMMAND_EXCHANGE_NAME();

    // bind the exchange
    bind_exchange(response_channel, response_queue, response_queue_name, exchange_name);

    // session id
    session_id = HOBBIT_SESSION_ID();

    // insert the session id
    accepted_command_headers.insert(session_id);

    // system is ready
    task_generation_finished = false;

}

void command_component::send_to_cmd_queue(char command) {
    send_to_cmd_queue(command, nullptr, 0);
}

void command_component::send_to_cmd_queue(char command, char *data, size_t length) {
    send_to_cmd_queue(command, data, length, nullptr);
}

void command_component::send_to_cmd_queue(char command, char *data, size_t length, amqp_basic_properties_t_ const *props) {

    int32_t session_id_len = (int32_t) session_id.size();

    // figure out the length
    size_t dataLength = session_id.size() + 5;
    bool attachData = (data != nullptr) && (length > 0);
    if (attachData) {
        dataLength += length;
    }

    // allocate the buffer
    char* buffer = (char*)malloc(sizeof(char) * length);

    // copy the session id length
    memcpy(buffer, &session_id_len, sizeof(int32_t));

    // swap the endian in the buffer
    swap_endian(*((int32_t*)buffer));

    // copy the session id
    memcpy(buffer + sizeof(int32_t), session_id.c_str(), session_id.size());

    // copy the command
    memcpy(buffer + sizeof(int32_t) + session_id_len, &command, sizeof(char));

    // make a message bytes struct
    amqp_bytes_t message_bytes;

    // fill it up
    message_bytes.len = dataLength;
    message_bytes.bytes = buffer;

    // send the message...
    die_on_error(amqp_basic_publish(response_channel.conn,
                                    1,
                                    amqp_cstring_bytes(exchange_name.c_str()),
                                    amqp_empty_bytes,
                                    0,
                                    0,
                                    props,
                                    message_bytes), "Publishing");

    // free the buffer
    free(buffer);
}

void command_component::run() {

    // start the consuming
    amqp_basic_consume(response_channel.conn, 1, amqp_cstring_bytes(response_queue_name.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(response_channel.conn), "Consuming");

    // TODO add the command queue here and move the input queue to another thread
    {
        for (;;) {
            amqp_rpc_reply_t res;
            amqp_envelope_t envelope;

            amqp_maybe_release_buffers(response_channel.conn);

            res = amqp_consume_message(response_channel.conn, &envelope, NULL, 0);

            if (AMQP_RESPONSE_NORMAL != res.reply_type) {
                break;
            }

            // parse the input..
            handle_command((char*)envelope.message.body.bytes, envelope.message.body.len, envelope.message.properties);

            amqp_destroy_envelope(&envelope);
        }
    }
}

void command_component::handle_command(char* bytes, size_t length, amqp_basic_properties_t props) {
    // allocate on stack for the command session id length
    int32_t command_session_len;

    // figure out the length of the session id
    memcpy(&command_session_len, bytes, sizeof(int32_t));

    swap_endian(command_session_len);

    // grab the session id
    string command_session_id = string(bytes + sizeof(int32_t), (unsigned long) command_session_len);

    // if the session id is accepted command headers
    if (accepted_command_headers.find(command_session_id) != accepted_command_headers.end()) {

        // get the command
        char command = *(bytes + sizeof(int32_t) + command_session_len);

        // remaining data
        char* remainingData;

        // figure out the length
        size_t remainingDataLen = length - sizeof(int32_t) - command_session_len - sizeof(char);

        // if we have still data
        if (remainingDataLen > 0) {
            remainingData = bytes + remainingDataLen;
        } else {
            // we have no data
            remainingData = nullptr;
        }

        // process the received command
        receive_command(command, remainingData, remainingDataLen);
    }
}

void command_component::receive_command(char command, char *remainingData, size_t length) {
    if (command == DOCKER_CONTAINER_TERMINATED) {
        // TODO implement container observers
    }
    else if (command == TASK_GENERATION_FINISHED) {
        task_generation_finished = true;
    }
}

command_component::~command_component() {
    // close the channel
    die_on_amqp_error(amqp_channel_close(response_channel.conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");

    // close the connection
    die_on_amqp_error(amqp_connection_close(response_channel.conn, AMQP_REPLY_SUCCESS), "Closing connection");

    // destroy the connection
    die_on_error(amqp_destroy_connection(response_channel.conn), "Ending connection");
}

void command_component::swap_endian(int32_t &value) {
    value = ( value >> 24 ) | (( value << 8) & 0x00ff0000 )| ((value >> 8) & 0x0000ff00) | ( value << 24)  ;
}
