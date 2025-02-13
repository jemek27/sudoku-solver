//
// Created by jemek on 12.02.2025.
//

#include "SudokuSolver.h"

void SudokuSolver::parseStringToMatrix(const std::string& input) {
    size_t index = 0;
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (index < input.size() && isdigit(input[index])) {
                sudokuTable.table[i][j].number = input[index] - '0';
            } else {
                sudokuTable.table[i][j].number = 0;
            }
            ++index;
        }
        while (index < input.size() && input[index] == '\n') {
            ++index;
        }
    }
}

void SudokuSolver::printTable() {
    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < 9; ++j){
            std::cout << int(sudokuTable.table[i][j].number);
        }
        std::cout << std::endl;
    }
}

void SudokuSolver::printTableWithPossibilities() {
    for (int i = 0; i < 9; ++i){
        std::string row(81, ' ');
        for (int j = 0; j < 9; ++j){
            int jx3 = j*3;
            int operationIndexes[] = {jx3, jx3+1, jx3+2, jx3+27, jx3+28, jx3+29, jx3+54, jx3+55,  jx3+56};
            if (sudokuTable.table[i][j].number == 0) {
                int counter = 1;
                for (int k = 0; k < 9; ++k) {
                    if (sudokuTable.table[i][j].numberIsPossible[k]) {
                        row[operationIndexes[k]] = '0' + counter;
                    }
                    ++counter;
                }
            } else {
                row[operationIndexes[0]] = ' '; row[operationIndexes[1]] = ' '; row[operationIndexes[2]] = ' ';
                row[operationIndexes[3]] = ' '; row[operationIndexes[4]] = '0' + sudokuTable.table[i][j].number; row[operationIndexes[5]] = ' ';
                row[operationIndexes[6]] = ' '; row[operationIndexes[7]] = '^'; row[operationIndexes[8]] = ' ';
            }

        }
        for (int j = 0; j < 80; j+=3){
            if ((j+3)%27 == 0) {
                std::cout << row.substr(j, 3) << std::endl;
            } else {
                std::cout << row.substr(j, 3) + '|';
            }
        }
        std::cout << "---+---+---+---+---+---+---+---+---\n";
//            std::cout << row.substr(0, 27) << std::endl;
//            std::cout << row.substr(27, 27) << std::endl;
//            std::cout << row.substr(54, 27) << std::endl;
    }
}

void SudokuSolver::markPossibilities() {
    markPossibleNumbersColumns();
    markPossibleNumbersRows();
    markPossibleNumbersSquares();
}
void SudokuSolver::markPossibleNumbersColumns(){
    for (int i = 0; i < 9; ++i){
        std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
        for (int j = 0; j < 9; ++j){
            if (sudokuTable.table[j][i].number != 0) {
                numberIsPossible[sudokuTable.table[j][i].number - 1] = false;
            }
        }
        for (int j = 0; j < 9; ++j){
            sudokuTable.table[j][i].numberIsPossible &= numberIsPossible;
        }
    }
}

void SudokuSolver::markPossibleNumbersRows(){
    for (int i = 0; i < 9; ++i){
        std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
        for (int j = 0; j < 9; ++j){
            if (sudokuTable.table[i][j].number != 0) {
                numberIsPossible[sudokuTable.table[i][j].number - 1] = false;
            }
        }
        for (int j = 0; j < 9; ++j){
            sudokuTable.table[i][j].numberIsPossible &= numberIsPossible;
        }
    }
}

void SudokuSolver::markPossibleNumbersSquares(){
    for (int i = 0; i < 9; i+=3){
        for (int j = 0; j < 9; j+=3){
            std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
            for (int ii = 0; ii < 3; ++ii){
                for (int jj = 0; jj < 3; ++jj){
                    if (sudokuTable.table[i+ii][j+jj].number != 0) {
                        numberIsPossible[sudokuTable.table[i+ii][j+jj].number - 1] = false;
                    }
                }
            }
            for (int ii = 0; ii < 3; ++ii){
                for (int jj = 0; jj < 3; ++jj){
                    sudokuTable.table[i+ii][j+jj].numberIsPossible &= numberIsPossible;
                }
            }
        }
    }
}
