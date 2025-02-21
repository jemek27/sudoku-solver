//
// Created by jemek on 12.02.2025.
//

#include "SudokuSolver.h"

void SudokuSolver::parseStringToMatrix(const std::string& input) {
    size_t index = 0;
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (index < input.size() && isdigit(input[index])) {
                int number = input[index] - '0';
                sudokuTable.table[i][j].number = number;
                --numbersCountDown[number - 1];
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

    for (int i = 0; i < 9; ++i){
        std::cout << i+1 << ":" << int(numbersCountDown[i]) << " ";
    }
    std::cout << std::endl;
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
    for (int i = 0; i < 9; ++i){
        std::cout << i+1 << ":" << int(numbersCountDown[i]) << " ";
    }
    std::cout << std::endl;
}

void SudokuSolver::markPossibilities() {
    markPossibleNumbersColumns();
    markPossibleNumbersRows();
    markPossibleNumbersSquares();
    for (auto row : sudokuTable.rows) {
        for (int i = 0; i < SIZE; ++i) {
            std::cout << int(row.cells[i].first) << " " << int(row.cells[i].second) << " | ";
        }
        std::cout << std::endl;
    }
    for (auto c : sudokuTable.columns) {
        for (int i = 0; i < SIZE; ++i) {
            std::cout << int(c.cells[i].first) << " " << int(c.cells[i].second) << " | ";
        }
        std::cout << std::endl;
    }
    for (auto g : sudokuTable.squares) {
        for (int i = 0; i < SIZE; ++i) {
            std::cout << int(g.cells[i].first) << " " << int(g.cells[i].second) << " | ";
        }
        std::cout << std::endl;
    }

}
void SudokuSolver::markPossibleNumbersColumns(){
    for (int8_t i = 0; i < 9; ++i){
        std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
        for (int j = 0; j < 9; ++j){
            if (sudokuTable.table[j][i].number != 0) {
                numberIsPossible[sudokuTable.table[j][i].number - 1] = false;
            }
        }
        for (int8_t j = 0; j < 9; ++j){
            sudokuTable.table[j][i].numberIsPossible &= numberIsPossible;
            sudokuTable.columns[i].cells[j] = {j, i};
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
            sudokuTable.rows[i].cells[j] = {i, j};
        }
    }
}

void SudokuSolver::markPossibleNumbersSquares(){
    int counterI = 0;
    for (int i = 0; i < 9; i+=3){
        for (int j = 0; j < 9; j+=3){
            int counterJ = 0;
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
                    sudokuTable.squares[counterI].cells[counterJ++] = {i+ii, j+jj};
                }
            }
            ++counterI;
        }
    }
}

// std::bitset<SIZE> SudokuSolver::checkSingleInstances(std::array<Cell, SIZE> cells) {
//     std::array<std::bitset<SIZE>, SIZE> x = {
//         cells[0].numberIsPossible, cells[1].numberIsPossible, cells[2].numberIsPossible,
//         cells[3].numberIsPossible, cells[4].numberIsPossible, cells[5].numberIsPossible,
//         cells[6].numberIsPossible, cells[7].numberIsPossible, cells[8].numberIsPossible
//     };
//
//     std::bitset<SIZE> XOR9 =
//             ~((x[0] & x[1]) | (x[0] & x[2]) | (x[0] & x[3]) | (x[0] & x[4]) | (x[0] & x[5]) | (x[0] & x[6]) | (x[0] & x[7]) | (x[0] & x[8]) |
//               (x[1] & x[2]) | (x[1] & x[3]) | (x[1] & x[4]) | (x[1] & x[5]) | (x[1] & x[6]) | (x[1] & x[7]) | (x[1] & x[8]) |
//               (x[2] & x[3]) | (x[2] & x[4]) | (x[2] & x[5]) | (x[2] & x[6]) | (x[2] & x[7]) | (x[2] & x[8]) |
//               (x[3] & x[4]) | (x[3] & x[5]) | (x[3] & x[6]) | (x[3] & x[7]) | (x[3] & x[8]) |
//               (x[4] & x[5]) | (x[4] & x[6]) | (x[4] & x[7]) | (x[4] & x[8]) |
//               (x[5] & x[6]) | (x[5] & x[7]) | (x[5] & x[8]) |
//               (x[6] & x[7]) | (x[6] & x[8]) |
//               (x[7] & x[8]));
//
//     XOR9 &= x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7] | x[8];
//
//     return XOR9;
// }

std::bitset<SIZE> SudokuSolver::checkSingleInstances(CellGroup cellGroup) {

    std::array<std::bitset<SIZE>, SIZE> x = {
        cellGroup.cells[0]->numberIsPossible, cellGroup.cells[1]->numberIsPossible, cellGroup.cells[2]->numberIsPossible,
        cellGroup.cells[3]->numberIsPossible, cellGroup.cells[4]->numberIsPossible, cellGroup.cells[5]->numberIsPossible,
        cellGroup.cells[6]->numberIsPossible, cellGroup.cells[7]->numberIsPossible, cellGroup.cells[8]->numberIsPossible
    };



    std::bitset<SIZE> XOR9 =
            ~((x[0] & x[1]) | (x[0] & x[2]) | (x[0] & x[3]) | (x[0] & x[4]) | (x[0] & x[5]) | (x[0] & x[6]) | (x[0] & x[7]) | (x[0] & x[8]) |
              (x[1] & x[2]) | (x[1] & x[3]) | (x[1] & x[4]) | (x[1] & x[5]) | (x[1] & x[6]) | (x[1] & x[7]) | (x[1] & x[8]) |
              (x[2] & x[3]) | (x[2] & x[4]) | (x[2] & x[5]) | (x[2] & x[6]) | (x[2] & x[7]) | (x[2] & x[8]) |
              (x[3] & x[4]) | (x[3] & x[5]) | (x[3] & x[6]) | (x[3] & x[7]) | (x[3] & x[8]) |
              (x[4] & x[5]) | (x[4] & x[6]) | (x[4] & x[7]) | (x[4] & x[8]) |
              (x[5] & x[6]) | (x[5] & x[7]) | (x[5] & x[8]) |
              (x[6] & x[7]) | (x[6] & x[8]) |
              (x[7] & x[8]));

    XOR9 &= x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7] | x[8];

    return XOR9;
}

bool SudokuSolver::tryObviousMoves() {
    for (int8_t i = 0; i < 9; ++i) {
        std::bitset<SIZE> singles = checkSingleInstances(sudokuTable.columns[i]);
        //TODO delete form posibilities
    }

}