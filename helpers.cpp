//
// Created by Ema on 26.02.2025.
//

#include "helpers.h"

int solveSudoku(const std::string& inputPath) {
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
    } while (sudokuSolver.searchForRelationships());

    sudokuSolver.backtrackSolving();
    sudokuSolver.printTableWithPossibilities();

    std::cout << sudokuSolver.getSolveHistory();

    if (sudokuSolver.correctSudoku()) {
        std::cout << "~~~~WIN~~~~" << std::endl;
    } else {
        std::cout << "~~~~LOSE~~~~" << std::endl;
    }

    delete fileHandler;
    return 0;
}

int testOnDataSet(int numOfThreads) {
    auto fileHandler = new FileHandler("sudoku.csv"); //inputHard.txt inputNaj.txt sudoku.csv
    std::vector<  std::vector< std::pair<std::size_t ,SudokuSolver> >  > backtrackErrorData(numOfThreads);
    std::vector<  std::vector<std::pair<std::size_t ,SudokuSolver> >  >  errorData(numOfThreads);
    std::vector<  std::vector<std::string>  > solveHistory(numOfThreads);
    std::vector<std::atomic<int>> progressBar(numOfThreads);
    for (auto& p : progressBar) {
        p.store(0, std::memory_order_relaxed); // Init with 0
    }

    std::vector<std::vector<std::string>> input {};
    if (!fileHandler->readCSV(input)) {
        std::cout << "No input data!" << std::endl;
        return 0;
    }
    std::cout << "Input finished" << std::endl;

    std::vector<std::thread> threads;

    std::size_t segment = input.size() / numOfThreads;
    std::size_t segmentStart = 0;
    std::size_t segmentEnd = 0;
    for (int progress = 0; progress < numOfThreads; ++progress) {
        segmentStart = (progress == 0) ? 1 : segment * progress; // id 0 has col names
        segmentEnd = (progress == numOfThreads - 1) ? input.size() : segment * (progress + 1);

        threads.emplace_back(solve, input.begin() + segmentStart, input.begin() + segmentEnd,
                             std::ref(errorData[progress]), std::ref(backtrackErrorData[progress]),
                             std::ref(solveHistory[progress]), std::ref(progressBar[progress]));
    }

    std::thread progressThread(displayProgress, std::ref(progressBar), numOfThreads);

    for (auto& t : threads) {
        t.join();
    }

    progressThread.join();

    std::cout << "Finished solving" << std::endl;

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

    std::cout << "Saving data" << std::endl;

    fileHandler->setFilePath("errorData.csv");
    fileHandler->writeToFile(fileBuffer);


    fileBuffer = {};
    fileBuffer += input[0][0] + ',' + input[0][1]  + ',' + "solving" + '\n';

    int i = 1;
    for (auto & dataSegment : solveHistory) {
        for (auto & data : dataSegment) {
            fileBuffer += input[i][0] + ',' + input[i][1] + ',' + data + ',' + '\n';
            ++i;
        }
    }

    fileHandler->setFilePath("solvedSudoku.csv");
    fileHandler->writeToFile(fileBuffer);

    delete fileHandler;
    return 0;
}

void solve(  std::vector<std::vector<std::string>>::iterator start, std::vector<std::vector<std::string>>::iterator end,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& errorData,
             std::vector<std::pair<std::size_t ,SudokuSolver>>& backtrackErrorData,
             std::vector<std::string>& solveHistory, std::atomic<int>& progressBar) {

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

            std::string status;

            if (!sudokuSolver.backtrackSolving()) {
                std::cout << end - it << " backtrack error" << std::endl;
                backtrackErrorData.push_back({end - it, sudokuSolver});
                status =  "~~ERROR/NO-SOLUTION~~";
            } else {
                if (sudokuSolver.correctSudoku((*it)[1])) {
                    status =  "~~WIN~~";
                } else if (sudokuSolver.correctSudoku()) { //double check maybe provided solution is incorrect
                    status =  "~~WIN?~~";
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({end - it, sudokuSolver});
                } else {
                    status =  "~~LOSE~~";
                    sudokuSolver.printTableWithPossibilities();
                    errorData.push_back({end - it, sudokuSolver});
                }
            }
            solveHistory.push_back(sudokuSolver.getSolveHistory() + status);
        }
        ++progressBar;

        if (backtrackErrorData.size() + errorData.size() > 0) {
            std::cout << "BE: " << backtrackErrorData.size()  << std::endl;
            std::cout << "E: " << errorData.size() << std::endl;
            std::cout << "ALL: " << backtrackErrorData.size() + errorData.size() << std::endl;
        }
    }
}

void displayProgress(const std::vector<std::atomic<int>>& progressBar, int numThreads) {
    const int barWidth = 50;
    int lastProgress = -1; // Poprzedni postęp (do optymalizacji wyświetlania)
    int totalIterations = segmentation * numThreads;
    std::string writeBuffer {};

    auto start = steady_clock::now();
    while (true) {

        int totalProgress = 0;
        for (const auto& progress : progressBar) {
            totalProgress += progress.load(std::memory_order_relaxed); // Atomic read, minimal barriers
        }

        if (totalProgress != lastProgress) {
            lastProgress = totalProgress;

            float percentage = (float)totalProgress / totalIterations * 100;

            // Drawing progress bar
            int pos = barWidth * percentage / 100;
            std::cout << "\r"; // \r goes back to the beginning of the line
            writeBuffer += "[";
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) { writeBuffer += "|"; }
                else         { writeBuffer += " "; }
            }
            writeBuffer += "]";
            std::cout << writeBuffer;
            std::cout << std::fixed << std::setprecision(1) << percentage << "%" << std::endl;
            writeBuffer.clear();
        }

        // End if all work done
        if (totalProgress >= totalIterations) {
            auto end = steady_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            std::cout << "\nExecution time: " << ConvertMillisecondsToTimeFormat(duration.count()) << " ms" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::string ConvertMillisecondsToTimeFormat(long long int milliseconds) {
    int minutes = milliseconds / 60000;
    int seconds = (milliseconds % 60000) / 1000;
    int remainingMilliseconds = milliseconds % 1000;

    // Format as MM:SS:FFF
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%03d", minutes, seconds, remainingMilliseconds);
    return {buffer};
}
