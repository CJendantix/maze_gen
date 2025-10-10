#include <algorithm>
#include <cassert>
#include "maze_gen.hpp"

using namespace maze_gen;
using namespace grid;

MazeGenerator::MazeGenerator(Grid& grid)
    : gen(rd())
    , maze(grid)
    , width(grid.get_width())
    , height(grid.get_height()) {}

Cell& MazeGenerator::get_cell(Coordinate coord) {
    auto cell_opt = maze.get_cell(coord);
    assert(cell_opt.has_value());
    return cell_opt.value().get();
}

void MazeGenerator::carve_passage(Coordinate current) {
    using enum Direction;

    visited.insert(current);

    std::array<Direction, 4> directions = {NORTH, SOUTH, EAST, WEST};
    std::ranges::shuffle(directions, gen);

    for (Direction direction : directions) {
        Coordinate offset = to_coord(direction);
        Coordinate next = {current.x + offset.x, current.y + offset.y};

        if (0 <= next.x && next.x < width && 0 <= next.y && next.y < height && !visited.contains(next)) {
            get_cell(current).remove_wall(direction);
            get_cell(next).remove_wall(opposite(direction));
            carve_passage(next);
        }
    }
}

std::array<Coordinate, 2> MazeGenerator::generate_maze() {
    using enum Direction;
    carve_passage({0, 0});

    std::array<Direction, 4> walls = {NORTH, SOUTH, EAST, WEST};
    std::ranges::shuffle(walls, gen);

    std::array<Coordinate, 2> ret;

    for (int i = 0; i < 2; i++) {
        auto wall = walls[i];
        
        int length;

        if (wall == NORTH || wall == SOUTH) {
            length = width;
        } else {
            length = height;
        }

        std::uniform_int_distribution length_dist(0, length - 1);
        int distance = length_dist(gen);

        Coordinate coord;
        switch (wall) {
            case NORTH:
                coord = {distance, 0};
                break;
            case SOUTH:
                coord = {distance, height - 1};
                break;
            case EAST:
                coord = {width - 1, distance};
                break;
            case WEST:
                coord = {0, distance};
                break;
        }

        get_cell(coord).remove_wall(wall);
        ret[i] = coord;
    }

    return ret;
}