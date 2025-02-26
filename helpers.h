//
// Created by Ema on 26.02.2025.
//

#ifndef SUDOKU_SOLVER_HELPERS_H
#define SUDOKU_SOLVER_HELPERS_H

#include <iostream>
#include "FileHandler.h"
#include "SudokuSolver.h"
#include "SudokuTable.h"
#include <bitset>
#include <iostream>
#include <thread>

int testOnDataSet(int numOfThread);
//void solve(  std::vector<std::vector<std::string>> input,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData);
void solve(  std::vector<std::vector<std::string>>::iterator start, std::vector<std::vector<std::string>>::iterator end,
             std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
             std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData);
int solveSudoku(std::string inputPath);

#endif //SUDOKU_SOLVER_HELPERS_H
