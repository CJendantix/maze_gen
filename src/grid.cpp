#include "grid.hpp"

using namespace grid;
void Cell::remove_wall(Direction direction) {
    using enum Direction;
    switch (direction) {
        case NORTH:
            north = false;
            break;
        case SOUTH:
            south = false;
            break;
        case EAST:
            east = false;
            break;
        case WEST:
            west = false;
            break;
    }
}

std::optional<std::reference_wrapper<Cell>> Grid::get_cell(Coordinate coord) {
    if (coord.x < 0 || coord.x >= width || coord.y < 0 || coord.y >= height)
        return std::nullopt;
    return grid[coord.x][coord.y];
}