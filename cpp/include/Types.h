#pragma once

#include <cstdint>

namespace sim
{
    const int NEURON = 0; // exists on both source and sink
    const int SENSOR = 1; // always represented in source
    const int ACTION = 1; // always represented in sink

    enum Sensors
    {
        AGE=0,
        OSC, // oscillator
        RANDOM,
        FORWARD_BLOCKED,
        S_COUNT
    };

    enum Actions
    {
        MOVE_LEFT=0,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_FORWARD,
        ROTATE_LEFT,
        ROTATE_RIGHT,
        MOVE_REVERSE,
        A_COUNT
    };

    /*
    *    1 2 3
    *    0 * 4
    *    7 6 5
    */
    enum Direction
    {
        W=0,
        NW,
        N,
        NE,
        E,
        SE,
        S,
        SW,
        D_COUNT
    };

    const int16_t MOVES[Direction::D_COUNT][2] = {
        {0, -1},
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1}
    };

    enum Activation
    {
        TAN_H=0,
        SIGMOID
    };

    const uint16_t TYPE_COUNT = Sensors::S_COUNT + Actions::A_COUNT;
}