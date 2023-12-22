#include <map>
#include <queue>
#include <math.h>
#include <utility>
#include <unordered_set>

#include "NeuralNet.h"
#include "Gene.h"
#include "Genome.h"
#include "Grid.h"
#include "Random.h"

namespace sim
{
    void NeuralNet::BuildNNet(const Genome& genome)
    {
        weights.clear();
        sensors.clear();

        // Aggregate edges
        std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>> aggEdges;

        for (const auto& gene : genome.genome)
        {
            auto itr = aggEdges.emplace(std::pair<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>>(
                {gene.sourceIdx, gene.sinkIdx}, {gene.weight, 1}
            ));
            if (!itr.second)
            {
                itr.first->second.first += gene.weight;
                itr.first->second.second++;
            }
        }

        // Build weight graph
        for (const auto& kv : aggEdges)
        {
            const auto& key = kv.first;  // first=sourceIdx, second=sinkIdx
            const auto& val = kv.second; // first=weightSum, second=totEdges

            if (key.first < Sensors::S_COUNT)
            {
                sensors.emplace(key.first);
            }
            
            auto itr = weights.emplace(std::pair<uint16_t, std::vector<Weight>>(key.first, { {val.first / val.second, key.second} }));
            if (!itr.second)
            {
                itr.first->second.emplace_back(val.first / val.second, key.second);
            }
        }
    }

    void NeuralNet::BuildIndexMapping()
    {
        uint16_t idx = TYPE_COUNT;
        std::queue<uint16_t> q;
        
        // Map indices of Sensors/Actions to it self
        for (uint16_t i = 0; i < TYPE_COUNT; i++)
        {
            idxMap[i] = i;
        }

        for (const auto& n : sensors)
        {
            q.push(n);
        }

        while (!q.empty())
        {
            uint16_t node = q.front();
            q.pop();
            for (const auto& w : weights[node])
            {
                if (idxMap.find(w.idx) == idxMap.end())
                {
                    // Ignore if its a Sensor/Action
                    if (w.idx >= TYPE_COUNT)
                    {
                        idxMap[w.idx] = idx++;
                        q.push(w.idx);
                    }
                }
            }
        }
    }

    float NeuralNet::GetSensorValue(uint16_t sensor, uint32_t simStep, const Position& pos) const
    {
        switch(sensor)
        {
            case Sensors::AGE:
            {
                return static_cast<float>(std::tanh(simStep));
            }
            case Sensors::FORWARD_BLOCKED:
            {
                Position newPos = pos;
                newPos.MoveForward();
                return (grid->OutOfBounds(newPos) || grid->Occupied(newPos)) ? 0.0f : 1.0f;
            }
            case Sensors::OSC:
            {
                return static_cast<float>((-std::cos(simStep) + 1.0f) / 2.0f);
            }
            case Sensors::RANDOM:
            {
                return std::abs(rng.GetRNGWeight());
            }
        }
        return 1.0f;
    }

    std::array<float, Actions::A_COUNT> NeuralNet::FeedForward(uint32_t simStep, const Position& agentPos)
    {
        std::array<float, Actions::A_COUNT> actionLevels;
        actionLevels.fill(0.0f);

        // Neurons = [Sensors|Actions|Nodes] in that order
        // Remove current number of sensors to not double count, since we include all types
        std::vector<float> neurons(weights.size() + TYPE_COUNT - sensors.size(), 0.0f);

        std::queue<uint16_t> q;
        for (const auto& s : sensors)
        {
            neurons[s] = GetSensorValue(s, simStep, agentPos);
            q.push(s);
        }

        std::unordered_set<uint16_t> layerX;
        while (!q.empty())
        {
            uint16_t n = q.front();
            q.pop();
            float input = neurons[idxMap[n]];
            
            // Accumulation
            for (const auto& w : weights[n])
            {
                // Dont add idx if its an ACTION, since we are dealing with a forest graph.
                if (w.idx >= TYPE_COUNT)
                {
                    layerX.insert(w.idx);
                }
                neurons[idxMap[w.idx]] += w.w * input;
            }

            if (q.empty())
            {
                for (const auto& neuronX : layerX)
                {
                    uint16_t idx = idxMap[neuronX];
                    neurons[idx] = ActivationFunc(neurons[idx]);
                    // Push items for next layer
                    q.push(neuronX);
                }
                layerX.clear();
            }
        }

        uint16_t idx = 0;
        for (uint16_t i = Sensors::S_COUNT; i < TYPE_COUNT; i++)
        {
            actionLevels[idx++] = ActivationFunc(neurons[i], Activation::SIGMOID);
        }

        return actionLevels;
    }

    void NeuralNet::ExecuteActions(const std::array<float, Actions::A_COUNT>& actions, Position& agentPos)
    {
        Position newPos = agentPos;

        // First, perform rotation
        if (actions[Actions::ROTATE_LEFT] > Config::GetActivationThreshold())
        {
            newPos.dir = static_cast<Direction>((static_cast<int>(newPos.dir) - 1) % Direction::D_COUNT);
            agentPos.Set(newPos);
        }
        if (actions[Actions::ROTATE_RIGHT] > Config::GetActivationThreshold())
        {
            newPos.dir = static_cast<Direction>((static_cast<int>(newPos.dir) + 1) % Direction::D_COUNT);
            agentPos.Set(newPos);
        }

        // Second, perform movement
        std::pair<Actions, const int16_t*> moves[6] = {
            { Actions::MOVE_UP, MOVES[2] },
            { Actions::MOVE_DOWN, MOVES[6] },
            { Actions::MOVE_LEFT, MOVES[0] },
            { Actions::MOVE_RIGHT, MOVES[4] },
            { Actions::MOVE_FORWARD, MOVES[newPos.dir] },
            { Actions::MOVE_REVERSE, MOVES[(newPos.dir + 4) % Direction::D_COUNT] },
        };
        
        for (const auto& move : moves)
        {
            if (actions[move.first] > Config::GetActivationThreshold())
            {
                newPos.coordX += move.second[0];
                newPos.coordY += move.second[1];
            }
        }

        // Rotation is not accounted here
        if (!grid->OutOfBounds(newPos) && !grid->Occupied(newPos))
        {
            grid->Set(newPos, agentPos);
            agentPos.Set(newPos);
        }
    }
}