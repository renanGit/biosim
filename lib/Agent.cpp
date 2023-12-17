#include <unordered_map>
#include <vector>

#include "Agent.h"
#include "Gene.h"

namespace sim
{
    void Agent::Init()
    {
        if (genome.genome.size() == 0)
        {
            genome.GenerateGenome();
        }
        genomeSizeBefore = static_cast<int>(genome.genome.size());
        WireGenome();
        genomeSizeAfter = static_cast<int>(genome.genome.size());
    }

    void Agent::SimAgent(uint32_t simStep)
    {
        Position start = pos;
        const auto& actions = nnet.FeedForward(simStep, pos);
        nnet.ExecuteActions(actions, pos);
        if (start != pos)
        {
            stepsTaken++;
        }
    }

    void Agent::WireGenome()
    {
        std::unordered_map<uint16_t, std::vector<uint16_t>> backEdgeMap;

        for (auto& gene : genome.genome)
        {
            Gene::MaxCapGene(gene);
            Gene::OffsetGene(gene);

            auto itr = backEdgeMap.emplace(std::pair<uint16_t, std::vector<uint16_t>>(gene.sinkIdx, { { gene.sourceIdx } }));
            if (!itr.second)
            {
                itr.first->second.push_back(gene.sourceIdx);
            }
        }

        std::unordered_set<uint16_t> visited;
        std::set<std::pair<uint16_t, uint16_t>> loopEdges;
        genome.DiscoverValidGenes(visited, loopEdges, backEdgeMap);
        genome.TrimGenome(visited, loopEdges);

        nnet.BuildNNet(genome);
        nnet.BuildIndexMapping();
    }
}