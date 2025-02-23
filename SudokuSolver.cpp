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
                sudokuTable.table[i][j].numberIsPossible = 0;
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
                }
            }
            ++counterI;
        }
    }
}

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

void SudokuSolver::tryObviousMoves() {
    bool moveMade;
    do {
        moveMade = false;
        moveMade |= tryObviousMovesOnGroup(sudokuTable.columns);
        moveMade |= tryObviousMovesOnGroup(sudokuTable.rows);
        moveMade |= tryObviousMovesOnGroup(sudokuTable.squares);
        std::cout << "Obvious moves\n";
    } while (moveMade);
}

bool SudokuSolver::tryObviousMovesOnGroup(std::array<CellGroup, 9> & groups) {
    bool moveMade = false;
    for (int8_t i = 0; i < 9; ++i) {

        moveMade |= trySinglePossibilities(groups[i]);
        std::bitset<SIZE> singles = checkSingleInstances(groups[i]);

        for (int8_t j = 0; j < 9; ++j) {
            if (singles[j]) {
                for (auto & cell : groups[i].cells) {
                    if (cell->numberIsPossible.test(j)) {
                        insertValue(cell, j+1);
                        moveMade = true;
                        break;
                    }
                }
            }
        }
    }
    return moveMade;
}

bool SudokuSolver::trySinglePossibilities(CellGroup & cellGroup) {
    bool moveMade = false;
    for (auto & cell : cellGroup.cells){
        if(cell->numberIsPossible.count() == 1) {
            for (int8_t i = 0; i < SIZE; ++i) {
                if(cell->numberIsPossible.test(i)) {
                    insertValue(cell, i + 1);
                    moveMade = true;
                    break;
                }
            }
        }
    }
    return moveMade;
}

void SudokuSolver::insertValue(Cell *cell, int8_t number) {
    cell->number = number;
    cell->numberIsPossible = 0;
    --numbersCountDown[number - 1];
    deletePossibleNumberFromGroups(*cell);
    std::cout << (int)cell->number << " " << cell->numberIsPossible << " "
              << (int)cell->colID << " " << (int)cell->rowID << std::endl;
}

void SudokuSolver::deletePossibleNumberFromGroups(Cell cell) {
    std::bitset<SIZE> terminator = ~std::bitset<SIZE>(0); // 0b111....
    terminator.reset(cell.number - 1);  // 0b111011...

    for (int8_t i = 0; i < 9; ++i) {
        sudokuTable.columns[cell.colID].cells[i]->numberIsPossible &= terminator;
        sudokuTable.rows[cell.rowID].cells[i]->numberIsPossible &= terminator;
        sudokuTable.squares[cell.squareID].cells[i]->numberIsPossible &= terminator;
    }
}

//todo add triple and more?
bool SudokuSolver::searchRelationshipsBetweenPairs(CellGroup & cellGroup) {
    bool moveMade = false;
    std::array<int8_t, SIZE> counters = {};
    std::cout << (int)counters[0] << std::endl;
    std::bitset<SIZE> twoOccurrences = std::bitset<SIZE>(0);

    for (int8_t i = 0; i < 9; ++i){
        for (int8_t j = 0; j < 9; ++j){
            if (cellGroup.cells[i]->numberIsPossible.test(j)){
                ++counters[j];
            }
        }
    }

    for (int8_t i = 0; i < 9; ++i){
        if (counters[i] == 2) {
            twoOccurrences.set(i);
        }
    }
    for (int8_t i = 0; i < 9; ++i){
        std::cout << (int)counters[i] << " ";
    }
    std::cout << " " << twoOccurrences << std::endl;

    std::array<std::bitset<SIZE>, SIZE> tempPossibilities = {};
    for (int8_t i = 0; i < 9; ++i){
        std::bitset<SIZE> tempPossibility = cellGroup.cells[i]->numberIsPossible & twoOccurrences;
        if ((tempPossibility).count() > 1) {
            tempPossibilities[i] = tempPossibility;
        }
    }

    for (int8_t i = 0; i < 9; ++i){
        if (tempPossibilities[i] != 0) {
            for (int8_t j = i + 1; j < 9; ++j){
                if (tempPossibilities[j] != 0 and
                    (cellGroup.cells[i]->numberIsPossible.count() > 2 or
                    cellGroup.cells[j]->numberIsPossible.count() > 2) and
                    tempPossibilities[i] == tempPossibilities[j]){
                    std::cout   << " pos1: " << cellGroup.cells[i]->numberIsPossible
                                << " pos2: " << cellGroup.cells[j]->numberIsPossible << std::endl;

                    cellGroup.cells[i]->numberIsPossible = tempPossibilities[i];
                    cellGroup.cells[j]->numberIsPossible = tempPossibilities[i];
                    moveMade = true;

                    std::cout << " 1c: " <<  (int)cellGroup.cells[i]->colID << " 1r: " <<  (int)cellGroup.cells[i]->rowID
                              << " 2c: " <<  (int)cellGroup.cells[j]->colID << " 2r: " <<  (int)cellGroup.cells[j]->rowID
                              << " pos: " << cellGroup.cells[i]->numberIsPossible << std::endl;
                }
            }
        }
    }

    return moveMade;
}

bool SudokuSolver::searchForRelationships() {
    bool moveMade = false;

    for (auto & group : sudokuTable.columns) {
        moveMade |= searchRelationshipsBetweenPairs(group);
    }
    for (auto & group : sudokuTable.rows) {
        moveMade |= searchRelationshipsBetweenPairs(group);
    }
    for (auto & group : sudokuTable.squares) {
        moveMade |= searchRelationshipsBetweenPairs(group);
    }

    return moveMade;
}

void SudokuSolver::test() {
    for (int8_t i = 0; i < 9; ++i) {
        for (int8_t j = 0; j < 9; ++j) {
            std::cout   << "r: " << (int)sudokuTable.table[i][j].rowID << "c: " <<  (int)sudokuTable.table[i][j].colID << "s: "
                        << (int)sudokuTable.table[i][j].squareID << std::endl;
        }
    }
}




