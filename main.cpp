#include <iostream>
#include "FileHandler.h"
#include "SudokuSolver.h"
#include "SudokuTable.h"
#include <bitset>
#include <iostream>

int main() {
    auto fileHandler = new FileHandler("sudoku.csv"); //inputHard.txt inputNaj.txt sudoku.csv
    std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData;
    std::vector<std::pair<std::size_t ,SudokuSolver>> errorData;

    std::vector<std::vector<std::string>> input {};
    if (!fileHandler->readCSV(input, 100'000)) {
        std::cout << "No input data!" << std::endl;
        return 0;
    }
    std::cout << "input finished" << std::endl;


    std::size_t segment = input.size() / 1000;
    std::size_t segmentStart = 0;
    std::size_t segmentEnd = 0;

    for (int promil = 0; promil < 1000; ++promil) {
        segmentStart = promil == 0 ? 1 : segment * promil; // id 0 has col names
        segmentEnd = promil == 999 ? input.size() : segment * (promil + 1);

        for (std::size_t i = segmentStart; i < segmentEnd; ++i) {
            auto sudokuSolver = SudokuSolver();

            sudokuSolver.parseDigitOneLineStringToMatrix(input[i][0]);
//        sudokuSolver.printTableWithPossibilities();
            sudokuSolver.markPossibilities();
            do {
//            sudokuSolver.printTableWithPossibilities();
                sudokuSolver.tryObviousMoves();
//            sudokuSolver.printTableWithPossibilities();
            } while (sudokuSolver.searchForRelationships());

//            std::cout << "backtrack" << std::endl;
            if (!sudokuSolver.backtrackSolving()) {
                std::cout << i << " backtrack error" << std::endl;
                backtrackErrorData.push_back({i, sudokuSolver});
            } else {
                if (sudokuSolver.correctSudoku(input[i][1])) {
                    //::cout << "~~~~WIN~~~~" << i << std::endl;
                } else if (sudokuSolver.correctSudoku()) { //double check maybe provided solution is incorrect
                    //std::cout << "~~~~WIN~~~~" << i << std::endl;
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({i, sudokuSolver});
                } else {
                    //std::cout << "~~~~LOSE~~~~"<< i << std::endl;
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({i, sudokuSolver});
                }
            }
        }
        std::cout << "BE: " << backtrackErrorData.size()  << std::endl;
        std::cout << "E: " << errorData.size() << std::endl;
        std::cout << "ALL: " << backtrackErrorData.size() + errorData.size() << std::endl;
    }

//    for (auto & data : backtrackErrorData) {
//        std::cout << "BE: " << data.first << "\n";
//        std::cout << "BE: " << input[data.first][0] << "\n";
//        std::cout << "BE: " << input[data.first][1] << "\n";
//        data.second.printTableWithPossibilities();
//    }
//
//    for (auto & data : errorData) {
//        std::cout << "E: " << data.first << "\n";
//        std::cout << "E: " << input[data.first][0] << "\n";
//        std::cout << "E: " << input[data.first][1] << "\n";
//        data.second.printTableWithPossibilities();
//    }

    std::string printBuffer = {};

    for (auto & data : backtrackErrorData) {
        printBuffer += "BE: " + std::to_string(data.first)  + "\n";
        printBuffer += "BE: " + input[data.first][0] + "\n";
        printBuffer += "BE: " + input[data.first][1] + "\n";
        printBuffer += "BE: " + data.second.getTableString() + "\n";
    }

    for (auto & data : errorData) {
        printBuffer += "E: " + std::to_string(data.first)  + "\n";
        printBuffer += "E: " + input[data.first][0] + "\n";
        printBuffer += "E: " + input[data.first][1] + "\n";
        printBuffer += "E: " + data.second.getTableString() + "\n";
    }

    std::cout << printBuffer << std::endl;
    std::cout << "BE: " << backtrackErrorData.size()  << std::endl;
    std::cout << "E: " << errorData.size() << std::endl;
    std::cout << "ALL: " << backtrackErrorData.size() + errorData.size() << std::endl;

    std::string fileBuffer = {};
    fileBuffer += input[0][0] + ',' + input[0][1] + '\n';

    for (auto & data : backtrackErrorData) {
        fileBuffer += input[data.first][0] + ',' + input[data.first][1] + '\n';
    }
    for (auto & data : errorData) {
        fileBuffer += input[data.first][0] + ',' + input[data.first][1] + '\n';
    }

    fileHandler->setFilePath("errorData.csv");
    fileHandler->writeToFile(fileBuffer);

    delete fileHandler;
    return 0;
}

//
//int main() {
//    auto fileHandler = new FileHandler("inputNaj.txt"); //inputHard.txt inputNaj.txt
//    auto sudokuSolver = SudokuSolver();
//
//    std::string input {};
//    if (!fileHandler->readFromFile(input)) {
//        std::cout << "No input data!" << std::endl;
//        return 0;
//    }
//    std::cout << input << std::endl;
//
//    sudokuSolver.parseStringToMatrix(input);
//    sudokuSolver.printTableWithPossibilities();
//    sudokuSolver.markPossibilities();
//    std::cout << "\n-------------------------------\n";
//
//    do {
//        sudokuSolver.printTableWithPossibilities();
//        sudokuSolver.tryObviousMoves();
//        sudokuSolver.printTableWithPossibilities();
//        //todo test if finished
//    } while (sudokuSolver.searchForRelationships());
//
//    sudokuSolver.backtrackSolving();
//
//    if (sudokuSolver.correctSudoku()) {
//        std::cout << "~~~~WIN~~~~" << std::endl;
//    } else {
//        std::cout << "~~~~LOSE~~~~" << std::endl;
//    }
//
//    delete fileHandler;
//    return 0;
//}
