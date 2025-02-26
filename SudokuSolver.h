//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUSOLVER_H
#define SUDOKU_SOLVER_SUDOKUSOLVER_H

#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include "SudokuTable.h"

constexpr int8_t SIZE = 9;

class SudokuSolver {
    int8_t numbersCountDown[SIZE] = {9,9,9,9,9,9,9,9,9};

    SudokuTable sudokuTable;
public:
    void parseStringToMatrix(const std::string& input);
    void parseDigitOneLineStringToMatrix(const std::string& input);

    void printTable();
    void printTableWithPossibilities();

    std::string getTableString();

    void markPossibilities();
    void markPossibleNumbersInGroups(std::array<CellGroup, 9> & groups);

    bool searchForRelationships();
    void tryObviousMoves();
    bool backtrackSolving();

    std::bitset<SIZE> checkSingleInstances(CellGroup cellGroup);
    bool trySinglePossibilities(CellGroup & cellGroup);
    bool tryObviousMovesOnGroup(std::array<CellGroup, 9> & group);
    bool searchRelationshipsBetweenPairs(CellGroup & cellGroup);
    //todo add Naked Pairs i Naked Triples?

    Cell* findCellWithLowestPossibilities();
    bool cellWithoutPossibilitiesExists();
    bool allCellsAreFilled();


    void insertValue(Cell * cell, int8_t number);
    void deletePossibleNumberFromGroups(Cell cell);
    bool correctSudoku();
    bool correctSudoku(std::string& correctNumbers);
    bool correctGroupType(std::array<CellGroup, 9> & groups);



    void testGroupIds();
};


#endif //SUDOKU_SOLVER_SUDOKUSOLVER_H
