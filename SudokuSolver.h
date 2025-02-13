//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUSOLVER_H
#define SUDOKU_SOLVER_SUDOKUSOLVER_H

#include <iostream>
#include <array>
#include <cstdint>
#include "SudokuTable.h"

class SudokuSolver {
    SudokuTable sudokuTable;
public:
    void parseStringToMatrix(const std::string& input);

    void printTable();
    void printTableWithPossibilities();

    void markPossibilities();
    void markPossibleNumbersColumns();
    void markPossibleNumbersRows();
    void markPossibleNumbersSquares();
};


#endif //SUDOKU_SOLVER_SUDOKUSOLVER_H
