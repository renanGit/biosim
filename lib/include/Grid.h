#pragma once

#include <vector>

#include "Types.h"

namespace sim
{
    class Position
    {
    public:
        Position() {}
        Position(uint16_t x, uint16_t y, Direction dir) : coordX(x), coordY(y), dir(dir) {}
        inline void MoveForward()
        {
            const auto& m = MOVES[dir];
            coordX += m[0];
            coordY += m[1];
        }
        uint16_t coordX;
        uint16_t coordY;
        Direction dir;

        bool operator==(const Position& rhs) { return coordX == rhs.coordX && coordY == rhs.coordY; }
    };

    class Grid
    {
    public:
        Grid(uint16_t sizeX, uint16_t sizeY) : sizeX(sizeX), sizeY(sizeY) {}
        inline void Init()
        {
            grid.clear();
            grid.resize(sizeX, std::vector<bool>(sizeY, false));
        }

        inline bool Occupied(Position& pos) const
        {
            return grid[pos.coordX][pos.coordY];
        }

        inline bool OutOfBounds(Position& pos) const
        {
            // Since the grid has unsigned type, we only check greater than sizeXY
            return pos.coordX >= sizeX || pos.coordY >= sizeY;
        }

        inline void Set(uint16_t coordX, uint16_t coordY, bool val)
        {
            grid[coordX][coordY] = val;
        }

        inline void Set(const Position& pNew, const Position& qOld)
        {
            grid[qOld.coordX][qOld.coordY] = false;
            grid[pNew.coordX][pNew.coordY] = true;
        }

        inline uint16_t SizeX() const { return sizeX; }
        inline uint16_t SizeY() const { return sizeY; }

    private:
        uint16_t sizeX;
        uint16_t sizeY;
        std::vector<std::vector<bool>> grid;
    };
}