
#ifndef SUDOKU_SOLVER_FILEHANDLER_H
#define SUDOKU_SOLVER_FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>


class FileHandler {
    std::string filePath;
public:
    FileHandler() = default;
    FileHandler(const std::string& filePath) : filePath(filePath) {}

    void setFilePath(const std::string& filePath);

    bool writeToFile(const std::string& content);
    bool appendToFile(const std::string& content);
    bool readFromFile(std::string& content);
    bool readCSV(std::vector<std::vector<std::string>>& content, std::size_t maxNumberOfRows = 0xffffffff);
    std::pair<std::size_t, std::size_t> csvSize(const std::string& filename);
    std::vector<std::vector<std::string>> loadLargeCsv(const std::string& filename); //todo bool and  (vector&)
    void loadLargeCsvInToQueue(const std::string& filename, int numberOfSegments, bool& inputFinished,
                               std::size_t& estimatedRowCount, std::size_t numOfColumns,
                               std::vector<std::string>& readColumnNames,
                               std::vector<std::shared_ptr<std::vector<std::vector<std::string>>>>& segments);
};


#endif //SUDOKU_SOLVER_FILEHANDLER_H
