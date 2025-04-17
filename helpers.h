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
//TODO Dynamic segments
void testInputs();

int solveSudoku(const std::string& inputPath);

int testOnDataSet(int numOfThreads, bool saveHistory = false);
int testOnDataSetV2(const std::string& filename, int numOfThreads, bool saveHistory = false);

void solve(  std::vector<std::vector<std::string>>::iterator start, std::vector<std::vector<std::string>>::iterator end,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& errorData,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& backtrackErrorData,
             std::vector<std::string>& solveHistory, std::atomic<int>& progressBar);

void solveV2(const std::shared_ptr<std::vector<std::vector<std::string>>>&  input, bool& inputFinished, bool saveHistory,
             std::vector<std::string>& solveHistory, std::atomic<int>& progressBar);

void displayProgress(const std::vector<std::atomic<int>>& progressBar, std::size_t& totalIterations);

std::string ConvertMillisecondsToTimeFormat(long long int milliseconds);

//void solve(  std::vector<std::vector<std::string>> input,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
//            std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData);
#endif //SUDOKU_SOLVER_HELPERS_H
