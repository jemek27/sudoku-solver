//
// Created by Ema on 26.02.2025.
//

#include "helpers.h"

int testOnDataSet(int numOfThread) {
    auto fileHandler = new FileHandler("sudoku.csv"); //inputHard.txt inputNaj.txt sudoku.csv
    std::vector<  std::vector< std::pair<std::size_t ,SudokuSolver> >  > backtrackErrorData(numOfThread);
    std::vector<  std::vector<std::pair<std::size_t ,SudokuSolver> >  >  errorData(numOfThread);

    std::vector<std::vector<std::string>> input {};
    if (!fileHandler->readCSV(input)) {
        std::cout << "No input data!" << std::endl;
        return 0;
    }
    std::cout << "input finished" << std::endl;

    std::vector<std::thread> threads;

    std::size_t segment = input.size() / numOfThread;
    std::size_t segmentStart = 0;
    std::size_t segmentEnd = 0;
    for (int progress = 0; progress < numOfThread; ++progress) {
        segmentStart = (progress == 0) ? 1 : segment * progress; // id 0 has col names
        segmentEnd = (progress == numOfThread - 1) ? input.size() : segment * (progress + 1);

        threads.emplace_back(solve, input.begin() + segmentStart, input.begin() + segmentEnd, errorData[progress], backtrackErrorData[progress]);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "finished solving" << std::endl;

    std::string printBuffer = {};

    int numOfBacktrackErrorData = 0;
    int numOfErrorData = 0;

    for (auto & dataSegment : backtrackErrorData) {
        for (auto & data : dataSegment) {
            printBuffer += "BE: " + std::to_string(data.first)  + "\n";
            printBuffer += "BE: " + input[data.first][0] + "\n";
            printBuffer += "BE: " + input[data.first][1] + "\n";
            printBuffer += "BE: " + data.second.getTableString() + "\n";
        }
        numOfBacktrackErrorData += dataSegment.size();
    }

    for (auto & dataSegment : errorData) {
        for (auto & data : dataSegment) {
            printBuffer += "E: " + std::to_string(data.first) + "\n";
            printBuffer += "E: " + input[data.first][0] + "\n";
            printBuffer += "E: " + input[data.first][1] + "\n";
            printBuffer += "E: " + data.second.getTableString() + "\n";
        }
        numOfErrorData += dataSegment.size();
    }

    std::cout << printBuffer << std::endl;
    std::cout << "BE: " << numOfBacktrackErrorData  << std::endl;
    std::cout << "E: " << numOfErrorData << std::endl;
    std::cout << "ALL: " << numOfBacktrackErrorData + numOfErrorData << std::endl;

    std::string fileBuffer = {};
    fileBuffer += input[0][0] + ',' + input[0][1] + '\n';

    for (auto & dataSegment : backtrackErrorData) {
        for (auto & data : dataSegment) {
            fileBuffer += input[data.first][0] + ',' + input[data.first][1] + '\n';
        }
    }
    for (auto & dataSegment : errorData) {
        for (auto & data : dataSegment) {
            fileBuffer += input[data.first][0] + ',' + input[data.first][1] + '\n';
        }
    }

    fileHandler->setFilePath("errorData.csv");
    fileHandler->writeToFile(fileBuffer);

    delete fileHandler;
    return 0;
}

void solve(  std::vector<std::vector<std::string>> input,
            std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
            std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData) {
    const int segmentation = 100;
    std::size_t segment = input.size() / segmentation;
    std::size_t segmentStart = 0;
    std::size_t segmentEnd = 0;

    for (int progress = 0; progress < segmentation; ++progress) {
        segmentStart = progress == 0 ? 1 : segment * progress; // id 0 has col names
        segmentEnd = progress == segmentation - 1 ? input.size() : segment * (progress + 1);

        for (std::size_t i = segmentStart; i < segmentEnd; ++i) {
            auto sudokuSolver = SudokuSolver();

            sudokuSolver.parseDigitOneLineStringToMatrix(input[i][0]);
            sudokuSolver.markPossibilities();
            do {
                sudokuSolver.tryObviousMoves();
            } while (sudokuSolver.searchForRelationships());

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
        std::cout << std::to_string(progress) + '\n';
        if (backtrackErrorData.size() + errorData.size() > 0) {
            std::cout << "BE: " << backtrackErrorData.size()  << std::endl;
            std::cout << "E: " << errorData.size() << std::endl;
            std::cout << "ALL: " << backtrackErrorData.size() + errorData.size() << std::endl;
        }
    }
}

void solve(  std::vector<std::vector<std::string>>::iterator start, std::vector<std::vector<std::string>>::iterator end,
             std::vector<std::pair<std::size_t ,SudokuSolver>> errorData,
             std::vector<std::pair<std::size_t ,SudokuSolver>> backtrackErrorData) {
    const int segmentation = 100;
    std::size_t segment = (int)(end - start) / segmentation;
    std::size_t segmentStart = 0;
    std::size_t segmentEnd = 0;

    for (int progress = 0; progress < segmentation; ++progress) {
        segmentStart = progress == 0 ? 1 : segment * progress; // id 0 has col names
        segmentEnd = progress == segmentation - 1 ? (int)(end - start) : segment * (progress + 1);

        for (auto it = start + segmentStart; it != start + segmentEnd; ++it) {
            auto sudokuSolver = SudokuSolver();

            sudokuSolver.parseDigitOneLineStringToMatrix((*it)[0]);
            sudokuSolver.markPossibilities();
            do {
                sudokuSolver.tryObviousMoves();
            } while (sudokuSolver.searchForRelationships());

            if (!sudokuSolver.backtrackSolving()) {
                std::cout << end - it << " backtrack error" << std::endl;
                backtrackErrorData.push_back({end - it, sudokuSolver});
            } else {
                if (sudokuSolver.correctSudoku((*it)[1])) {
                    //::cout << "~~~~WIN~~~~" << i << std::endl;
                } else if (sudokuSolver.correctSudoku()) { //double check maybe provided solution is incorrect
                    //std::cout << "~~~~WIN~~~~" << i << std::endl;
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({end - it, sudokuSolver});
                } else {
                    //std::cout << "~~~~LOSE~~~~"<< i << std::endl;
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({end - it, sudokuSolver});
                }
            }
        }
        std::cout << progress << std::endl;
        if (backtrackErrorData.size() + errorData.size() > 0) {
            std::cout << "BE: " << backtrackErrorData.size()  << std::endl;
            std::cout << "E: " << errorData.size() << std::endl;
            std::cout << "ALL: " << backtrackErrorData.size() + errorData.size() << std::endl;
        }
    }
}

int solveSudoku(std::string inputPath) {
    auto fileHandler = new FileHandler(inputPath); //inputHard.txt inputNaj.txt
    auto sudokuSolver = SudokuSolver();

    std::string input {};
    if (!fileHandler->readFromFile(input)) {
        std::cout << "No input data!" << std::endl;
        return 0;
    }
    std::cout << input << std::endl;

    sudokuSolver.parseStringToMatrix(input);
    sudokuSolver.printTableWithPossibilities();
    sudokuSolver.markPossibilities();
    std::cout << "\n-------------------------------\n";

    do {
        sudokuSolver.printTableWithPossibilities();
        sudokuSolver.tryObviousMoves();
        sudokuSolver.printTableWithPossibilities();
        //todo test if finished
    } while (sudokuSolver.searchForRelationships());

    sudokuSolver.backtrackSolving();
    sudokuSolver.printTableWithPossibilities();

    if (sudokuSolver.correctSudoku()) {
        std::cout << "~~~~WIN~~~~" << std::endl;
    } else {
        std::cout << "~~~~LOSE~~~~" << std::endl;
    }

    delete fileHandler;
    return 0;
}
