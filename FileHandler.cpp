//
// Created by jemek on 12.02.2025.
//

#include "FileHandler.h"

void FileHandler::setFilePath(const std::string& filePath) {
    this->filePath = filePath;
}

bool FileHandler::writeToFile(const std::string& content) {
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile) {
        std::cerr << "Could not open file for writing: " << filePath << std::endl;
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}

bool FileHandler::appendToFile(const std::string& content) {
    std::ofstream outFile(filePath, std::ios::out | std::ios::app);
    if (!outFile) {
        std::cerr << "Could not open file for appending: " << filePath << std::endl;
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}

bool FileHandler::readFromFile(std::string& content) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Could not open file for reading: " << filePath << std::endl;
        return false;
    }
    std::string line;
    content.clear();
    while (std::getline(inFile, line)) {
        content += line + "\n";
    }
    inFile.close();
    return true;
}

bool FileHandler::readCSV(std::vector<std::vector<std::string>>& content, std::size_t numberOfRows) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Could not open file for reading: " << filePath << std::endl;
        return false;
    }
    int c = 0;
    std::string line;
    while (std::getline(inFile, line)) {
        std::vector<std::string> row;
        std::istringstream lineStream(line);
        std::string cell;

        // Podział linii według separatora ','
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        content.push_back(row);
        if (c++ == numberOfRows){ break; }
    }

    inFile.close();
    return true;
}
