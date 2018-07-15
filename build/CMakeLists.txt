cmake_minimum_required(VERSION 2.8)
project(tcpTunnel)
aux_source_directory(../src/ SRCS)
add_executable(tcpTunnel ${SRCS})
