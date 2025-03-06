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
#include <atomic>
#include <chrono>
#include <iomanip>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

const int segmentation = 100;

int solveSudoku(const std::string& inputPath);

int testOnDataSet(int numOfThreads);

void solve(  std::vector<std::vector<std::string>>::iterator start, std::vector<std::vector<std::string>>::iterator end,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& errorData,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& backtrackErrorData,
             std::vector<std::string>& solveHistory, std::atomic<int>& progressBar);

void displayProgress(const std::vector<std::atomic<int>>& progressBar, int numThreads);

std::string ConvertMillisecondsToTimeFormat(long long int milliseconds);

//void solve(  std::vector<std::vector<std::string>> input,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData);
#endif //SUDOKU_SOLVER_HELPERS_H
