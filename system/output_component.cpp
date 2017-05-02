//
// Created by dimitrije on 16/04/17.
//

#include <cstring>
#include "output_component.h"
#include "parameters.h"
#include "utils.h"


string output_component::output_queue_name() {
    return SYSTEM_2_EVAL_STORAGE_QUEUE_NAME() + "." + HOBBIT_SESSION_ID();
}

output_component::output_component() : counter(0), queue(400)  {
    out_channel.socket = NULL;

    // init the input channel
    init_channel(out_channel);

    // init the input amqp_queue
    init_queue(out_channel, out_queue, output_queue_name());

    // the system just started...
    is_finished = false;

    // the length of the anomaly begin
    ANOMALY_BEGIN_LEN = strlen(ANOMALY_BEGIN);

    // end of the line length
    ANOMALY_END_LEN = strlen(ANOMALY_END);

    // the end for the first line length
    ANOMALY_LINE_1_PT_1_LEN = strlen(ANOMALY_LINE_1_PT_1);

    // the second line of the anomaly length - the machine number
    ANOMALY_LINE_2_PT_1_LEN = strlen(ANOMALY_LINE_2_PT_1);

    // the third line length - the machine dimension
    ANOMALY_LINE_3_PT_1_LEN = strlen(ANOMALY_LINE_3_PT_1);

    // the timestamp length
    ANOMALY_LINE_4_PT_1_LEN = strlen(ANOMALY_LINE_4_PT_1);

    // the probability part 2 length
    ANOMALY_LINE_5_PT_1_LEN = strlen(ANOMALY_LINE_5_PT_1);

    // the probability part 1 length
    ANOMALY_LINE_5_PT_2_LEN = strlen(ANOMALY_LINE_5_PT_2);
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

void output_component::output_anomaly(size_t idx, size_t machine_no, size_t dimension_no, double final_threshold, size_t timestamp) {

    // just so we don't have to grab new memory each time
    static anomaly tmp;

    tmp.idx = idx;
    tmp.machine_no = machine_no;
    tmp.dimension_no = dimension_no;
    tmp.final_threshold = final_threshold;
    tmp.timestamp = timestamp;

    // push the anomaly into the buffer.
    queue.enqueue(tmp);
}

void output_component::run() {

    // grab the anomaly
    anomaly a;

    //uint64_t duration = 0;
    //uint64_t n = 0;

    for(;;) {

        // grab the anomaly
        bool grabbed = queue.wait_dequeue_timed(a, TIMEOUT);

        //std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        // if the system is finished and there is nothing in the queue finish the thread
        if(!grabbed && is_finished) {
            break;
        }

        // if we don't have an anomaly continue
        if(!grabbed) {
            continue;
        }

        // make the message
        size_t size = fill_buffer(counter++, a.machine_no, a.dimension_no, a.final_threshold, a.timestamp);

        // send the message
        send(message_buffer, size - 1);

        //std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        //n++;
        //duration += ( t2 - t1 ).count();

        //cout << "Elapsed time : " << duration / n << endl;
    }
}

size_t output_component::fill_buffer(size_t anomaly_no, size_t &machine_no, size_t &dimension_no, double &final_threshold,
                                   size_t &timestamp) {

    char machine_string[5];
    char anomaly_string[5];
    char dimension_string[5];
    char timestamp_string[10];
    char probability_string[30];

    sprintf(machine_string, "%lu", machine_no);
    sprintf(anomaly_string, "%lu", anomaly_no);
    sprintf(dimension_string, "%lu", dimension_no);
    sprintf(timestamp_string, "%lu", timestamp);

    dc.getBinaryToASCIIConverter(final_threshold, true);
    dc.copy_to_string(probability_string);
    //sprintf(probability_string, "%.16lg", final_threshold);

    size_t machine_string_len = strlen(machine_string);
    size_t anomaly_string_len = strlen(anomaly_string);
    size_t dimension_string_len = strlen(dimension_string);
    size_t timestamp_string_len = strlen(timestamp_string);
    size_t probability_string_len = strlen(probability_string);


    char *offset = message_buffer;

    /// start of the line 1

    // copy the beginning
    memccpy(offset, ANOMALY_BEGIN, sizeof(char), ANOMALY_BEGIN_LEN);

    // increase the offset
    offset += ANOMALY_BEGIN_LEN;

    // copy the anomaly number
    memccpy(offset, anomaly_string, sizeof(char), anomaly_string_len);

    // increase the offset
    offset += anomaly_string_len;

    // copy the line part 1
    memccpy(offset, ANOMALY_LINE_1_PT_1, sizeof(char), ANOMALY_LINE_1_PT_1_LEN);

    // increase the offset
    offset += ANOMALY_LINE_1_PT_1_LEN;

    /// start of the line 2

    // copy the beginning and the anomaly number
    memccpy(offset, message_buffer, sizeof(char), ANOMALY_BEGIN_LEN + anomaly_string_len);

    // increase the offset
    offset += ANOMALY_BEGIN_LEN + anomaly_string_len;

    // copy the beginning and the ANOMALY_LINE_2_PT_1
    memccpy(offset, ANOMALY_LINE_2_PT_1, sizeof(char), ANOMALY_LINE_2_PT_1_LEN);

    // increase the offset
    offset += ANOMALY_LINE_2_PT_1_LEN;

    // copy the machine_string
    memccpy(offset, machine_string, sizeof(char), machine_string_len);

    // increase the offset
    offset += machine_string_len;

    // copy the anomaly end
    memccpy(offset, ANOMALY_END, sizeof(char), ANOMALY_END_LEN);

    // increase the offset
    offset += ANOMALY_END_LEN;

    /// start of line 3

    // copy the beginning and the anomaly number
    memccpy(offset, message_buffer, sizeof(char), ANOMALY_BEGIN_LEN + anomaly_string_len);

    // increase the offset
    offset += ANOMALY_BEGIN_LEN + anomaly_string_len;

    // copy the beginning and the ANOMALY_LINE_3_PT_1
    memccpy(offset, ANOMALY_LINE_3_PT_1, sizeof(char), ANOMALY_LINE_3_PT_1_LEN);

    // increase the offset
    offset += ANOMALY_LINE_3_PT_1_LEN;

    // copy the machine_string
    memccpy(offset, machine_string, sizeof(char), machine_string_len);

    // increase the offset
    offset += machine_string_len;

    // add the underscore
    *offset = '_';

    // increase the offset
    offset++;

    // copy the dimension string
    memccpy(offset, dimension_string, sizeof(char), dimension_string_len);

    // increase the offset
    offset += dimension_string_len;

    // copy the anomaly end
    memccpy(offset, ANOMALY_END, sizeof(char), ANOMALY_END_LEN);

    // increase the offset
    offset += ANOMALY_END_LEN;

    /// start of line 4

    // copy the beginning and the anomaly number
    memccpy(offset, message_buffer, sizeof(char), ANOMALY_BEGIN_LEN + anomaly_string_len);

    // increase the offset
    offset += ANOMALY_BEGIN_LEN + anomaly_string_len;

    // copy the beginning and the ANOMALY_LINE4_PT_1
    memccpy(offset, ANOMALY_LINE_4_PT_1, sizeof(char), ANOMALY_LINE_4_PT_1_LEN);

    // increase the offset
    offset += ANOMALY_LINE_4_PT_1_LEN;

    // copy the dimension string
    memccpy(offset, timestamp_string, sizeof(char), timestamp_string_len);

    // increase the offset
    offset += timestamp_string_len;

    // copy the anomaly end
    memccpy(offset, ANOMALY_END, sizeof(char), ANOMALY_END_LEN);

    // increase the offset
    offset += ANOMALY_END_LEN;

    /// start of line 5

    // copy the beginning and the anomaly number
    memccpy(offset, message_buffer, sizeof(char), ANOMALY_BEGIN_LEN + anomaly_string_len);

    // increase the offset
    offset += ANOMALY_BEGIN_LEN + anomaly_string_len;

    // copy the beginning and the ANOMALY_LINE5_PT_1
    memccpy(offset, ANOMALY_LINE_5_PT_1, sizeof(char), ANOMALY_LINE_5_PT_1_LEN);

    // increase the offset
    offset += ANOMALY_LINE_5_PT_1_LEN;

    // copy the dimension string
    memccpy(offset, probability_string, sizeof(char), probability_string_len);

    // increase the offset
    offset += probability_string_len;

    // copy the beginning and the ANOMALY_LINE5_PT_1
    memccpy(offset, ANOMALY_LINE_5_PT_2, sizeof(char), ANOMALY_LINE_5_PT_2_LEN);

    // increase the offset
    offset += ANOMALY_LINE_5_PT_2_LEN;

    // add the end line
    *offset = '\0';

    // increase the offset
    offset++;

    return offset - this->message_buffer;
}

void output_component::set_is_finished(bool value) {
    is_finished = value;
}
