#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <math.h>

#include "Types.h"

namespace sim
{
    class Genome;
    class Grid;
    class Position;

    struct Weight
    {
        float w;
        uint16_t idx;
    };

    class NeuralNet
    {
    public:
        NeuralNet() : grid(nullptr) {}
        void BuildIndexMapping();
        void BuildNNet(const Genome& genome);
        std::array<float, Actions::A_COUNT> FeedForward(uint32_t simStep, const Position& agentPos);
        void ExecuteActions(const std::array<float, Actions::A_COUNT>& actions, Position& agentPos);
    private:
        // All sensor values will range [0.0, 1.0]
        float GetSensorValue(uint16_t sensor, uint32_t simStep, const Position& pos) const;
        inline float ActivationFunc(const float& input, Activation activation=Activation::TAN_H) const
        {
            switch(activation)
            {
            case Activation::SIGMOID:
                return static_cast<float>(1 / (1 + std::exp(-input)));
            default:
                return static_cast<float>(std::tanh(input));
            }
        }
    public:
        std::unordered_set<uint16_t> sensors;
        std::unordered_map<uint16_t, std::vector<Weight>> weights;
        std::unordered_map<uint16_t, uint16_t> idxMap;
        Grid* grid;
    };
}