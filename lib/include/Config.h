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

        inline static float GetMateTopNPercent() { return mateTopNPercent; }
        inline static float GetMinDominantCutPercent() { return minDominantCutPercent; }
        inline static float GetDistanceWeight() { return distanceWeight; }
        inline static float GetStepsTakenWeight() { return stepsTakenWeight; }
        
        inline static void to_string()
        {
            printf("Agents: [max %d], Neurons: [max %d], Genes: [max %d, min %d], Grid: [x %d, y %d], \n \
                Activation: [threshold %0.2f], MateTopN: [%0.2f], MinDominantCutPercent: [%0.2f], \n \
                DistanceWeight: [%0.2f], StepsTakenWeight: [%0.2f]\n",
                maxAgents, maxNeurons, maxGenes, minGenes, gridSizeX, gridSizeY, activationThreshold,
                mateTopNPercent, minDominantCutPercent, distanceWeight, stepsTakenWeight);
        }

        static int maxAgents;
        static int maxNeurons;
        
        static int maxGenes;
        static int minGenes;
        
        static int gridSizeX;
        static int gridSizeY;
        
        static float activationThreshold;   // Action activation

        static float mateTopNPercent;       // After survival & sorting, the top N percent will reproduce.
                                            // Note: All agents in TopN% will have a chance to mate, after that random TopN parents are selected.
        static float minDominantCutPercent; // Dominant parent min cut percent, between (0.0, 1.0]. Ie. use more genes from dominant parent.
                                            // Dominant means agent X out performed agent Y, Ie. survival score was greater
        /*
         * Weights are used to bring more influence on fields. The higher it is, the more influence.
         * More influence, means if the agent performed more of the type the more likely it is to survive and mate.
         * Weights should sum to 1.0
         */
        static float distanceWeight;
        static float stepsTakenWeight;
    };
}