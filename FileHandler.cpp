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

bool FileHandler::readCSV(std::vector<std::vector<std::string>>& content, std::size_t maxNumberOfRows) {
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
        if (c++ == maxNumberOfRows){ break; }
    }

    inFile.close();
    return true;
}

std::pair<std::size_t, std::size_t> FileHandler::csvSize(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return {0, 0};
    }

    std::string header, firstRecord;
    std::getline(file, header);  // Pobieramy nagłówek
    std::getline(file, firstRecord);  // Pobieramy pierwszy rekord

    if (header.empty()) return {0, 0}; // Pusty plik

    // Liczymy liczbę kolumn (liczba przecinków + 1)
    std::size_t numOfColumns = std::count(header.begin(), header.end(), ',') + 1;
    std::size_t fileSize = std::filesystem::file_size(filename);

    if (firstRecord.empty()) return {0, numOfColumns}; // Plik zawiera tylko nagłówek

    std::size_t avgRecordSize = firstRecord.size() + 1; // +1 dla znaku '\n'
    std::size_t estimatedRows = (fileSize / avgRecordSize);

    return {estimatedRows, numOfColumns};
}


std::vector<std::vector<std::string>> FileHandler::loadLargeCsv(const std::string& filename) {
    auto [estimatedRowCount, numOfColumns] = csvSize(filename);
    if (estimatedRowCount == 0 || numOfColumns == 0) {
        std::cerr << "Brak danych w pliku CSV." << std::endl;
        return {};
    }

    std::size_t bufferSize = std::min<std::size_t>(64 * 1024 * 1024, std::filesystem::file_size(filename)); // Maks 64 MB
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<std::string>> data;
    data.reserve(estimatedRowCount); // Rezerwujemy miejsce

    std::vector<char> buffer(bufferSize);
    std::string leftover;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        std::size_t bytesRead = file.gcount();
        std::string chunk(buffer.data(), bytesRead);

        chunk = leftover + chunk;
        leftover.clear();

        std::istringstream chunkStream(chunk);
        std::string line;
        while (std::getline(chunkStream, line)) {
            if (chunkStream.eof()) {
                leftover = line; // Ostatnia linia może być niepełna, zapisujemy
                break;
            }

            // Parsowanie CSV
            std::stringstream lineStream(line);
            std::vector<std::string> row;
            std::string value;

            while (std::getline(lineStream, value, ',')) {
                row.push_back(value);
            }

            // Sprawdzenie, czy mamy odpowiednią liczbę kolumn
            while (row.size() < numOfColumns) row.push_back(""); // Uzupełniamy brakujące wartości

            data.push_back(std::move(row));
        }
    }

    if (!leftover.empty()) {
        std::stringstream lineStream(leftover);
        std::string value;
        std::vector<std::string> row;
        while (std::getline(lineStream, value, ',')) {
            row.push_back(value);
        }
        data.push_back(std::move(row));
    }

    std::cout << "Wczytano " << data.size() - 1 << " rekordów + nagłówek. Esteamted:" << estimatedRowCount << std::endl;

    return data;
}


void FileHandler::loadLargeCsvInToQueue(const std::string& filename, const int numberOfSegments, bool& inputFinished,
                                        std::size_t& estimatedRowCount, std::size_t numOfColumns,
                                        std::vector<std::string>& readColumnNames,
                                        std::vector<std::shared_ptr<std::vector<std::vector<std::string>>>>& segments) {
    if (estimatedRowCount == 0 || numOfColumns == 0) {
        std::cerr << "Brak danych w pliku CSV." << std::endl;
        inputFinished = false;
        return;
    }

    // Przydzielamy segmenty
    segments.resize(numberOfSegments);
    for (int i = 0; i < numberOfSegments; ++i) {
        // Prealokujemy segment – możemy np. oszacować, ile wierszy przypada na segment
        segments[i] = std::make_shared<std::vector<std::vector<std::string>>>();
        segments[i]->reserve(estimatedRowCount / numberOfSegments);
    }

    std::size_t bufferSize = std::min<std::size_t>(64 * 1024 * 1024, std::filesystem::file_size(filename)); // Maks 64 MB
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        inputFinished = false;
        return;
    }

    // Wskaźnik do aktualnego segmentu – możemy np. przydzielać wiersze round-robin
    int currentSegment = 0;

    std::vector<char> buffer(bufferSize);
    std::string leftover;

    int aaa = 0;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        if (aaa++ > 1) { break; }
        std::size_t bytesRead = file.gcount();
        std::string chunk(buffer.data(), bytesRead);

        chunk = leftover + chunk;
        leftover.clear();

        std::istringstream chunkStream(chunk);
        std::string line;

        if (segments[0]->empty()) {
            if (std::getline(chunkStream, line)) {
                std::stringstream lineStream(line);
                std::vector<std::string> header;
                std::string value;
                while (std::getline(lineStream, value, ',')) {
                    readColumnNames.push_back(value);
                }
            }
        }

        while (std::getline(chunkStream, line)) {
            // Jeśli ostatnia linia w chunk jest niepełna, przerwij i zapisz do leftover.
            if (chunkStream.eof()) {
                leftover = line;
                break;
            }
            // Parsowanie CSV
            std::stringstream lineStream(line);
            std::vector<std::string> row;
            std::string value;
            while (std::getline(lineStream, value, ',')) {
                row.push_back(value);
            }
            while (row.size() < numOfColumns) row.push_back(""); // Uzupełniamy brakujące kolumny

            // Dodajemy wiersz do bieżącego segmentu (np. round-robin)
            segments[currentSegment]->push_back(std::move(row));
            currentSegment = (currentSegment + 1) % numberOfSegments;
        }
    }

    // Obsługa ewentualnego niedokończonego wiersza
    if (!leftover.empty()) {
        std::stringstream lineStream(leftover);
        std::vector<std::string> row;
        std::string value;
        while (std::getline(lineStream, value, ',')) {
            row.push_back(value);
        }
        while (row.size() < numOfColumns) row.push_back("");
        segments[currentSegment]->push_back(std::move(row));
    }

    std::size_t actualRowCount = 0;
    for (auto p : segments) {
        actualRowCount += p->size();
    }

    std::cout << "Wczytano " << actualRowCount << " rekordów i podzielono na " << numberOfSegments << " segmentów." << std::endl;
    std::cout << "         " << estimatedRowCount << " a tyle przybliżano" << std::endl;

    estimatedRowCount = actualRowCount;
    inputFinished = true;
}