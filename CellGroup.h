//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_CELLGROUP_H
#define SUDOKU_SOLVER_CELLGROUP_H

#include "Cell.h"
#include <cstdint>

struct CellGroup {
    bool validGroup = false;
    Cell* cells[9] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
};

// struct CellGroup {
//     bool validGroup = false;
//     std::pair<int8_t, int8_t> cells[9];
// };

#endif //SUDOKU_SOLVER_CELLGROUP_H
