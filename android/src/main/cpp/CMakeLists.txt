cmake_minimum_required(VERSION 3.10.2)

project(saf_stream)

add_library(saf_stream SHARED
        saf_stream.cpp)

find_library(log-lib log)

target_link_libraries(saf_stream
        ${log-lib})
