
#include <functional>

#include "Genome.h"
#include "Gene.h"
#include "Random.h"

namespace sim
{
    void Genome::GenerateGenome()
    {
        genome.clear();
        genome.resize(rng.GetGenomeSize());
        for (auto& gene : genome)
        {
            gene.sourceType = rng.GetRNGuint16() & 1; 
            gene.sourceIdx = rng.GetRNGuint16();
            gene.sinkType = rng.GetRNGuint16() & 1;
            gene.sinkIdx = rng.GetRNGuint16();
            gene.weight = rng.GetRNGWeight();
        }
    }

    void Genome::DiscoverValidGenes(std::unordered_set<uint16_t>& validNodes, std::set<std::pair<uint16_t, uint16_t>>& loopEdges,
        const std::unordered_map<uint16_t, std::unordered_set<uint16_t>>& backEdgeMap)
    {
        std::unordered_set<uint16_t> invalidNodes;
        std::function<bool(uint16_t)> dfs;
        
        dfs = [&](uint16_t node)
        {
            if (node >= TYPE_COUNT && backEdgeMap.find(node) == backEdgeMap.end())
            {
                // This a terminal NEURON (has no neighbors), invalid path.
                return false;
            }
            else if (node < Sensors::S_COUNT)
            {
                // Successfully reached a sensor terminal
                return true;
            }

            bool pathValid = false;
            for (const auto& sourceNode : backEdgeMap.at(node))
            {
                if (validNodes.find(sourceNode) != validNodes.end())
                {
                    // Node already reached a sensor, no need to traverse path
                    pathValid = true;
                    continue;
                }
                else if (invalidNodes.find(sourceNode) == invalidNodes.end())
                {
                    // Only visit if not already seen.
                    validNodes.insert(sourceNode);
                    bool tmp = dfs(sourceNode);
                    if (!tmp)
                    {
                        validNodes.erase(sourceNode);
                        invalidNodes.insert(sourceNode);
                    }
                    pathValid = tmp || pathValid;
                }
            }
            return pathValid;
        };

        for (const auto& gene : genome)
        {
            if (gene.sinkType == ACTION &&
                validNodes.find(gene.sinkIdx) == validNodes.end() &&
                invalidNodes.find(gene.sinkIdx) == invalidNodes.end())
            {
                validNodes.insert(gene.sinkIdx);
                if (!dfs(gene.sinkIdx))
                {
                    validNodes.erase(gene.sinkIdx);
                    invalidNodes.insert(gene.sinkIdx);
                }
            }
        }

        std::unordered_set<uint16_t> loopPath;
        std::function<bool(uint16_t)> dfsLoop;

        dfsLoop = [&](uint16_t node)
        {
            if (loopPath.find(node) != loopPath.end())
            {
                return false;
            }
            else if (node < Sensors::S_COUNT)
            {
                return true;
            }
            
            loopPath.insert(node);
            bool validPath = false;
            for (const auto& sourceNode : backEdgeMap.at(node))
            {
                if (validNodes.find(sourceNode) != validNodes.end())
                {
                    bool tmp = dfsLoop(sourceNode);
                    if (!tmp)
                    {
                        loopEdges.emplace(std::pair<uint16_t,uint16_t>(sourceNode, node));
                    }
                    validPath = tmp || validPath;
                }
            }
            loopPath.erase(node);

            return validPath;
        };

        for (const auto& node : validNodes)
        {
            if (node >= Sensors::S_COUNT && node < TYPE_COUNT)
            {
                dfsLoop(node);
            }
        }
    }

    void Genome::TrimGenome(const std::unordered_set<uint16_t>& valid, std::set<std::pair<uint16_t, uint16_t>>& loopEdges)
    {
        std::vector<Gene> correctedGenome;
        // valid contains P & Q, so divide by 2 to get true size of number of edges (P -> Q)
        correctedGenome.reserve(valid.size() / 2);
        for (auto& gene : genome)
        {
            // Ignore loop
            if (loopEdges.find(std::pair<uint16_t, uint16_t>(gene.sourceIdx, gene.sinkIdx)) != loopEdges.end())
            {
                continue;
            }

            // Edge must exist in the valid
            if (valid.find(gene.sourceIdx) != valid.end() && valid.find(gene.sinkIdx) != valid.end())
            {
                correctedGenome.push_back(std::move(gene));
            }
        }

        genome.clear();
        genome = std::move(correctedGenome);
    }

}