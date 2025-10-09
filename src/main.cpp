#include <iostream>
#include <optional>
#include "maze_gen.hpp"
#include "maze_solver.hpp"

constexpr int WIDTH = 12;
constexpr int HEIGHT = 12;

void output(grid::Grid& maze, std::optional<const std::reference_wrapper<std::unordered_set<grid::Coordinate>>> solution) {
    std::cout << " ";
    for (int x = 0; x < WIDTH; x++) {
        const auto& cell = maze.get_cell({x, 0}).value().get();
        std::cout << (cell.north ? "_ " : "  ");
    }
    std::cout << std::endl;

    for (int y = 0; y < HEIGHT; y++) {
        std::cout << (maze.get_cell({0, y}).value().get().west ? '|' : ' ');
        for (int x = 0; x < WIDTH; x++) {
            const auto& cell = maze.get_cell({x, y}).value().get();

            char floor = cell.south ? '_' : ' ';
            char wall = cell.east ? '|' : ' ';
            if (solution.has_value() && solution.value().get().contains({x, y})) {
                std::cout << "\033[7m" << floor << wall << "\033[0m";
            } else {
                std::cout << floor << wall;
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    grid::Grid grid(WIDTH, HEIGHT);
    maze_gen::MazeGenerator gen(grid);
    
    auto points = gen.generate_maze();
    maze_solver::MazeSolver solver(grid, points[0], points[1]);
    auto solution = solver.solve_maze();

    output(grid, std::nullopt);
    output(grid, solution);

    return 0;
}
