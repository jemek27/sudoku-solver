//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H

#include <iostream>
#include <cstdint>
#include <bitset>

struct Cell {
    uint8_t number = 0;
    //[T, F, T, T...] = 1, 3, 4 are possible
    std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
};

#endif //SUDOKU_SOLVER_CELL_H
