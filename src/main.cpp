#include <iostream>
#include "grid.hpp"

const int WIDTH = 12;
const int HEIGHT = 12;

int main() {
    Grid<WIDTH, HEIGHT> grid;

    std::cout << " ";
    for (int x = 0; x < WIDTH; x++) {
        const auto& cell = grid.get_cell(x, 0).value().get();
        std::cout << (cell.north ? "_ " : "  ");
    }
    std::cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        std::cout << "|"; // Left border
        for (int x = 0; x < WIDTH; x++) {
            const auto& cell = grid.get_cell(x, y).value().get();
    
            char floor = cell.south ? '_' : ' ';
            char wall = cell.east ? '|' : ' ';
    
            std::cout << floor << wall;
        }
        std::cout << "\n";
    }

    return 0;
}
