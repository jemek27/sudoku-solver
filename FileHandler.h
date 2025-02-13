
#ifndef SUDOKU_SOLVER_FILEHANDLER_H
#define SUDOKU_SOLVER_FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>

class FileHandler {
    std::string filePath;
public:
    FileHandler() = default;
    FileHandler(const std::string& filePath) : filePath(filePath) {}

    void setFilePath(const std::string& filePath);

    bool writeToFile(const std::string& content);
    bool appendToFile(const std::string& content);
    bool readFromFile(std::string& content);
};


#endif //SUDOKU_SOLVER_FILEHANDLER_H
