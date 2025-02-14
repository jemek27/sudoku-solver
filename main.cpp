#include <iostream>
#include "FileHandler.h"
#include "SudokuSolver.h"
#include "SudokuTable.h"
#include <bitset>
#include <iostream>

//const int SIZE = 9;
//
//int main() {
//
//    std::array<std::bitset<SIZE>, SIZE> x = {
//            0b101000101, // Przykładowe dane
//            0b000101010,
//            0b111000011,
//            0b000111000,
//            0b101010001,
//            0b000101010,
//            0b101000011,
//            0b000111000,
//            0b101010001
//    };
//
//    std::bitset<SIZE> XOR9 =
//            ~((x[0] & x[1]) | (x[0] & x[2]) | (x[0] & x[3]) | (x[0] & x[4]) | (x[0] & x[5]) | (x[0] & x[6]) | (x[0] & x[7]) | (x[0] & x[8]) |
//              (x[1] & x[2]) | (x[1] & x[3]) | (x[1] & x[4]) | (x[1] & x[5]) | (x[1] & x[6]) | (x[1] & x[7]) | (x[1] & x[8]) |
//              (x[2] & x[3]) | (x[2] & x[4]) | (x[2] & x[5]) | (x[2] & x[6]) | (x[2] & x[7]) | (x[2] & x[8]) |
//              (x[3] & x[4]) | (x[3] & x[5]) | (x[3] & x[6]) | (x[3] & x[7]) | (x[3] & x[8]) |
//              (x[4] & x[5]) | (x[4] & x[6]) | (x[4] & x[7]) | (x[4] & x[8]) |
//              (x[5] & x[6]) | (x[5] & x[7]) | (x[5] & x[8]) |
//              (x[6] & x[7]) | (x[6] & x[8]) |
//              (x[7] & x[8]));

//  XOR9 &= x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7] | x[8];

//
//    std::cout << "Wynik: " << XOR9 << std::endl;
//
//    return 0;
//}

//int main() {
//    // Przykładowe 9 bitsetów, każdy o długości 9 bitów
//    std::array<std::bitset<SIZE>, SIZE> rows = {
//            0b101000101, // Przykładowe dane
//            0b000101010,
//            0b111000011,
//            0b000111000,
//            0b101010001,
//            0b000101010,
//            0b101000011,
//            0b000111000,
//            0b101010001
//    };
//
//    // Sprawdzenie, czy istnieje kolumna z dokładnie jedną jedynką
//    bool has_single_one = false;
//
//    for (int col = 0; col < SIZE; ++col) {
//        std::bitset<SIZE> column_mask(1 << col); // Maska dla kolumny
//        std::bitset<SIZE> column_bits; // Zbiorczy bitset dla kolumny
//
//        // Zbieranie jedynek w kolumnie
//        for (int row = 0; row < SIZE; ++row) {
//            if ((rows[row] & column_mask).any()) {
//                column_bits.set(row); // Ustawienie bitu w zbiorczym bitsetcie
//            }
//        }
//
//        // Sprawdzenie, czy w kolumnie jest dokładnie jedna jedynka
//        if (column_bits.count() == 1) {
//            has_single_one = true;
//            std::cout << "Kolumna " << col << " ma dokładnie jedną jedynkę.\n";
//        }
//    }
//
//    if (!has_single_one) {
//        std::cout << "Żadna kolumna nie ma dokładnie jednej jedynki.\n";
//    }
//
//    return 0;
//}
int main() {
    const int SIZE = 9;
    std::array<std::bitset<SIZE>, SIZE> x = {
            0b101000001, // Przykładowe dane
            0b000101010,
            0b111000111,
            0b000111000,
            0b101010001,
            0b000101010,
            0b101000011,
            0b000111000,
            0b101010001
    };

    std::bitset<SIZE> XOR9 =
            ~((x[0] & x[1]) | (x[0] & x[2]) | (x[0] & x[3]) | (x[0] & x[4]) | (x[0] & x[5]) | (x[0] & x[6]) | (x[0] & x[7]) | (x[0] & x[8]) |
              (x[1] & x[2]) | (x[1] & x[3]) | (x[1] & x[4]) | (x[1] & x[5]) | (x[1] & x[6]) | (x[1] & x[7]) | (x[1] & x[8]) |
              (x[2] & x[3]) | (x[2] & x[4]) | (x[2] & x[5]) | (x[2] & x[6]) | (x[2] & x[7]) | (x[2] & x[8]) |
              (x[3] & x[4]) | (x[3] & x[5]) | (x[3] & x[6]) | (x[3] & x[7]) | (x[3] & x[8]) |
              (x[4] & x[5]) | (x[4] & x[6]) | (x[4] & x[7]) | (x[4] & x[8]) |
              (x[5] & x[6]) | (x[5] & x[7]) | (x[5] & x[8]) |
              (x[6] & x[7]) | (x[6] & x[8]) |
              (x[7] & x[8]));

    XOR9 &= x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7] | x[8];

    std::cout << "Wynik: " << XOR9 << std::endl;

    auto fileHandler = new FileHandler("inputHard.txt");
    auto sudokuSolver = SudokuSolver();

    std::cout << "Hello, World!" << std::endl;

    std::string input {};
    if (!fileHandler->readFromFile(input)) {
        std::cout << "No input data!" << std::endl;
        return 0;
    }
    std::cout << input << std::endl;

    sudokuSolver.parseStringToMatrix(input);
    sudokuSolver.printTable();
    sudokuSolver.printTableWithPossibilities();
    sudokuSolver.markPossibilities();
    std::cout << "\n-------------------------------\n";
    sudokuSolver.printTableWithPossibilities();

    delete fileHandler;
    return 0;
}
