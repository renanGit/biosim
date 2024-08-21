add_library(darwin SHARED Agent.cpp Config.cpp Genome.cpp NeuralNet.cpp Sim.cpp
    include/Agent.h include/Config.h include/Gene.h include/Genome.h include/Grid.h
    include/NeuralNet.h include/Random.h include/Sim.h include/Types.h
)
include_directories(include)
add_compile_options(-Wall)