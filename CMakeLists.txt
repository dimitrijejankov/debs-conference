cmake_minimum_required(VERSION 3.6)
project(HobbitRewrite)
include(CheckCSourceCompiles)

# set the standard to C++ 11
set(CMAKE_CXX_STANDARD 11)

# find the RabbitMQ
find_library(RABBITMQ_LIBRARIES NAMES rabbitmq)
find_path(RABBITMQ_HEADERS amqp.h)

if(${RABBITMQ_LIBRARIES} MATCHES "NOTFOUND")
    set(Rabbitmq_FOUND FALSE CACHE INTERNAL "")
    message(STATUS "RabbitMQ library not found.")
    unset(RABBITMQ_LIBRARIES)
else()
    set(Rabbitmq_FOUND TRUE CACHE INTERNAL "")
    message(STATUS "Found RabbitMQ library: ${RABBITMQ_LIBRARIES}")
endif()

# if found include the RabbitMQ
set(CMAKE_REQUIRED_INCLUDES ${RABBITMQ_HEADERS})

# set the sources
set(SOURCE_FILES main.cpp parameters.h)

# set the executable
add_executable(HobbitRewrite ${SOURCE_FILES})

# link rabbitmq
target_link_libraries(HobbitRewrite ${RABBITMQ_LIBRARIES})