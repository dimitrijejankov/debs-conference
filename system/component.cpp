//
// Created by dimitrije on 15/04/17.
//

#include <amqp_tcp_socket.h>
#include "component.h"
#include "../utils.h"
#include "../parameters.h"

component::component() {
    // init the host name
    hostname = RABBIT_MQ_HOST_NAME();

    // init the port
    port = RABBIT_MQ_PORT();
}

void component::init_channel(channel &c) {
    // create a connection object
    c.conn  = amqp_new_connection();

    // create a socket
    c.socket = amqp_tcp_socket_new(c.conn);
    if (!c.socket) {
        die("creating TCP socket");
    }

    // open the socket
    c.status = amqp_socket_open(c.socket, hostname.c_str(), port);
    if (c.status) {
        die("opening TCP socket");
    }

    // login to the rabbitMQ if fail exit
    die_on_amqp_error(amqp_login(c.conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest"), "Logging in");

    // open a channel to the rabbitMQ
    amqp_channel_open(c.conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Opening channel");

    // set the prefetch count to 1
    auto qos_status = amqp_basic_qos(c.conn, 1, 0, 1, true);
    if(qos_status == NULL) {
        die("setting prefetch count");
    }
}


void component::init_queue(channel &c, queue &q, string queue_name) {
    // declare queue
    amqp_queue_declare_ok_t *r = amqp_queue_declare(c.conn, 1, amqp_cstring_bytes(queue_name.c_str()), 0, 0, 0, 1, amqp_empty_table);

    // if we failed to create a queue exit
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Declaring queue");
}

string component::init_queue(channel &c, queue &q) {
    // declare queue
    amqp_queue_declare_ok_t *r = amqp_queue_declare(c.conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);

    // if we failed to create a queue exit
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Declaring anonymous queue");

    // return the queue name
    return string((char*) amqp_bytes_malloc_dup(r->queue).bytes, r->queue.len);
}

void component::bind_exchange(channel &c, queue &q, string &queue, string &exchange) {

    // declare the exchange
    amqp_exchange_declare(c.conn, 1, amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes("fanout"),
                          0, 0, 0, 0, amqp_empty_table);

    // exit if error
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Declaring exchange");

    // bind the exchange
    amqp_queue_bind(c.conn, 1, amqp_cstring_bytes(queue.c_str()), amqp_cstring_bytes(exchange.c_str()),
                    amqp_cstring_bytes(""), amqp_empty_table);

    // exit if error
    die_on_amqp_error(amqp_get_rpc_reply(c.conn), "Binding queue");
}
