#include <cassert>
#include <cmath>
#include "maze_solver.hpp"

using namespace maze_solver;
using namespace grid;

MazeSolver::MazeSolver(Grid& grid, Coordinate start, Coordinate end)
    : gen(rd())
    , maze(grid)
    , width(grid.get_width())
    , height(grid.get_height())
    , start(start)
    , end(end)
    , comp([this](grid::Coordinate a, grid::Coordinate b) {
        return data.at(a).total_cost > data.at(b).total_cost;
    })
    , open_set(comp) {}

int MazeSolver::heuristic(grid::Coordinate a, grid::Coordinate b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

Cell &MazeSolver::get_cell(Coordinate coord)
{
    auto cell_opt = maze.get_cell(coord);
    assert(cell_opt.has_value());
    return cell_opt.value().get();
}

std::unordered_set<grid::Coordinate> MazeSolver::reconstruct_path(grid::Coordinate coord) {
    std::optional<grid::Coordinate> current = coord;
    std::unordered_set<grid::Coordinate> set;

    while (current != std::nullopt) {
        set.insert(current.value());
        current = data.at(current.value()).parent;
    }

    return set;
}

std::unordered_set<grid::Coordinate> MazeSolver::solve_maze()
{
    open_set.push(start);
    data.insert_or_assign(start, Cost{0, 0, 0, std::nullopt});

    while (open_set.size() > 0) {
        auto current = open_set.pop();

        if (current == end) {
            return reconstruct_path(current);
        }

        closed_set.insert(current);

        std::vector<grid::Coordinate> neighbors;
        auto& cell = get_cell(current);

        std::array<std::pair<grid::Coordinate, bool>, 4> directions = {{
            {{current.x, current.y - 1}, cell.north},
            {{current.x, current.y + 1}, cell.south},
            {{current.x + 1, current.y}, cell.east},
            {{current.x - 1, current.y}, cell.west}
        }};
        
        for (auto& [coord, blocked] : directions) {
            if (!blocked && maze.get_cell(coord).has_value()) {
                neighbors.push_back(coord);
            }
        }

        for (auto neighbor : neighbors) {
            if (closed_set.contains(neighbor)) {
                continue;
            }

            // Ensure the neighbor exists in the data map
            if (!data.contains(neighbor)) {
                data.insert_or_assign(neighbor, Cost{INT_MAX, 0, INT_MAX, std::nullopt});
            }

            int tentative_g = data.at(current).cost + 1;

            if (!open_set.contains(neighbor)) {
                open_set.push(neighbor);
            } else if (tentative_g >= data.at(neighbor).cost) {
                continue;
            }

            int h = heuristic(neighbor, end);
            data.insert_or_assign(neighbor, Cost{tentative_g, h, tentative_g + h, current});
        }
    }
    return std::unordered_set<grid::Coordinate>();
}