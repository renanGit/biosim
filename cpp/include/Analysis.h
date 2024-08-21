#pragma once

#include <vector>
#include <map>
#include <cstdint>

namespace sim
{
    class NeuralNet;
    class Genome;

    class Analysis
    {
    public:
        static void SingleAgentRun();
        static void SimRun();
        static std::vector<long> GraphAnalysis(NeuralNet& nnet);
        static std::vector<long> GenomeAnalysis(Genome& genome);
        static std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>> WeightAnalysis(Genome& genome);
    };
}