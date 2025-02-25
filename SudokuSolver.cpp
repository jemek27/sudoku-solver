//
// Created by jemek on 12.02.2025.
//

#include "SudokuSolver.h"

void SudokuSolver::parseStringToMatrix(const std::string& input) {
    size_t index = 0;
    for (int8_t i = 0; i < 9; ++i) {
        for (int8_t j = 0; j < 9; ++j) {
            if (index < input.size() && isdigit(input[index])) {
                int8_t number = input[index] - '0';
                insertValue(&sudokuTable.table[i][j], number);
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
    }
    for (int i = 0; i < 9; ++i){
        std::cout << i+1 << ":" << int(numbersCountDown[i]) << " ";
    }
    std::cout << std::endl;
}

void SudokuSolver::markPossibilities() {
    markPossibleNumbersInGroups(sudokuTable.rows);
    markPossibleNumbersInGroups(sudokuTable.columns);
    markPossibleNumbersInGroups(sudokuTable.squares);
}

void SudokuSolver::markPossibleNumbersInGroups(std::array<CellGroup, 9> & groups) {
    for (auto & group : groups) {
        std::bitset<9> numberIsPossible = ~std::bitset<9>(0); //0b1'1111'1111
        for (auto & cell : group.cells) {
            if (cell->number != 0) {
                numberIsPossible[cell->number - 1] = false;
            }
        }
        for (auto & cell : group.cells) {
            cell->numberIsPossible &= numberIsPossible;
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
        if (groups[i].cellsLeft != 0) {
            moveMade |= trySinglePossibilities(groups[i]);
            std::bitset<SIZE> singles = checkSingleInstances(groups[i]);

            for (int8_t j = 0; j < 9; ++j) {
                if (singles[j]) {
                    for (auto & cell : groups[i].cells) {
                        if (cell->numberIsPossible.test(j)) {
                            insertValue(cell, j+1);
                            deletePossibleNumberFromGroups(*cell);
                            moveMade = true;
                            break;
                        }
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
                    deletePossibleNumberFromGroups(*cell);
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
    --sudokuTable.columns[cell->colID].cellsLeft;
    --sudokuTable.rows[cell->rowID].cellsLeft;
    --sudokuTable.squares[cell->squareID].cellsLeft;
    std::cout   << ">" << (int)cell->number << "< " << (int)cell->colID << " " << (int)cell->rowID << " "
                << cell->numberIsPossible << std::endl;
}

void SudokuSolver::deletePossibleNumberFromGroups(Cell cell) {
    std::bitset<SIZE> terminator = ~std::bitset<SIZE>(0); // 0b111....
    terminator.reset(cell.number - 1);  // 0b111011...

    if (sudokuTable.columns[cell.colID].cellsLeft != 0) {
        for (auto & c : sudokuTable.columns[cell.colID].cells) {
            c->numberIsPossible &= terminator;
        }
    }
    if (sudokuTable.rows[cell.rowID].cellsLeft != 0) {
        for (auto & c : sudokuTable.rows[cell.rowID].cells) {
            c->numberIsPossible &= terminator;
        }
    }
    if (sudokuTable.squares[cell.squareID].cellsLeft != 0) {
        for (auto & c : sudokuTable.squares[cell.squareID].cells) {
            c->numberIsPossible &= terminator;
        }
    }
}

//todo add triple and more?
bool SudokuSolver::searchRelationshipsBetweenPairs(CellGroup & cellGroup) {
    bool moveMade = false;
    std::array<int8_t, SIZE> counters = {};
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
        if (group.cellsLeft != 0) {
            moveMade |= searchRelationshipsBetweenPairs(group);
        }
    }
    for (auto & group : sudokuTable.rows) {
        if (group.cellsLeft != 0) {
            moveMade |= searchRelationshipsBetweenPairs(group);
        }
    }
    for (auto & group : sudokuTable.squares) {
        if (group.cellsLeft != 0) {
            moveMade |= searchRelationshipsBetweenPairs(group);
        }
    }

    return moveMade;
}

void SudokuSolver::backtrackSolving() {
    SudokuTable sudokuTableCopy = sudokuTable;
    int8_t numbersCountDownCopy[SIZE];
    for (int8_t i = 0; i < SIZE; ++i) {
        numbersCountDownCopy[i] = numbersCountDown[i];
    }
    int trysCounter = 0;

    while (!allCellsAreFilled()) {
        int8_t currPossibleNumber = 0;

        Cell* cell = findCellWithLowestPossibilities();

        for (int8_t i = 0; i < SIZE; ++i) {
            if (cell->numberIsPossible.test(i)) {
                currPossibleNumber = i+1;
                insertValue(cell, currPossibleNumber);
                deletePossibleNumberFromGroups(*cell);
                tryObviousMoves();
            }
        }


        if (cellWithoutPossibilitiesExists()) {
            std::cout << "\n~~~~" << ++trysCounter << "~~~~\n";
            printTableWithPossibilities();

            sudokuTable = sudokuTableCopy;
            for (int8_t i = 0; i < SIZE; ++i) {
                numbersCountDown[i] = numbersCountDownCopy[i];
            }
            cell->numberIsPossible.reset(currPossibleNumber-1);

            if (cell->numberIsPossible.count() == 1) {
                for (int8_t i = 0; i < SIZE; ++i) {
                    if (cell->numberIsPossible.test(i)) {
                        insertValue(cell, i+1);
                        deletePossibleNumberFromGroups(*cell);
                        tryObviousMoves();
                    }
                }
            }
        }

        std::cout << "\n~~~~" << ++trysCounter << "~~~~\n";
        printTableWithPossibilities();
    }

}

Cell* SudokuSolver::findCellWithLowestPossibilities() {
    int8_t min = 9;
    Cell* result = nullptr;

    for (auto & row : sudokuTable.rows) {
        if (row.cellsLeft != 0) {
            for (auto & cell : row.cells) {
                if (cell->number == 0) {
                    int8_t currNum = cell->numberIsPossible.count();
                    if (currNum < min) {
                        min = currNum;
                        result = cell;
                    }
                    if (min == 1) { break; }
                }
            }
        }
        if (min == 1) { break; } //lowest possible
    }

    return result;
}

bool SudokuSolver::cellWithoutPossibilitiesExists() {
    for (auto & group : sudokuTable.rows) {
        if (group.cellsLeft != 0) {
            for (auto & cell : group.cells){
                if (cell->number == 0 and
                    cell->numberIsPossible.count() == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool SudokuSolver::allCellsAreFilled() {
    for (auto & group : sudokuTable.rows) {
        if (group.cellsLeft != 0) {
            return false;
        }
    }
    return true;
}

bool SudokuSolver::correctSudoku() {
    bool result = false;
    result |= correctGrupeType(sudokuTable.rows);
    result |= correctGrupeType(sudokuTable.columns);
    result |= correctGrupeType(sudokuTable.squares);
    return result;
}

bool SudokuSolver::correctGrupeType(std::array<CellGroup, 9> & groups) {
    for (auto & group : groups) {
        int sum = 0;
        for (auto & cell : group.cells) {
            sum += cell->number;
        }

        if (sum != 45) { return false; }
    }
    return true;
}

void SudokuSolver::testGrupIds() {
    for (int8_t i = 0; i < 9; ++i) {
        for (int8_t j = 0; j < 9; ++j) {
            std::cout   << "r: " << (int)sudokuTable.table[i][j].rowID << "c: " <<  (int)sudokuTable.table[i][j].colID << "s: "
                        << (int)sudokuTable.table[i][j].squareID << std::endl;
        }
    }
}