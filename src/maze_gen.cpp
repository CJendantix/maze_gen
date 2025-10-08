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

void MazeGenerator::generate_maze() {
    using enum Direction;
    carve_passage({0, 0});

    std::array<Direction, 4> walls = {NORTH, SOUTH, EAST, WEST};
    std::ranges::shuffle(walls, gen);

    for (int i = 0; i < 2; i++) {
        auto wall = walls[i];
        
        int length;

        if (wall == NORTH || wall == SOUTH) {
            length = width;
        } else {
            length = height;
        }

        std::uniform_int_distribution length_dist(0, length - 1);
        int coord = length_dist(gen);

        switch (wall) {
            case NORTH:
                get_cell({coord, 0}).remove_wall(NORTH);
                break;
            case SOUTH:
                get_cell({coord, height - 1}).remove_wall(SOUTH);
                break;
            case EAST:
                get_cell({width - 1, coord}).remove_wall(EAST);
                break;
            case WEST:
                get_cell({0, coord}).remove_wall(WEST);
                break;
        }
    }
}