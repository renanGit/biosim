#pragma once

#include "NeuralNet.h"
#include "Genome.h"
#include "Grid.h"

namespace sim
{
    class Agent
    {
    public:
        Agent() : initialPos(UINT16_MAX, UINT16_MAX, Direction::W), stepsTaken(0), rotationsTaken(0)
        { }
        void Init();
        void SimAgent(uint32_t simStep);
        void WireGenome();
    private:
        void InvokeActions();
    public:
        NeuralNet nnet;
        Genome genome;
        Position initialPos;
        Position pos;
        // A criteria for survival
        uint32_t stepsTaken;
        uint32_t rotationsTaken;

        int genomeSizeBefore;
        int genomeSizeAfter;
    };
}