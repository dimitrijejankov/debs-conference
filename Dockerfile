FROM ubuntu:latest
RUN mkdir -p /usr/src/debs

WORKDIR /usr/src/debs

RUN apt-get update
RUN apt-get -y install cmake
RUN apt-get -y install librabbitmq-dev
RUN apt-get -y install g++

ADD . /usr/src/debs

RUN cmake CMakeLists.txt
RUN make

CMD ["./HobbitRewrite"]
