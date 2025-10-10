#include <vector>
#include <optional>

#ifndef GRID_HPP
#define GRID_HPP

namespace grid {
    struct Coordinate {
        int x;
        int y;

        bool operator==(const Coordinate& other) const = default;
    };

    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    constexpr Direction opposite(Direction direction) {
        using enum Direction;
        switch (direction) {
            case NORTH: return SOUTH;
            case SOUTH: return NORTH;
            case EAST:  return WEST;
            case WEST:  return EAST;
        }
    }

    constexpr Coordinate to_coord(Direction direction) {
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

        void remove_wall(Direction direction);
    };

    class Grid {
    private:
        int width;
        int height;

        std::vector<std::vector<Cell>> grid;

    public:
        Grid(int width, int height) 
            : width(width)
            , height(height)
            , grid(width, std::vector<Cell>(height)) {}
        int get_width() const { return width; }
        int get_height() const { return height; }

        std::optional<std::reference_wrapper<Cell>> get_cell(Coordinate coord);
    };
}

namespace std {
    template <>
    struct hash<grid::Coordinate> {
        size_t operator()(const grid::Coordinate& s) const {
            return std::hash<int>()(s.x) ^ (std::hash<int>()(s.y) << 1);
        }
    };
}

#endif // GRID_HPP