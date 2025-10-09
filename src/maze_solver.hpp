#pragma once

#include <random>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <optional>
#include "grid.hpp"

namespace maze_solver {
    namespace {
        template<typename type, typename comp>
        class QueueSet {
            std::priority_queue<type, std::vector<type>, comp> queue;
            std::unordered_set<type> set;
        public:
            QueueSet(comp comparator) : queue(comparator) {}
            inline int size() {
                return queue.size();
            }

            inline void push(type obj) {
                queue.push(obj);
                set.insert(obj);
            }

            inline type pop() {
                type ret = queue.top();
                queue.pop();
                set.erase(ret);
                return ret;
            }

            inline bool contains(type obj) {
                return set.contains(obj);
            }
        };
    }

    struct Cost {
        int cost;
        int heuristic_cost;
        int total_cost;
        std::optional<grid::Coordinate> parent;
    };

    class MazeSolver {
        std::random_device rd;
        std::mt19937 gen;

        grid::Grid& maze;
        int width;
        int height;
        grid::Coordinate start;
        grid::Coordinate end;

        grid::Cell& get_cell(grid::Coordinate coord);

        int heuristic(grid::Coordinate a, grid::Coordinate b);

        std::unordered_map<grid::Coordinate, Cost> data;

        std::function<bool(grid::Coordinate, grid::Coordinate)> comp;
        QueueSet<grid::Coordinate, decltype(comp)> open_set;
        std::unordered_set<grid::Coordinate> closed_set;

        std::unordered_set<grid::Coordinate> reconstruct_path(grid::Coordinate coord);

    public:
        explicit MazeSolver(grid::Grid& grid, grid::Coordinate start, grid::Coordinate end);

        std::unordered_set<grid::Coordinate> solve_maze();
    };
}