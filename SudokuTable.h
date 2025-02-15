//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUTABLE_H
#define SUDOKU_SOLVER_SUDOKUTABLE_H

#include "Cell.h"
#include "CellGroup.h"
#include <array>

struct SudokuTable {
    std::array<CellGroup, 9> rows = initRows();
    std::array<CellGroup, 9> columns = initColumns();
    std::array<CellGroup, 9> squares = initSquares();
    std::array<std::array<Cell, 9>, 9> table{};

    constexpr std::array<CellGroup, 9> initRows() {
        std::array<CellGroup, 9> rows{};
        for (uint8_t i = 0; i < 9; ++i) {
            for (uint8_t j = 0; j < 9; ++j) {
                rows[i].cells[j] = {i, j};
            }
        }
        return rows;
    }

    constexpr std::array<CellGroup, 9> initColumns() {
        std::array<CellGroup, 9> columns{};
        for (uint8_t i = 0; i < 9; ++i) {
            for (uint8_t j = 0; j < 9; ++j) {
                columns[i].cells[j] = {j, i};
            }
        }
        return columns;
    }

    constexpr std::array<CellGroup, 9> initSquares() {
        std::array<CellGroup, 9> squares{};
        uint8_t counterI = 0;
        for (uint8_t i = 0; i < 9; i += 3) {
            for (uint8_t j = 0; j < 9; j += 3) {
                uint8_t counterJ = 0;
                for (uint8_t ii = 0; ii < 3; ++ii) {
                    for (uint8_t jj = 0; jj < 3; ++jj) {
                        squares[counterI].cells[counterJ++] = {i + ii, j + jj};
                    }
                }
                ++counterI;
            }
        }
        return squares;
    }
};

#endif //SUDOKU_SOLVER_SUDOKUTABLE_H
