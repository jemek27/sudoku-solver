# Sorting Algorithm Visualizer

## Description
The solver applies logical techniques that a person would use when solving a Sudoku puzzle step by step. 
Each move is recorded in a structured format, allowing detailed analysis of the solving process.

## Features
- **Human-like Solving Approach**: Implements logical techniques used by human solvers rather than brute-force algorithms.
- **Step-by-step Solve History**: Records each move in a structured format for debugging and analysis.
- **Possibility Marking**: Identifies and marks possible values for each empty cell.
- **Obvious Move Detection**: Applies simple techniques to determine the next valid number.
- **Advanced Move Detection**: Applies more advanced techniques as searching for relationships between pairs and backtrack solving as a last resort
- **Input & Output**: Supports two formats for reading and printing Sudoku puzzles.

## How It Works

The solver follows a series of logical steps to solve the puzzle:
1. Parsing the Input
   - The Sudoku grid is read from a string format and converted into a matrix representation.
   - Each cell is analyzed for existing numbers and possible candidates.

2. Marking Possibilities
   - Each empty cell is assigned a set of possible numbers based on row, column, and 3x3 box constraints.
   - The function markPossibilities() ensures each cell gets updated possibilities.

3. Obvious Move Execution 
   - The function tryObviousMoves() repeatedly checks for possible placements until no further progress can be made.
     - If a call has only one possible it is placed immediately.
     - The function trySinglePossibilities() finds such unique candidates.
     - If a number can only fit into one cell within a row, column, or box, it is placed immediately.
     - The function checkSingleInstances() finds such unique candidates.

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
       moveID:grupID > number < column:row
     - Marking relationships between pairs
       ```sh
       RBP:grupID > possibilities < column1:row1 column2:row2
     - restoring state while Backtrack Solving
       ```sh
       R: > possibilities < column1:row1
   - This allows step-by-step debugging and visualization of the solving process.

