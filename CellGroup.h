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

#endif //SUDOKU_SOLVER_CELLGROUP_H
