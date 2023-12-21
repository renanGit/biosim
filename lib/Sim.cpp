#include <iostream>
#include <array>
#include <algorithm>

#include "Sim.h"
#include "Config.h"
#include "Random.h"
#include "Grid.h"

namespace sim
{
    void Sim::SetupGeneration()
    {
        grid->Init();
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
                    ProduceMovement(agent.pos);
                    break;
                }
                startPos.coordX = rng.GetRNGuint16() % SIZE_X;
                startPos.coordY = rng.GetRNGuint16() % SIZE_Y;
            }
        }
        ProduceMovement(posNotSet); // A delimiter
    }

    void Sim::Init()
    {
        if (grid == nullptr)
        {
            grid = new Grid(Config::GetGridSizeX(), Config::GetGridSizeY());
        }

        agents.resize(Config::GetMaxAgents());
        for (auto& agent : agents)
        {
            agent.Init();
        }
        SetupGeneration();
    }

    void Sim::Run(uint32_t stepsPerEpoch, uint32_t epochs)
    {
        Reset();
        printf("Steps per Epoch: %u, Epochs: %u\n", stepsPerEpoch, epochs);
        producer = std::thread(&Sim::ThreadRun, this, stepsPerEpoch, epochs);
    }

    void Sim::ThreadRun(Sim* sim, uint32_t stepsPerEpoch, uint32_t epochs)
    {
        Position posNotSet(UINT16_MAX, UINT16_MAX, Direction::W);

        for (uint32_t epoch = 0; epoch < epochs; epoch++)
        {
            for (uint32_t step = 0; step < stepsPerEpoch; step++)
            {
                if (sim->killSig)
                {
                    return;
                }

                for (auto& agent : sim->agents)
                {
                    agent.SimAgent(step);

                    sim->ProduceMovement(agent.pos);
                }
                sim->ProduceMovement(posNotSet); // A delimiter
            }

            sim->NewGeneration();
            sim->SetupGeneration();
            printf("Epoch %u completed...\n", epoch+1);
        }
    }

    void Sim::Mate(const Agent& parent1, const Agent& parent2, Agent& child)
    {
        auto CutPositions = [&](const auto& p, const float cutPercent, int& start, int& end)
        {
            int size = static_cast<int>(p.genome.genome.size());
            start = std::min(static_cast<int>(std::floor(size * (1.0f - cutPercent))), rng.GetRNGuint16() % size);
            end = std::min(static_cast<int>(start + std::ceil(size * cutPercent)), size);
        };
        
        const float recesiveCutPercent = 1.0f - Config::GetMinDominantCutPercent();

        int offset1 = 0, cutPos1 = 0;
        CutPositions(parent1, Config::GetMinDominantCutPercent(), offset1, cutPos1);
        int offset2 = 0, cutPos2 = 0;
        CutPositions(parent2, recesiveCutPercent, offset2, cutPos2);
        
        std::vector<Gene>& childGenes = child.genome.genome;
        childGenes.clear();
        childGenes.insert(childGenes.begin(), parent1.genome.genome.begin() + offset1, parent1.genome.genome.begin() + cutPos1);
        childGenes.insert(childGenes.end(), parent2.genome.genome.begin() + offset2, parent2.genome.genome.begin() + cutPos2);
        child.Init();
    }

    void Sim::NewGeneration()
    {
        std::vector<Agent> newGeneration;
        newGeneration.resize(Config::GetMaxAgents());
        std::vector<std::pair<int, float>> survivalScore;
        survivalScore.reserve(Config::GetMaxAgents());
        
        int index = 0;
        for (const auto& agent : agents)
        {
            float deltaDist = static_cast<float>(std::sqrt(std::pow(std::abs(agent.initialPos.coordX - agent.pos.coordX), 2) +
                std::pow(std::abs(agent.initialPos.coordY - agent.pos.coordY), 2)));
            
            float score = deltaDist * Config::GetDistanceWeight() + agent.stepsTaken * Config::GetStepsTakenWeight();
            survivalScore.emplace_back(index++, score);
        }
        
        std::sort(survivalScore.begin(), survivalScore.end(),
            [](const std::pair<int, float>& p1, const std::pair<int, float>& p2)
            {
                return p1.second > p2.second;
            });
        
        int population = 0;
        const int TOP_N = static_cast<int>(std::floor(Config::GetMaxAgents() * Config::GetMateTopNPercent()));
        for (int i = 1; i < TOP_N; i++)
        {
            const auto& parent1 = agents[survivalScore[i-1].first];
            const auto& parent2 = agents[survivalScore[i].first];
            Mate(parent1, parent2, newGeneration[population]);
            population++;
        }

        while (population < Config::GetMaxAgents())
        {
            int index1 = rng.GetRNGuint16() % TOP_N;
            int index2 = rng.GetRNGuint16() % TOP_N;
            while (index1 == index2) { index2 = rng.GetRNGuint16() % TOP_N; }
            if (survivalScore[index1].second < survivalScore[index2].second)
            {
                std::swap(index1, index2);
            }
            Mate(agents[survivalScore[index1].first], agents[survivalScore[index2].first], newGeneration[population]);
            population++;
        }

        std::random_shuffle(newGeneration.begin(), newGeneration.end());

        agents = std::move(newGeneration);
    }
}