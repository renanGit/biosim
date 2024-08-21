cmake_minimum_required(VERSION 3.20)

project(darwin VERSION 1.0)

add_executable(darwin_service main.cpp Agent.cpp Config.cpp Genome.cpp NeuralNet.cpp Sim.cpp Analysis.cpp)

set_target_properties(darwin_service PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(include)
add_compile_options(-Wall)