/*************************************
* Tanner Wagner
* Professor Haugh
* CS 241
* Group 3RW
*
* This file is used to store all code and
* functions for sudoku.c. To use it,
* simply compile it using the ansi, Wall,
* and pedantic flags. You can then run
* the executable and feed it input using the
* standard redirection operators <>.
*
*************************************/

#include <stdio.h>
#include <string.h>
#define SIZE 9
#define DONE FALSE
#define TRUE 1
#define FALSE 0

/*************************************
 * Note: I will be putting comment
 * blocks for each of the functions 
 * below this section rather then
 * including them all right here along
 * with the templates. That being said
 * thes functions here are simply templates
 * that I decided to use purely as a way
 * of organization for myself. Their only
 * functionality is to tell the compiler
 * that it's ok that these functions exist
 * out of order because their templates
 * exist here.
*************************************/

int readSudoku(int sudokuPuzzle[SIZE][SIZE]);
int findRowErrors(int sudokuPuzzle[SIZE][SIZE]);
int findColErrors(int sudokuPuzzle[SIZE][SIZE]);
int findBoxErrors(int sudokuPuzzle[SIZE][SIZE]);
int solveSudoku(int sudokuPuzzle[SIZE][SIZE], int puzzleIndex);
int isSafe(int sudokuPuzzle[SIZE][SIZE], int puzzleIndex, int puzzleNum);
int noSolution(int sudokuPuzzle[SIZE][SIZE]);
void writePuzzle(int sudokuPuzzle[SIZE][SIZE]);

/*************************************
* Each parameter’s type and name: None
* input and/or output: 
  * Input: Reads Sudoku puzzles from standard input.
  * Output: Writes to standard output - either the solved Sudoku puzzles or error messages.
* its meaning: This function does not take any parameters, but it deals with Sudoku puzzles.
* its range of values: Not applicable as there are no parameters.
* Function’s return value: int (0 after all puzzles have been processed).
* Description of what the function does: 
  * Orchestrates the entire process of reading, validating, and solving Sudoku puzzles. It continues reading and processing puzzles until EOF is encountered.
* Function’s algorithm:
  * 1. Reads a Sudoku puzzle from standard input.
  * 2. Validates the Sudoku puzzle for any row, column, or 3x3 box errors.
  * 3. If errors are present, prints an error message.
  * 4. If no errors are found, attempts to solve the puzzle.
  * 5. Checks if the puzzle has a solution.
  * 6. If a solution exists, prints the solved puzzle.
*************************************/

int main()
{
  int sudokuPuzzle[SIZE][SIZE];
  int puzzleIndex = 0;
  int myError;

  while ((myError = readSudoku(sudokuPuzzle)) != EOF)
  {
    myError += findRowErrors(sudokuPuzzle);
    myError += findColErrors(sudokuPuzzle);
    myError += findBoxErrors(sudokuPuzzle);

    if (myError)
    {
      printf("Error\n\n");
    }
    
    else
    {
      if (DONE)	writePuzzle(sudokuPuzzle);
      solveSudoku(sudokuPuzzle, puzzleIndex);
      if (!noSolution(sudokuPuzzle)) writePuzzle(sudokuPuzzle);
    }
  }
  
  return 0;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input/output, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Reads characters representing Sudoku numbers or empty cells from standard input.
  * Output: Fills the sudokuPuzzle array with integers representing the Sudoku puzzle.
* Its meaning:
  * sudokuPuzzle is used to store the Sudoku grid that is being read from input. Each element corresponds to a cell in the Sudoku puzzle.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * Returns EOF if the end of the file is encountered; otherwise, returns TRUE if there is an input format error and FALSE if not.
* Description of what the function does:
  * Reads characters representing a Sudoku puzzle from standard input and populates the sudokuPuzzle array based on the characters read. Checks for input format errors.
* Function’s algorithm:
  * 1. Loop through input characters until a newline or EOF.
  * 2. Check if each character is a valid Sudoku digit or a placeholder ('.').
  * 3. Convert valid digits or placeholders into integers and store them in sudokuPuzzle.
  * 4. Return EOF if EOF is encountered, or return a boolean indicating whether there was an error in the input.
*************************************/

int readSudoku(int sudokuPuzzle[SIZE][SIZE])
{
  int i;
  int puzzleNum;
  int puzzleRow;
  int puzzleCol;
  int myError = FALSE;

  for (i = 0; (puzzleNum = getchar()) != '\n'; i++)
  {
    if (puzzleNum == EOF)
    {
      return EOF;
    }
    
    putchar(puzzleNum);
    
    if ((puzzleNum < '1' || puzzleNum > '9') && (puzzleNum != '.'))
    {
      myError = TRUE;
    }
    
    if (puzzleNum == '.')
    {
      puzzleNum = '0';
    }
    
    puzzleRow = (i / 9) % 9;
    puzzleCol = i % 9;
    sudokuPuzzle[puzzleRow][puzzleCol] = puzzleNum - '0';
  }

  putchar('\n');

  if (i != 81)
  {
    myError = TRUE;
  }
  
  return myError;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Uses the sudokuPuzzle array to check for row errors.
  * Output: None directly, but affects the return value based on found errors.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle being checked for row-wise errors.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * Returns TRUE if any row errors (duplicates of numbers 1 through 9) are found, otherwise returns FALSE.
* Description of what the function does:
  * Checks each row in the Sudoku puzzle to determine if any row contains duplicate numbers, which would violate Sudoku rules.
* Function’s algorithm:
  * 1. Loop through each row in the sudokuPuzzle.
  * 2. Within each row, compare each cell with every other cell in the same row.
  * 3. If a duplicate number (other than zero) is found within the same row, return TRUE indicating an error.
  * 4. If no duplicates are found after checking all rows, return FALSE.
*************************************/

int findRowErrors(int sudokuPuzzle[SIZE][SIZE])
{
  int puzzleRow;
  int puzzleCol;
  int i;

  for (puzzleRow = 0; puzzleRow < 9; puzzleRow++)
  {
    for (puzzleCol = 0; puzzleCol < 9; puzzleCol++)
    {
      for (i = puzzleCol + 1; i < 9; i++)
      {
	if ((sudokuPuzzle[puzzleRow][puzzleCol] != 0) &&
	    (sudokuPuzzle[puzzleRow][puzzleCol] == sudokuPuzzle[puzzleRow][i]))
	{
	  return TRUE;
	}
      }
    }
  }

  return FALSE;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Uses the sudokuPuzzle array to check for column errors.
  * Output: None directly, but affects the return value based on found errors.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle being checked for column-wise errors.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * Returns TRUE if any column errors (duplicates of numbers 1 through 9) are found, otherwise returns FALSE.
* Description of what the function does:
  * Checks each column in the Sudoku puzzle to determine if any column contains duplicate numbers, which would violate Sudoku rules.
* Function’s algorithm:
  * 1. Loop through each column in the sudokuPuzzle.
  * 2. Within each column, compare each cell with every other cell in the same column.
  * 3. If a duplicate number (other than zero) is found within the same column, return TRUE indicating an error.
  * 4. If no duplicates are found after checking all columns, return FALSE.
*************************************/

int findColErrors(int sudokuPuzzle[SIZE][SIZE])
{
  int puzzleRow;
  int puzzleCol;
  int i;

  for (puzzleCol = 0; puzzleCol < 9; puzzleCol++)
  {
    for (puzzleRow = 0; puzzleRow < 9; puzzleRow++)
    {
      for (i = puzzleRow + 1; i < 9; i++)
      {
	if ((sudokuPuzzle[puzzleRow][puzzleCol] != 0) &&
	    (sudokuPuzzle[puzzleRow][puzzleCol] == sudokuPuzzle[i][puzzleCol]))
	{
	  return TRUE;
	}
      }
    }
  }

  return FALSE;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Uses the sudokuPuzzle array to check for 3x3 box errors.
  * Output: None directly, but affects the return value based on found errors.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle being checked for errors within each 3x3 box.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * Returns TRUE if any 3x3 box errors (duplicates of numbers 1 through 9) are found, otherwise returns FALSE.
* Description of what the function does:
  * Checks each 3x3 box in the Sudoku puzzle to determine if any box contains duplicate numbers, which would violate Sudoku rules.
* Function’s algorithm:
  * 1. Loop through the Sudoku grid by 3x3 boxes (top-left corner of each box starts at rows 0, 3, 6 and columns 0, 3, 6).
  * 2. Within each 3x3 box, compare each cell with every other cell in the same box.
  * 3. If a duplicate number (other than zero) is found within the same box, return TRUE indicating an error.
  * 4. If no duplicates are found after checking all 3x3 boxes, return FALSE.
*************************************/

int findBoxErrors(int sudokuPuzzle[SIZE][SIZE])
{
  int puzzleRow;
  int puzzleCol;
  int i;
  int j;

  for (puzzleRow = 0; puzzleRow < 9; puzzleRow += 3)
  {
    for (puzzleCol = 0; puzzleCol < 9; puzzleCol += 3)
    {
      for (i = 0; i < 9; i++)
      {
	for (j = i + 1; j < 9; j++)
	{
	  if ((sudokuPuzzle[puzzleRow + i / 3][puzzleCol + i % 3] != 0) &&
	      (sudokuPuzzle[puzzleRow + i / 3][puzzleCol + i % 3] ==
	       sudokuPuzzle[puzzleRow + j / 3][puzzleCol + j % 3]))
	    {
	      return TRUE;
	    }
	}
      }
    }
  }
  
  return FALSE;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Uses the sudokuPuzzle array to determine if the puzzle is unsolvable.
  * Output: Prints a message to standard output if no solution is found.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle being checked for solvability.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * Returns TRUE if the Sudoku puzzle is unsolvable (contains empty cells after attempting to solve), otherwise FALSE.
* Description of what the function does:
  * Checks if the Sudoku puzzle is filled completely, which indicates whether a solution exists or not.
* Function’s algorithm:
  * 1. Loop through each cell in the sudokuPuzzle.
  * 2. Check if any cell in the puzzle is still empty (contains a 0).
  * 3. If an empty cell is found, print "No solution" and return TRUE, indicating that the puzzle is unsolvable.
  * 4. If no empty cells are found (all cells contain numbers 1 through 9), return FALSE, indicating that a solution might exist.
*************************************/

int noSolution(int sudokuPuzzle[SIZE][SIZE])
{
  int puzzleRow;
  int puzzleCol;

  for (puzzleRow = 0; puzzleRow < 9; puzzleRow++)
  {
    for (puzzleCol = 0; puzzleCol < 9; puzzleCol++)
    {
      if (!sudokuPuzzle[puzzleRow][puzzleCol])
      {
	printf("No solution\n\n");
	return TRUE;
      }
    }
  }

  return FALSE;
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
* Input and/or output:
  * Input: Takes the sudokuPuzzle array containing the state of the Sudoku puzzle.
  * Output: Prints the Sudoku puzzle to standard output, formatted for readability.
* Its meaning:
  * sudokuPuzzle represents the state of the Sudoku puzzle to be printed.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
* Function’s return value:
  * void (does not return a value).
* Description of what the function does:
  * Prints the current state of the Sudoku puzzle in a readable format, including grid lines after every third line to mimic Sudoku's 3x3 box structure.
* Function’s algorithm:
  * 1. Loop through each row and column of the sudokuPuzzle.
  * 2. Print a newline character to separate each row, and additional grid lines after every third row.
  * 3. For each cell, print the number if it is not zero; print a dot (.) if it is zero.
  * 4. After every third column, print a vertical separator to mimic the 3x3 box structure of Sudoku.
*************************************/

void writePuzzle(int sudokuPuzzle[SIZE][SIZE])
{
  int puzzleRow;
  int puzzleCol;

  for (puzzleRow = 0; puzzleRow < 9; puzzleRow++)
  {
    if (DONE)
    {
      printf("\n");
    }
    
    if ((DONE) && (puzzleRow == 3 || puzzleRow == 6))
    {
      printf("----------------------\n");
    }

    for (puzzleCol = 0; puzzleCol < 9; puzzleCol++)
    {
      if (DONE)
      {
	printf(" ");
      }
      
      if (sudokuPuzzle[puzzleRow][puzzleCol])
      {
	printf("%d", sudokuPuzzle[puzzleRow][puzzleCol]);
      }
      
      else
      {
	printf(".");
      }
      
      if ((DONE) && (puzzleCol == 2 || puzzleCol == 5))
      {
	printf(" |");
      }
    }
  }

  printf("\n\n");
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input/output, a two-dimensional array representing the Sudoku puzzle.
  * int puzzleIndex: input, an integer representing the current index in the puzzle being solved.
* Input and/or output:
  * Input: The current state of the Sudoku puzzle and the current index to be solved.
  * Output: Modifies the sudokuPuzzle array as it attempts to solve the puzzle.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle.
  * puzzleIndex is used to determine the current row and column in the puzzle being solved.
* Its range of values:
  * sudokuPuzzle elements can range from 0 (representing an empty cell) to 9 (representing the numbers in the Sudoku puzzle).
  * puzzleIndex ranges from 0 to 80, representing each cell's index in the 9x9 Sudoku grid.
* Function’s return value:
  * Returns TRUE if the puzzle can be solved from the current index onwards, otherwise FALSE.
* Description of what the function does:
  * Attempts to solve the Sudoku puzzle using a backtracking algorithm. It fills the puzzle by placing valid numbers into empty cells and recursively checking if the puzzle can be solved with these numbers.
* Function’s algorithm:
  * 1. Check if puzzleIndex equals 81 (indicating the end of the puzzle) and return TRUE.
  * 2. If the current cell is not empty, recursively call solvePuzzle for the next cell.
  * 3. If the current cell is empty, try each number from 1 to 9:
    * a. Check if placing the current number in the current cell is safe using isSafe.
    * b. If safe, place the number and recursively call solvePuzzle for the next cell.
    * c. If the recursive call returns TRUE, return TRUE (solution found).
    * d. If the recursive call returns FALSE, reset the cell (backtrack) and try the next number.
  * 4. If no number can lead to a solution, return FALSE (no solution from this configuration).
*************************************/

int solveSudoku(int sudokuPuzzle[SIZE][SIZE], int puzzleIndex)
{
  int puzzleNum;
  int puzzleRow = puzzleIndex / 9;
  int puzzleCol = puzzleIndex % 9;

  if (puzzleIndex == 81)
  {
    return TRUE;
  }
  
  if (sudokuPuzzle[puzzleRow][puzzleCol] != 0)
  {
    return solveSudoku(sudokuPuzzle, ++puzzleIndex);
  }
  
  else
  {
    for (puzzleNum = 1; puzzleNum <= 9; puzzleNum++)
    {
      if (isSafe(sudokuPuzzle, puzzleIndex, puzzleNum))
      {
	sudokuPuzzle[puzzleRow][puzzleCol] = puzzleNum;
	if (solveSudoku(sudokuPuzzle, puzzleIndex))
	{
	  return TRUE;
	}
	sudokuPuzzle[puzzleRow][puzzleCol] = 0;
      }
    }

    return FALSE;
  }
}

/*************************************
* Each parameter’s type and name:
  * int sudokuPuzzle[SIZE][SIZE]: input, a two-dimensional array representing the Sudoku puzzle.
  * int puzzleIndex: input, an integer representing the current index in the puzzle.
  * int puzzleNum: input, an integer representing the number to check for safety.
* Input and/or output:
  * Input: The current state of the Sudoku puzzle, the index of the current cell, and the number to be placed.
  * Output: None directly, but affects the return value based on the safety check.
* Its meaning:
  * sudokuPuzzle represents the current state of the Sudoku puzzle.
  * puzzleIndex determines the current cell being checked.
  * puzzleNum is the number being tested for placement in the puzzle.
* Its range of values:
  * sudokuPuzzle elements can range from 0 to 9.
  * puzzleIndex ranges from 0 to 80.
  * puzzleNum ranges from 1 to 9.
* Function’s return value:
  * Returns TRUE if it is safe to place puzzleNum at the position indicated by puzzleIndex, otherwise FALSE.
* Description of what the function does:
  * Checks if it is safe to place a given number in a specified cell of the Sudoku puzzle without violating Sudoku rules (no same numbers in a row, column, or 3x3 subgrid).
* Function’s algorithm:
  * 1. Calculate the row and column indices from puzzleIndex.
  * 2. Check all cells in the same row; if puzzleNum is found, return FALSE.
  * 3. Check all cells in the same column; if puzzleNum is found, return FALSE.
  * 4. Determine the starting indices of the 3x3 box containing the cell, then check all cells in this box; if puzzleNum is found, return FALSE.
  * 5. If no conflicts are found in the row, column, and box, return TRUE (safe to place the number).
*************************************/

int isSafe(int sudokuPuzzle[SIZE][SIZE], int puzzleIndex, int puzzleNum)
{
  int i;
  int j;
  int puzzleRow = puzzleIndex / 9;
  int puzzleCol = puzzleIndex % 9;
  int boxStartRow = puzzleRow - (puzzleRow % 3);
  int boxStartCol = puzzleCol - (puzzleCol % 3);

  for (i = 0; i < 9; i++)
  {
    if (sudokuPuzzle[puzzleRow][i] == puzzleNum)
    {
      return FALSE;
    }
  }

  for (i = 0; i < 9; i++)
  {
    if (sudokuPuzzle[i][puzzleCol] == puzzleNum)
    {
      return FALSE;
    }
  }

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (sudokuPuzzle[boxStartRow + i][boxStartCol + j] == puzzleNum)
      {
	return FALSE;
      }
    }
  }

  return TRUE;
}
