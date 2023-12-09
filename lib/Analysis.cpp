#include <queue>
#include <map>
#include <unordered_set>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

#include "Analysis.h"
#include "Agent.h"
#include "NeuralNet.h"
#include "Genome.h"
#include "Types.h"
#include "Config.h"
#include "Sim.h"

namespace sim
{
    void UpdateGraphMetrics(std::vector<long>& graph, Agent& ag)
    {
        int idx = 0;
        for (const auto& a : Analysis::GraphAnalysis(ag.nnet))
        {
            graph[idx++]+= a;
        }
    }

    void UpdateGenomeMetrics(std::vector<long>& nodes, Agent& ag)
    {
        int idx = 0;
        for (const auto& a : Analysis::GenomeAnalysis(ag.genome))
        {
            nodes[idx++]+= a;
        }
    }

    void UpdateEdgeMetrics(std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>>& edges, Agent& ag)
    {
        for (const auto& kv : Analysis::WeightAnalysis(ag.genome))
        {
            auto itr = edges.emplace(std::pair<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>>({kv.first.first, kv.first.second}, {kv.second.first, kv.second.second}));
            if (!itr.second)
            {
                itr.first->second.first += kv.second.first;
                itr.first->second.second += kv.second.second;
            }
        }
    }

    void Analysis::SingleAgentRun()
    {
        std::unordered_map<int, std::pair<int, std::chrono::nanoseconds>> measure;
        long before = 0;
        long after = 0;
        std::vector<long> graph(139, 0);
        std::vector<long> nodes(139, 0);
        std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>> aggEdges;
        std::vector<long> actionCnt(Actions::A_COUNT, 0);
        
        Grid grid(128, 128);
        grid.Init();

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100000; i++)
        {
            Agent ag;
            auto s = std::chrono::high_resolution_clock::now();
            ag.Init();
            ag.nnet.grid = &grid;
            ag.initialPos = {0, 0, Direction::W};
            ag.pos = {0, 0, Direction::W};

            before += ag.genomeSizeBefore;
            after += ag.genomeSizeAfter;
            ag.SimAgent(0);
            auto t = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - s);

            UpdateGraphMetrics(graph, ag);
            UpdateGenomeMetrics(nodes, ag);
            UpdateEdgeMetrics(aggEdges, ag);

            auto itr = measure.find(ag.genome.genome.size());
            if (itr == measure.end())
            {
                measure.emplace(std::pair<int, std::pair<int, std::chrono::nanoseconds>>(ag.genome.genome.size(), { 1, t }));
            }
            else
            {
                itr->second.first++;
                itr->second.second = (itr->second.second + t);
            }
        }
        
        printf("TIME: %f\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0f);
        printf("B: %.4f A: %.4f\n", before / 100000.0f, after / 100000.0f);
        int idx = 0;
        for (const auto& g : graph)
        {
            printf("%d: %lu ", idx++, g);
        }
        std::cout << std::endl;
        idx = 0;
        for (const auto& n : nodes)
        {
            printf("%d: %lu ", idx++, n);
        }
        std::cout << std::endl;
        for (const auto& kv : aggEdges)
        {
            if (kv.first.second < TYPE_COUNT)
            {
                printf("[%d, %d] %.4f %d\n", kv.first.first, kv.first.second, kv.second.first / kv.second.second, kv.second.second);
                actionCnt[kv.first.second - Sensors::S_COUNT] += kv.second.second;
            }
        }
        for (const auto& a : actionCnt)
        {
            printf("%lu ", a);
        }
        std::cout << std::endl;

        std::vector<std::pair<int, std::pair<int, std::chrono::nanoseconds>>> tmp(measure.begin(), measure.end());
        std::sort(tmp.begin(), tmp.end(), [](const auto& l, const auto& r) { return l.first > r.first; });
        for (const auto& m : tmp)
        {
            printf("G: %d O: %d A: %.4f\n", m.first, m.second.first, (m.second.second.count() / (float)m.second.first) / 1000000.f);
        }
    }

    void Analysis::SimRun()
    {
        Sim sim;
        sim.Init();
    }

    std::vector<long> Analysis::GraphAnalysis(NeuralNet& nnet)
    {
        std::vector<long> nodes(Config::GetMaxGenes() + TYPE_COUNT, 0);
        std::queue<uint16_t> q;
        std::unordered_set<uint16_t> visited;

        for (const auto& s : nnet.sensors)
        {
            q.push(s);
            nodes[s]++;

            while (!q.empty())
            {
                uint16_t n = q.front();
                q.pop();

                for (const auto& w : nnet.weights[n])
                {
                    if (visited.find(w.idx) == visited.end())
                    {
                        nodes[w.idx]++;
                        visited.insert(w.idx);
                        if (w.idx >= TYPE_COUNT)
                        {
                            q.push(w.idx);
                        }
                    }
                }
            }
        }
        return nodes;
    }

    std::vector<long> Analysis::GenomeAnalysis(Genome& genome)
    {
        std::vector<long> nodes(Config::GetMaxGenes() + TYPE_COUNT, 0);

        for (const auto& gene : genome.genome)
        {
            if (gene.sourceType == SENSOR)
            {
                nodes[gene.sourceIdx]++;
            }
            nodes[gene.sinkIdx]++;
        }
        return nodes;
    }

    std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>> Analysis::WeightAnalysis(Genome& genome)
    {
        std::map<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>> edges;

        for (const auto& gene : genome.genome)
        {
            auto itr = edges.emplace(std::pair<std::pair<uint16_t, uint16_t>, std::pair<float, uint16_t>>({gene.sourceIdx, gene.sinkIdx}, {gene.weight, 1}));
            if (!itr.second)
            {
                itr.first->second.first += gene.weight;
                itr.first->second.second++;
            }
        }

        return edges;
    }
}