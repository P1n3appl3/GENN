cmake_minimum_required(VERSION 3.6)
project(GENN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp NEAT.cpp NEAT.h Genome.cpp Genome.h Connection.h Neuron.h)
add_executable(GENN ${SOURCE_FILES})