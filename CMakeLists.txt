cmake_minimum_required(VERSION 3.28)
project(sudoku_solver)

set(CMAKE_CXX_STANDARD 17)

add_executable(sudoku_solver main.cpp
        FileHandler.cpp
        FileHandler.h
        SudokuSolver.cpp
        SudokuSolver.h
        Cell.h
        CellGroup.h
        SudokuTable.h)
