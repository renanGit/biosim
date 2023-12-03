#include "Config.h"
#include "Types.h"
#include "Random.h"

namespace sim
{
    const int MAX_AGENTS = 1000;
    const int MAX_NEURONS = 128;

    // Sizes are not guaranteed, because the graph will trim invalid genes
    const int MAX_GENES = 128;
    const int MIN_GENES = 128;

    const float ACTIVATION_THRESHOLD = 0.6f;

    const int GRID_SIZE_X = 128;
    const int GRID_SIZE_Y = 128;

    RNG rng;
    #pragma omp threadprivate(rng)
}
