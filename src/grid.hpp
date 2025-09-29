#include <algorithm>
#include <array>
#include <random>

struct Coordinate {
    int x;
    int y;
};

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

Direction opposite(Direction direction) {
    using enum Direction;
    switch (direction) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
    }
}

Coordinate to_coord(Direction direction) {
    using enum Direction;
    switch (direction) {
        case NORTH: return {0, -1};
        case SOUTH: return {0, 1};
        case EAST:  return {1, 0};
        case WEST:  return {-1, 0};
    }
}

struct Cell {
    bool north = true;
    bool east = true;
    bool south = true;
    bool west = true;
    bool visited = false;

    void remove_wall(Direction direction) {
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
};

template <int width, int height>
class Grid {
private:
    std::array<std::array<Cell, height>, width> grid = {};

    std::random_device rd;
    std::mt19937 gen;

    void carve_passage(int cx, int cy) {
        using enum Direction;
    
        grid[cx][cy].visited = true;
    
        std::array<Direction, 4> directions = {NORTH, SOUTH, EAST, WEST};
        std::ranges::shuffle(directions, gen);
    
        for (Direction direction : directions) {
            Coordinate offset = to_coord(direction);
            int nx = cx + offset.x;
            int ny = cy + offset.y;
    
            if (0 <= nx && nx < width && 0 <= ny && ny < height && !grid[nx][ny].visited) {
                grid[cx][cy].remove_wall(direction);
                grid[nx][ny].remove_wall(opposite(direction));
                carve_passage(nx, ny);
            }
        }
    }

    void generate_maze() {
        using enum Direction;
        carve_passage(0, 0);

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
                    grid[coord][0].remove_wall(NORTH);
                    break;
                case SOUTH:
                    grid[coord][height - 1].remove_wall(SOUTH);
                    break;
                case EAST:
                    grid[width - 1][coord].remove_wall(EAST); // Fix: Use the last column for EAST
                    break;
                case WEST:
                    grid[0][coord].remove_wall(WEST); // Fix: Use the first column for WEST
                    break;
            }
        }
    }

public:
    Grid() : gen(rd()) {
        generate_maze();
    }

    std::optional<std::reference_wrapper<Cell>> get_cell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return std::nullopt;
        }
        return grid[x][y];
    }
};