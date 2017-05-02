//
// Created by dimitrije on 16/04/17.
//

#ifndef HOBBITREWRITE_OUTPUT_COMPONENT_H
#define HOBBITREWRITE_OUTPUT_COMPONENT_H

#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <data/double_converter.h>
#include "component.h"
#include "data/readerwriterqueue.h"
#include "data/anormaly.h"

// the beginning of each line
#define ANOMALY_BEGIN "<http://project-hobbit.eu/resources/debs2017#Anomaly_"

// end of the line
#define ANOMALY_END "> .\n"

// the end for the first line
#define ANOMALY_LINE_1_PT_1 "> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/DEBSAnalyticResults#Anomaly> .\n"

// the second line of the anomaly - the machine number
#define ANOMALY_LINE_2_PT_1 "> <http://www.agtinternational.com/ontologies/I4.0#machine> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#Machine_"

// the third line - the machine dimension
#define ANOMALY_LINE_3_PT_1 "> <http://www.agtinternational.com/ontologies/DEBSAnalyticResults#inAbnormalDimension> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#_"

// the timestamp
#define ANOMALY_LINE_4_PT_1 "> <http://www.agtinternational.com/ontologies/DEBSAnalyticResults#hasTimeStamp> <http://project-hobbit.eu/resources/debs2017#Timestamp_"

// the probability part 2
#define ANOMALY_LINE_5_PT_1 "> <http://www.agtinternational.com/ontologies/DEBSAnalyticResults#hasProbabilityOfObservedAbnormalSequence> \""

// the probability part 1
#define ANOMALY_LINE_5_PT_2 "\"^^<http://www.w3.org/2001/XMLSchema#double> ."

using namespace concurent;

class output_component : public component {
private:

    // timeout
    static const int TIMEOUT = 1000000;

    // returns the output queue name
    string output_queue_name();

    // the response channel
    amqp_channel out_channel;

    // the response queue
    amqp_queue out_queue;

    // double converter
    double_converter dc;

    // mutex to sync the output
    mutex m;

    // anomaly counter
    size_t counter;

    // queue for the anomalies
    blocking_reader_writer_queue<anomaly> queue;

    // message buffer
    char message_buffer[4000];

    // fill the string buffer
    size_t fill_buffer(size_t count, size_t &machine_no, size_t &dimension_no, double &final_threshold, size_t &timestamp);

    //
    size_t ANOMALY_BEGIN_LEN;

    // end of the line length
    size_t ANOMALY_END_LEN;

    // the end for the first line length
    size_t ANOMALY_LINE_1_PT_1_LEN;

    // the second line of the anomaly length - the machine number
    size_t ANOMALY_LINE_2_PT_1_LEN;

    // the third line length - the machine dimension
    size_t ANOMALY_LINE_3_PT_1_LEN;

    // the timestamp length
    size_t ANOMALY_LINE_4_PT_1_LEN;

    // the probability part 2 length
    size_t ANOMALY_LINE_5_PT_1_LEN;

    // the probability part 1 length
    size_t ANOMALY_LINE_5_PT_2_LEN;

    // is the system finished.
    atomic_bool is_finished;

public:

    // constructor
    output_component();

    // destructor
    ~output_component();

    void output_anomaly(size_t idx, size_t machine_no, size_t dimension_no, double final_threshold, size_t timestamp);

    // send the bytes
    void send(const char* bytes, size_t length);

    // send the string
    void send(const string value);

    // run the component
    void run();

    void set_is_finished(bool value);
};


#endif //HOBBITREWRITE_OUTPUT_COMPONENT_H
