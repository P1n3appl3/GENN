cmake_minimum_required(VERSION 3.6)
project(GENN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Genome.cpp Genome.h NEAT.cpp NEAT.h Simulation.h problems/Xor.h problems/Network.cpp problems/Network.h)
add_executable(GENN ${SOURCE_FILES})