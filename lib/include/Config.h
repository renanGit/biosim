#pragma once

#include <iostream>

namespace sim
{
    class Config
    {
    public:
        inline static int GetMaxAgents() { return maxAgents; }
        inline static int GetMaxNuerons() { return maxNeurons; }

        inline static int GetMaxGenes() { return maxGenes; }
        inline static int GetMinGenes() { return minGenes; }

        inline static int GetGridSizeX() { return gridSizeX; }
        inline static int GetGridSizeY() { return gridSizeY; }

        inline static float GetActivationThreshold() { return activationThreshold; }
        
        inline static void to_string()
        {
            printf("Agents: [max %d], Neurons: [max %d], Genes: [max %d, min %d], Grid: [x %d, y %d], Activation: [threshold %0.2f]\n",
                maxAgents, maxNeurons, maxGenes, minGenes, gridSizeX, gridSizeY, activationThreshold);
        }

        static int maxAgents;
        static int maxNeurons;
        
        static int maxGenes;
        static int minGenes;
        
        static int gridSizeX;
        static int gridSizeY;
        
        static float activationThreshold;
    };
}