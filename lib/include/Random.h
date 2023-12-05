#pragma once

#include <random>

#include "Config.h"

namespace sim
{
    class RNG
    {
    public:
        RNG(uint32_t seed=0)
        {
            if (seed != 0)
            {
                gen.seed(seed);
            }
            else
            {
                gen.seed(rd());
            }
            distrib.param(std::uniform_int_distribution<>::param_type(0, INT_MAX-1));
        }

        inline uint16_t GetRNGuint16()
        {
            return static_cast<uint16_t>(distrib(gen) % UINT16_MAX);
        }

        inline uint16_t GetGenomeSize()
        {
            return std::max<uint16_t>(Config::GetMinGenes(), (GetRNGuint16() + 1) % Config::GetMaxGenes());
        }

        inline float GetRNGWeight()
        {
            // Weight range of [-1, 1]
            return -1.0f + static_cast<float>(distrib(gen)) / RNG_W;
        }

    private:
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen; // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib;

        const float RNG_W = static_cast<float>(INT_MAX / 2.0f);
    };

    extern RNG rng;
    #pragma omp threadprivate(rng)
}