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
        std::cerr << "Nie można otworzyć pliku do zapisu: " << filePath << std::endl;
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}

bool FileHandler::appendToFile(const std::string& content) {
    std::ofstream outFile(filePath, std::ios::out | std::ios::app);
    if (!outFile) {
        std::cerr << "Nie można otworzyć pliku do dopisania: " << filePath << std::endl;
        return false;
    }
    outFile << content;
    outFile.close();
    return true;
}

bool FileHandler::readFromFile(std::string& content) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Nie można otworzyć pliku do odczytu: " << filePath << std::endl;
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