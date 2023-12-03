#pragma once

#include "NeuralNet.h"
#include "Genome.h"
#include "Grid.h"

namespace sim
{
    class Agent
    {
    public:
        Agent()
        {
            Position _initialPos(UINT16_MAX, UINT16_MAX, Direction::W);
            initialPos = std::move(_initialPos);
        }
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

        int genomeSizeBefore;
        int genomeSizeAfter;
    };
}