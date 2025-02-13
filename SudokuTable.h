//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUTABLE_H
#define SUDOKU_SOLVER_SUDOKUTABLE_H

#include "Cell.h"
#include "CellGroup.h"

struct SudokuTable {
    CellGroup rows;
    CellGroup columns;
    CellGroup squares;
    Cell table[9][9];
};

#endif //SUDOKU_SOLVER_SUDOKUTABLE_H
