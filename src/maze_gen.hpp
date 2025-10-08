#include <random>
#include <unordered_set>
#include "grid.hpp"


namespace maze_gen {
    class MazeGenerator {
        std::random_device rd;
        std::mt19937 gen;

        grid::Grid& maze;
        int width;
        int height;

        std::unordered_set<grid::Coordinate> visited;

        grid::Cell& get_cell(grid::Coordinate coord);
        void carve_passage(grid::Coordinate current);

    public:
        explicit MazeGenerator(grid::Grid& grid);

        void generate_maze();
    };
}