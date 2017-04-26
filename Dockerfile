FROM ubuntu:latest
RUN mkdir -p /usr/src/debs

WORKDIR /usr/src/debs

RUN apt-get update
RUN apt-get -y install cmake
RUN apt-get -y install librabbitmq-dev
RUN apt-get -y install g++
RUN apt-get -y install default-jdk

ADD . /usr/src/debs

RUN cmake CMakeLists.txt
RUN make HobbitRewrite

CMD ["./HobbitRewrite"]
