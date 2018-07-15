cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

project(tcpTunnel)
aux_source_directory(Tunnels TSRCS)
aux_source_directory(. SRCS)
add_executable(tcpTunnel ${SRCS} ${TSRCS})
