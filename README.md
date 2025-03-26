# Sorting Algorithm Visualizer

## Description
The solver applies logical techniques that a person would use when solving a Sudoku puzzle step by step. 
Each move is recorded in a structured format, allowing detailed analysis of the solving process. Tested using a Sudoku dataset containing 9 million puzzles: [Kaggle Dataset](https://www.kaggle.com/datasets/rohanrao/sudoku)

## Features
- **Human-like Solving Approach**: Implements logical techniques used by human solvers rather than brute-force algorithms.
- **Step-by-step Solve History**: Records each move in a structured format for debugging and analysis.
- **Possibility Marking**: Identifies and marks possible values for each empty cell.
- **Obvious Move Detection**: Applies simple techniques to determine the next valid number.
- **Advanced Move Detection**: Applies more advanced techniques as searching for relationships between pairs and backtrack solving as a last resort
- **Input & Output**: Supports two formats for reading and printing Sudoku puzzles.
  - ```
    parseDigitOneLineStringToMatrix(090038007140570082807146359209800763004090008080002104430905870900780045578401936)`
  - ```
    parseStringToMatrix( ---4---91
                         --7--9-86
                         ---7-----
                         -1-87--6-
                         79-----28
                         -8--23-5-
                         -----7---
                         82-1--9--
                         17---6---)
   - ```
       23|  3| 23|   |  3| 2 | 23|   |
       56| 56| 56| 4 | 56| 5 | 5 | 9 | 1
         |   | 8 | ^ | 8 | 8 |7  | ^ | ^
      ---+---+---+---+---+---+---+---+---
       23|  3|   | 23|1 3|   | 23|   |
      45 |45 | 7 | 5 | 5 | 9 |45 | 8 | 6
         |   | ^ |   |   | ^ |   | ^ | ^
      ---+---+---+---+---+---+---+---+---
       23|  3|123|   |1 3|12 | 23|  3| 23
      456|456|456| 7 | 56| 5 |45 |4  |45
        9|   | 89| ^ | 8 | 8 |   |   |
      ---+---+---+---+---+---+---+---+---
       23|   | 23|   |   |   |  3|   |  3
      45 | 1 |45 | 8 | 7 |45 |4  | 6 |4
         | ^ |   | ^ | ^ |   |   | ^ |  9
      ---+---+---+---+---+---+---+---+---
         |   |  3|   |1  |1  |1 3|   |
       7 | 9 |456| 56|456|45 |4  | 2 | 8
       ^ | ^ |   |   |   |   |   | ^ | ^
      ---+---+---+---+---+---+---+---+---
         |   |   |   |   |   |1  |   |
      4 6| 8 |4 6|  6| 2 | 3 |4  | 5 |4
         | ^ |   |  9| ^ | ^ |7  | ^ |7 9
      ---+---+---+---+---+---+---+---+---
        3|  3|  3| 23|  3|   |123|1 3| 23
      456|456|456| 5 |45 | 7 |456|4  |45
        9|   |  9|  9| 89| ^ | 8 |   |
      ---+---+---+---+---+---+---+---+---
         |   |  3|   |  3|   |   |  3|  3
       8 | 2 |456| 1 |45 |45 | 9 |4  |45
       ^ | ^ |   | ^ |   |   | ^ |7  |7
      ---+---+---+---+---+---+---+---+---
         |   |  3| 23|  3|   | 23|  3| 23
       1 | 7 |45 | 5 |45 | 6 |45 |4  |45
       ^ | ^ |  9|  9| 89| ^ | 8 |   |
      ---+---+---+---+---+---+---+---+---
      1:5 2:6 3:8 4:8 5:8 6:6 7:3 8:4 9:5

## How It Works

The solver follows a series of logical steps to solve the puzzle:
1. Parsing the Input
   - The Sudoku grid is read from a string format and converted into a matrix representation.
   - Each cell is analyzed for existing numbers and possible candidates.

2. Marking Possibilities
   - Each empty cell is assigned a set of possible numbers based on row, column, and 3x3 box constraints.
   - The function `markPossibilities()` ensures each cell gets updated possibilities.

3. Obvious Move Execution 
   - The function `tryObviousMoves()` repeatedly checks for possible placements until no further progress can be made.
     - If a call has only one possible it is placed immediately.
     - The function `trySinglePossibilities()` finds such unique candidates.
     - If a number can only fit into one cell within a row, column, or box, it is placed immediately.
     - The function `checkSingleInstances()` finds such unique candidates.

4. Searching for Relationships Between Pairs
   - After no obvious move was made we start to search for relationships between pairs within groups
   - It detects cases where two possible numbers appear only in two specific cells within the group,
     and if such a relationship is found, it removes other possible numbers from these cells and excludes the two numbers from other cells in the group.
   
5. Backtrack Solving
   - If the puzzle was not solved using the previous strategies, backtracking is employed as a last resort to systematically test different possible numbers for empty cells.

6. Recording Solve History
   - Each move is logged using the format:
     - Inster move
       ```sh
       moveID:grupID>number<column:row
     - Marking relationships between pairs
       ```sh
       RBP:grupID>possibilities<column1:row1&column2:row2
     - restoring state while Backtrack Solving
       ```sh
       R:>possibilities<column1:row1
   - This allows step-by-step debugging and visualization of the solving process.


## Example Solve History

A solve history entry might look like:
```
SP:R>2<8:8 SI:R>5<3:8 SP:S>5<0:0 RBP:S{37}6:7&8:7 R:>{7}< 6:0 ...
```
Where each entry means:
- `SP:R>2<8:8` → Single Possibility whlie working on a Row Group: Placed number 2 in column 8, row 8.
- `SI:R>5<3:8` → Single Instances in a Row Group: Placed number 5 in column 3, row 8.
- `SP:S>5<0:0` → Single Possibility whlie working on a Square Group: Placed number 5 in column 0, row 0.
- `RBP:S{37}6:7&8:7` → Relationships Between Pairs in a Square Group: Only the numbers 3 and 7 are possible in column 6, row 7, and column 8, row 7. These numbers are marked as not possible in other cells within the group.
- `R:>{7}< 6:0` → Reset the puzzle state to the point when the cell in column 6, row 0 was last edited. The possible numbers are now: 7.

## Running the Solver

 - Provide a Sudoku puzzle as an input string in a txt file. `solveSudoku("input.txt")`
 - Compile the program using a C++ compiler.
 - Observe the output, including the solved grid and the solving history.
