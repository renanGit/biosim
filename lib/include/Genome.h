#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cstdint>

#include "Gene.h"

namespace sim
{
    struct pair_hash
    {
        size_t operator() (const std::pair<uint16_t, uint16_t>& pair) const
        {
            return std::hash<uint16_t>()(pair.first) ^ std::hash<uint16_t>()(pair.second);
        }
    };

    class Genome
    {
    public:
        void GenerateGenome();
        void WireGenome();
        /*
        *   Traverse edges backward from ACTION.
        *    Any unvisited nodes means unreachable.
        *    Any ACTION path that does not terminate on a SENSOR is invalid.
        */
        void DiscoverValidGenes(std::unordered_set<uint16_t>& valid, 
            std::set<std::pair<uint16_t, uint16_t>>& loopEdges,
            std::unordered_map<uint16_t, std::vector<uint16_t>>& backEdgeMap);
        void TrimGenome(std::unordered_set<uint16_t>& valid,
            std::set<std::pair<uint16_t, uint16_t>>& loopEdges);

    public:
        std::vector<Gene> genome;
    };

}