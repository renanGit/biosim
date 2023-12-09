#include <array>

#include "Sim.h"
#include "Config.h"
#include "Random.h"
#include "Grid.h"

namespace sim
{
    void Sim::Init()
    {
        grid = new Grid(Config::GetGridSizeX(), Config::GetGridSizeY());
        grid->Init();

        agents.resize(Config::GetMaxAgents());
        Position posNotSet(UINT16_MAX, UINT16_MAX, Direction::W);
        
        const int SIZE_X = grid->SizeX();
        const int SIZE_Y = grid->SizeY();
        
        for (auto& agent : agents)
        {
            Position startPos(
                rng.GetRNGuint16() % SIZE_X,
                rng.GetRNGuint16() % SIZE_Y,
                static_cast<Direction>(rng.GetRNGuint16() % Direction::D_COUNT));
            while (agent.initialPos == posNotSet)
            {
                if (!grid->Occupied(startPos))
                {
                    agent.initialPos.Set(startPos);
                    agent.pos.Set(startPos);
                    agent.nnet.grid = grid;
                    movementProducer.emplace(agent.pos);
                    break;
                }
                startPos.coordX = rng.GetRNGuint16() % SIZE_X;
                startPos.coordY = rng.GetRNGuint16() % SIZE_Y;
            }
        }
        movementProducer.emplace(posNotSet); // A delimiter
    }

    void Sim::Run(uint32_t stepsPerEpoch, uint32_t epochs)
    {
        if (runner.joinable())
        {
            runner.join();
        }
        runner = std::thread(ThreadRun, this, stepsPerEpoch, epochs);
    }

    void Sim::ThreadRun(uint32_t stepsPerEpoch, uint32_t epochs)
    {
        Position posNotSet(UINT16_MAX, UINT16_MAX, Direction::W);

        for (uint32_t epoch = 0; epoch < epochs; epoch++)
        {
            for (uint32_t step = 0; step < stepsPerEpoch; step++)
            {
                for (auto& agent : agents)
                {
                    agent.SimAgent(step);
                    movementProducer.emplace(agent.pos);
                }
                movementProducer.emplace(posNotSet); // A delimiter, to know the end of a step
            }

            // Mate
        }
    }
}