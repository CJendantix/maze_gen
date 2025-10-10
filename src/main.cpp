#include <iostream>
#include <optional>
#include "maze_gen/maze_gen.hpp"
#include "maze_solver/maze_solver.hpp"

void output(grid::Grid& maze, std::optional<const std::reference_wrapper<std::unordered_set<grid::Coordinate>>> solution) {
    std::cout << " ";
    for (int x = 0; x < maze.get_width(); x++) {
        const auto& cell = maze.get_cell({x, 0}).value().get();
        std::cout << (cell.north ? "_ " : "  ");
    }
    std::cout << std::endl;

    for (int y = 0; y < maze.get_width(); y++) {
        std::cout << (maze.get_cell({0, y}).value().get().west ? '|' : ' ');
        for (int x = 0; x < maze.get_height(); x++) {
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

int get_valid_size(std::string_view prompt) {
    int value;
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            std::exit(1); // EOF or input error
        }
        try {
            size_t pos;
            value = std::stoi(line, &pos);
            if (pos == line.size() && value >= 0 && value <= 200)
                return value;
        } catch (std::invalid_argument&) {
            std::cout << "Invalid input.\n";
        } catch (std::out_of_range&) {
            std::cout << "Too large.\n";
        }
        std::cout << "Please enter a valid integer between 0 and 200.\n";
    }
}

int main() {
    int width = get_valid_size("Enter maze width (max 200): ");
    int height = get_valid_size("Enter maze height (max 200): ");

    grid::Grid grid(width, height);
    maze_gen::MazeGenerator gen(grid);
    
    auto points = gen.generate_maze();
    maze_solver::MazeSolver solver(grid, points[0], points[1]);
    auto solution = solver.solve_maze();

    output(grid, std::nullopt);
    output(grid, solution);

    return 0;
}
