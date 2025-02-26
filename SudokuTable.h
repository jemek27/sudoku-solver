//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUTABLE_H
#define SUDOKU_SOLVER_SUDOKUTABLE_H

#include "Cell.h"
#include "CellGroup.h"
#include <array>

constexpr int8_t SIZE = 9;

struct SudokuTable {
    int8_t numbersCountDown[SIZE] = {9,9,9,9,9,9,9,9,9};
    std::array<std::array<Cell, SIZE>, SIZE> table{};
    std::array<CellGroup, SIZE> rows = initRows();
    std::array<CellGroup, SIZE> columns = initColumns();
    std::array<CellGroup, SIZE> squares = initSquares();

    constexpr std::array<CellGroup, SIZE> initRows() {
        std::array<CellGroup, SIZE> rows{};
        for (uint8_t i = 0; i < SIZE; ++i) {
            for (uint8_t j = 0; j < SIZE; ++j) {
                rows[i].cells[j] = &table[i][j];
                table[i][j].rowID = i;
            }
        }
        return rows;
    }

    constexpr std::array<CellGroup, SIZE> initColumns() {
        std::array<CellGroup, SIZE> columns{};
        for (uint8_t i = 0; i < SIZE; ++i) {
            for (uint8_t j = 0; j < SIZE; ++j) {
                columns[i].cells[j] = &table[j][i];
                table[j][i].colID = i;
            }
        }
        return columns;
    }

    constexpr std::array<CellGroup, SIZE> initSquares() {
        std::array<CellGroup, SIZE> squares{};
        uint8_t counterI = 0;
        for (uint8_t i = 0; i < SIZE; i += 3) {
            for (uint8_t j = 0; j < SIZE; j += 3) {
                uint8_t counterJ = 0;
                for (uint8_t ii = 0; ii < 3; ++ii) {
                    for (uint8_t jj = 0; jj < 3; ++jj) {
                        squares[counterI].cells[counterJ++] = &table[i + ii][j + jj];
                        table[i + ii][j + jj].squareID = counterI;
                    }
                }
                ++counterI;
            }
        }
        return squares;
    }
};

#endif //SUDOKU_SOLVER_SUDOKUTABLE_H
