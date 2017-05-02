//
// Created by dimitrije on 5/1/17.
//

#ifndef HOBBITREWRITE_RDF_CONSTANTS_H
#define HOBBITREWRITE_RDF_CONSTANTS_H

// the text to skip from the beginning
#define HEADER_SKIP_1_STR "<http://project-hobbit.eu/resources/debs2017#ObservationGroup_x> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#MoldingMachineObservationGroup> .\n<http://project-hobbit.eu/resources/debs2017#ObservationGroup_0> <http://purl.oclc.org/NET/ssnx/ssn#observationResultTime> <http://project-hobbit.eu/resources/debs2017#Timestamp_x> ."

// the skip to the machine number
#define MACHINE_SKIP_STR "<http://project-hobbit.eu/resources/debs2017#ObservationGroup_0> <http://www.agtinternational.com/ontologies/I4.0#machine> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#Machine_"

// skip to the timestamp
#define HEADER_SKIP_2_STR " .\n<http://project-hobbit.eu/resources/debs2017#ObservationGroup_0> <http://www.agtinternational.com/ontologies/I4.0#observedCycle> <http://project-hobbit.eu/resources/debs2017#Cycle_0> .\n<http://project-hobbit.eu/resources/debs2017#Cycle_0> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#Cycle> .\n<http://project-hobbit.eu/resources/debs2017#Cycle_0> <http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> \"13\"^^<http://www.w3.org/2001/XMLSchema#int> .\n<http://project-hobbit.eu/resources/debs2017#Timestamp_0> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/IoTCore#Timestamp> ."

// skip to the index of the timestamp
#define TIMESTAMP_SKIP_1_STR "<http://project-hobbit.eu/resources/debs2017#Timestamp_"

// skip to the timestamp value
#define TIMESTAMP_SKIP_2_STR " <http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> \""

// skip to the end
#define HEADER_SKIP_3_STR "\"^^<http://www.w3.org/2001/XMLSchema#dateTime> .\n"

// skip the date values
#define DATE_VALUE_SKIP_1_STR "2017-01-"
#define DATE_VALUE_SKIP_2_STR "T01:"

// one whole data point
#define DATA_POINT_CONTENT "<http://project-hobbit.eu/resources/debs2017#ObservationGroup_X> <http://www.agtinternational.com/ontologies/I4.0#contains> <http://project-hobbit.eu/resources/debs2017#Observation_X> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_X> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#MoldingMachineObservation> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_X> <http://purl.oclc.org/NET/ssnx/ssn#observationResult> <http://project-hobbit.eu/resources/debs2017#Output_X> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_X> <http://purl.oclc.org/NET/ssnx/ssn#observedProperty> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#_X_X> .\n\
<http://project-hobbit.eu/resources/debs2017#Output_X> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://purl.oclc.org/NET/ssnx/ssn#SensorOutput> .\n\
<http://project-hobbit.eu/resources/debs2017#Output_X> <http://purl.oclc.org/NET/ssnx/ssn#hasValue> <http://project-hobbit.eu/resources/debs2017#Value_X> .\n\
<http://project-hobbit.eu/resources/debs2017#Value_X> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#NumberValue> .\n\
<http://project-hobbit.eu/resources/debs2017#Value_X> <http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> \"\"^^<http://www.w3.org/2001/XMLSchema#double> ."

// the skip on the dimension
#define DIMENSION_SKIP_STR "<http://project-hobbit.eu/resources/debs2017#ObservationGroup_0> <http://www.agtinternational.com/ontologies/I4.0#contains> <http://project-hobbit.eu/resources/debs2017#Observation_0> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_0> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#MoldingMachineObservation> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_0> <http://purl.oclc.org/NET/ssnx/ssn#observationResult> <http://project-hobbit.eu/resources/debs2017#Output_0> .\n\
<http://project-hobbit.eu/resources/debs2017#Observation_0> <http://purl.oclc.org/NET/ssnx/ssn#observedProperty> <http://www.agtinternational.com/ontologies/WeidmullerMetadata#_"

//the skip on the value
#define VALUE_SKIP_STR "> .\n<http://project-hobbit.eu/resources/debs2017#Output_0> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://purl.oclc.org/NET/ssnx/ssn#SensorOutput> .\n<http://project-hobbit.eu/resources/debs2017#Output_0> <http://purl.oclc.org/NET/ssnx/ssn#hasValue> <http://project-hobbit.eu/resources/debs2017#Value_0> .\n<http://project-hobbit.eu/resources/debs2017#Value_0> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.agtinternational.com/ontologies/I4.0#NumberValue> .\n<http://project-hobbit.eu/resources/debs2017#Value_0> <http://www.agtinternational.com/ontologies/IoTCore#valueLiteral> "

#define DATA_POINT_SKIP_STR "\"^^<http://www.w3.org/2001/XMLSchema#double> ."

#endif //HOBBITREWRITE_RDF_CONSTANTS_H
