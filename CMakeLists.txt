cmake_minimum_required(VERSION 3.5)
project(HobbitRewrite)
include(CheckCSourceCompiles)

# set the standard to C++ 11
set(CMAKE_CXX_STANDARD 11)

#find pthreads
find_package (Threads)

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

# set the include directories
include_directories(parser system .)

# if found include the RabbitMQ
set(CMAKE_REQUIRED_INCLUDES ${RABBITMQ_HEADERS})

# grab the source files
file(GLOB PROJECT_SOURCE "system/*.cpp" "parser/*.cpp" "kmeans/*.cpp" "*.cpp")

# grab the headers files
file(GLOB PROJECT_HEADERS "system/*.h" "parser/*.h" "kmeans/*.h" "*.h")

# set the sources
set(SOURCE_FILES ${PROJECT_SOURCE} ${PROJECT_HEADERS})

# set the executable
add_executable(HobbitRewrite ${SOURCE_FILES})

# link rabbitmq
target_link_libraries(HobbitRewrite ${RABBITMQ_LIBRARIES})

# link pthreads
target_link_libraries (HobbitRewrite ${CMAKE_THREAD_LIBS_INIT})