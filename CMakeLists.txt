cmake_minimum_required(VERSION 3.6)
project(GENN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    examples/main.cpp
    examples/Xor.cpp
    examples/Xor.h
    include/Genome.h
    include/NEAT.h
    include/Network.h
    include/Simulation.h
    lib/base64.cpp
    lib/base64.h
    lib/tigr.c
    lib/tigr.h
    src/Genome.cpp
    src/NEAT.cpp
    src/Network.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(GENN ${SOURCE_FILES})

target_link_libraries (GENN d3d9.lib)