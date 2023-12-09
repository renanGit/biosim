#pragma once

#include <vector>
#include <cstdint>
#include <queue>
#include <thread>

#include "Agent.h"

namespace sim
{
    class Grid;

    class Sim
    {
    public:
        Sim() : grid(nullptr), killSig(false) {}
        ~Sim()
        {
            if (grid != nullptr)
            {
                delete grid;
            }
            killSig = true;
            if (runner.joinable())
            {
                runner.join();
            }
        }
        void Init();
        void Run(uint32_t stepsPerEpoch, uint32_t epochs);

        inline bool CanPollMovement() { return !movementProducer.empty(); }
        inline Position ConsumeMovement()
        {
            Position _pos = movementProducer.front();
            movementProducer.pop();
            return _pos;
        }
    
    private:
        void ThreadRun(uint32_t stepsPerEpoch, uint32_t epochs);

    private:
        Grid* grid;
        std::vector<Agent> agents;
        // A consumer / producer. Python consumes, C++ produces.
        std::queue<Position> movementProducer;
        std::thread runner;
        bool killSig;
    };
}