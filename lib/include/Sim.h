#pragma once

#include <vector>
#include <cstdint>
#include <array>
#include <thread>
#include <chrono>
#include <atomic>

#include "Agent.h"

namespace sim
{
    class Grid;

    class Sim
    {
    public:
        Sim() : grid(nullptr), producerPtr(1), consumerPtr(0), killSig(false) {}
        ~Sim()
        {
            if (producer.joinable())
            {
                killSig = true;
                producer.join();
            }
            if (grid != nullptr)
            {
                delete grid;
                grid = nullptr;
            }
        }
        void Init();
        void Run(uint32_t stepsPerEpoch, uint32_t epochs);

        inline bool CanPollMovement() { return producerPtr != (consumerPtr + 1); }
        inline Position ConsumeMovement()
        {
            consumerPtr++;
            consumerPtr = consumerPtr % movements.size();
            auto pos = movements[consumerPtr];
            return pos;
        }
    
    private:
        void SetupGeneration();

        static void ThreadRun(Sim* sim, uint32_t stepsPerEpoch, uint32_t epochs);
        void NewGeneration();
        // Parent 1 is treated as dominant
        void Mate(const Agent& parent1, const Agent& parent2, Agent& child);

        inline void ProduceMovement(const Position& pos)
        {
            while (producerPtr == consumerPtr && !killSig)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            if (!killSig)
            {
                movements[producerPtr] = pos;
                producerPtr++;
                producerPtr = producerPtr % movements.size();
            }
        }

        inline void Reset()
        {
            producerPtr = 1;
            consumerPtr = 0;
            killSig = false;
            for (auto agent : agents)
            {
                agent.pos = agent.initialPos;
            }
        }

    private:
        Grid* grid;
        std::vector<Agent> agents;
        std::thread producer;

        // Circular producer/consumer
        std::array<Position, 10000> movements;
        // Starts at index 1
        std::atomic_int producerPtr;
        // Lagging index, starts at index 0, but consumes the next index
        std::atomic_int consumerPtr;
        std::atomic_bool killSig;
        
        friend class Analysis;
    };
}