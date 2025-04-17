#include "helpers.h"

int main() {
    //testInputs();
    testOnDataSetV2("sudoku.csv", 6);
    std::cout << "returned\n";
    return 0;
    //return testOnDataSet(6);
    return solveSudoku("inputHard.txt"); //inputHard.txt inputNaj.txt
}
