#pragma once

#include <string>

#include "Config.h"
#include "Types.h"

namespace sim
{
    // Each gene is technically a weighted edge
    class Gene
    {
    public:
        Gene() {}
        Gene(uint16_t sourceType, uint16_t sourceIdx, uint16_t sinkType, uint16_t sinkIdx, float weight) :
            sourceType(sourceType), sourceIdx(sourceIdx), sinkType(sinkType), sinkIdx(sinkIdx), weight(weight)
        {}

        inline static void MaxCapGene(Gene& gene)
        {
            gene.sourceIdx %= (gene.sourceType == NEURON ? Config::GetMaxNuerons() : Sensors::S_COUNT);
            gene.sinkIdx %= (gene.sinkType == NEURON ? Config::GetMaxNuerons() : Actions::A_COUNT);
        }

        // Everything is left aligned, offset all idx to not overlap
        inline static void OffsetGene(Gene& gene)
        {
            gene.sourceIdx += gene.sourceType == NEURON ? TYPE_COUNT : 0;
            gene.sinkIdx += gene.sinkType == NEURON ? TYPE_COUNT : Sensors::S_COUNT;
        }

        inline std::string to_string() const
        {
            return std::to_string(sourceType) + " " + std::to_string(sourceIdx) + " " +
                std::to_string(sinkType) + " " + std::to_string(sinkIdx) + " " + std::to_string(weight);
        }

    public:
        uint16_t sourceType;
        uint16_t sourceIdx;
        uint16_t sinkType;
        uint16_t sinkIdx;
        float weight;
    };
}