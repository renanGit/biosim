#include "Config.h"
#include "Random.h"

namespace sim
{
    // Defaults

    int Config::maxAgents = 1000;
    int Config::maxNeurons = 128;
        
    int Config::maxGenes = 128;
    int Config::minGenes = 128;
        
    int Config::gridSizeX = 128;
    int Config::gridSizeY = 128;
        
    float Config::activationThreshold = 0.85f;

    // Mating configs

    float Config::mateTopNPercent = 0.3f;
    float Config::minDominantCutPercent = 0.6f;
    float Config::distanceWeight = 0.8f;
    float Config::stepsTakenWeight = 0.2f;

    RNG rng;
    #pragma omp threadprivate(rng)
}
