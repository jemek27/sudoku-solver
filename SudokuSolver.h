//
// Created by jemek on 12.02.2025.
//

#ifndef SUDOKU_SOLVER_SUDOKUSOLVER_H
#define SUDOKU_SOLVER_SUDOKUSOLVER_H

#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <stack>
#include "SudokuTable.h"

class SudokuSolver {
    SudokuTable sudokuTable;
    std::string solveHistory;
public:
    void parseStringToMatrix(const std::string& input);
    void parseDigitOneLineStringToMatrix(const std::string& input);

    void printTable();
    void printTableWithPossibilities();

    std::string getSolveHistory();
    std::string getTableString();
    std::string getTableWithPossibilitiesString();
    std::string possibilitiesToString(std::bitset<9> possibilities);

    void markPossibilities();
    void markPossibleNumbersInGroups(std::array<CellGroup, 9> & groups);

    bool searchForRelationships();
    void tryObviousMoves();
    bool backtrackSolving();

    std::bitset<SIZE> checkSingleInstances(CellGroup cellGroup);
    bool trySinglePossibilities(CellGroup & cellGroup, const std::string& groupID);
    bool tryObviousMovesOnGroup(std::array<CellGroup, 9> & group, const std::string& groupID);
    bool searchRelationshipsBetweenPairs(CellGroup & cellGroup, const std::string& groupID);
    //todo add Naked Pairs i Naked Triples?

    Cell* findCellWithLowestPossibilities();
    bool cellWithoutPossibilitiesExists();
    bool allCellsAreFilled();


    void insertValue(Cell *cell, int8_t number);
    void insertValue(Cell *cell, int8_t number, const std::string& moveID);
    void deletePossibleNumberFromGroups(Cell cell);
    bool correctSudoku();
    bool correctSudoku(std::string& correctNumbers);
    bool correctGroupType(std::array<CellGroup, 9> & groups);



    void testGroupIds();
};


#endif //SUDOKU_SOLVER_SUDOKUSOLVER_H
