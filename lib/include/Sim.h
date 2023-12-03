#pragma once

#include <vector>
#include <cstdint>

#include "Agent.h"

namespace sim
{
    class Grid;

    class Sim
    {
    public:
        Sim() : grid(nullptr) {}
        ~Sim()
        {
            if (grid != nullptr)
            {
                delete grid;
            }
        }
        void Init();
        void Run(uint32_t stepsPerEpoch, uint32_t epochs);
    
    private:
        Grid* grid;
        std::vector<Agent> agents;
    };
}