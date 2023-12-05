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

        for (auto& agent : agents)
        {
            Position startPos(
                rng.GetRNGuint16() % grid->SizeX(),
                rng.GetRNGuint16() % grid->SizeY(),
                static_cast<Direction>(rng.GetRNGuint16() % Direction::D_COUNT));
            while (agent.initialPos == posNotSet)
            {
                if (!grid->Occupied(startPos))
                {
                    agent.initialPos = std::move(startPos);
                    agent.pos = agent.initialPos;
                    agent.nnet.grid = grid;
                    break;
                }
                startPos.coordX = rng.GetRNGuint16() % grid->SizeX();
                startPos.coordY = rng.GetRNGuint16() % grid->SizeY();
            }
        }
    }

    void Sim::Run(uint32_t stepsPerEpoch, uint32_t epochs)
    {
        for (uint32_t epoch = 0; epoch < epochs; epoch++)
        {
            for (uint32_t step = 0; step < stepsPerEpoch; step++)
            {
                for (auto& agent : agents)
                {
                    agent.SimAgent(step);
                }
            }

            // Mate
        }
    }
}