#include "c4lib.h"
#include <iostream>
using namespace std;

// Prototype internal helper functions
/**
 * @brief returns the smallest y coordinate that is BLANK in 
 *        column x or -1 if no location is BLANK
 */
int findYValue(BoardValue** board, int ydim, int x);


// ------------------------------------------------------------
// Global variabes - the ONLY we allow in this program

// converts integer player value: 0 or 1 to appropriate enum
enum BoardValue playerToValue[2] = {RED, YELLOW};

// ----------------------------------------------------------------------


// Helper function 
int findYValue(BoardValue** board, int ydim, int x)
{
  // You complete
  //vertical search
  for(int i=0; i<ydim; i++){
    if(board[i][x] == BLANK)
      return i;
  }
  return -1;
}

BoardValue** allocateBoard(int ydim, int xdim)
{
  // You complete
  BoardValue** board = new BoardValue*[ydim];
  for(int i = 0; i< ydim; i++){
    board[i] = new BoardValue[xdim];
    for(int j=0; j<xdim; j++){
      board[i][j] = BLANK;
    }
  }
  return board;
}
void deallocateBoard(BoardValue** board, int ydim)
{
  // You complete
  for(int i=0; i<ydim; i++){
    delete[] board[i];
  }
  delete[] board;
}

void printBoard(BoardValue** board, int ydim, int xdim)
{
  // Complete - Do not alter
  const char* boardToString[] = { "\U000026ab", "\U0001F534", "\U0001F7E1" }; 
                                  //  "⚫",          "🔴",         "🟡"}
  for(int y = ydim-1; y >= 0; y--){
    for(int x = 0; x < xdim; x++) {
      cout << boardToString[(int)board[y][x]] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

bool getNextHumanInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  char input[5];
  cout << "Enter the location: " << "\n";
  cin >> input;

  //If it is an invalid input
  *x = atoi(input);
  if(*x >= xdim || *x < 0)
    return true;

  //Assign Y value
  *y = findYValue(board, ydim, *x);

  //If the column is full
  if(*y == -1 || *y >= ydim)
    return true;
  
  board[*y][*x] = playerToValue[currentPlayer];
  return false;
}

bool hasWon(
  BoardValue** board,
  int ydim, int xdim,
  int sy, int sx,
  int currentPlayer)
{
  // Keep, modify, or delete these as you desire
  const int NDIRS = 4;
  const int xDirDelta[NDIRS] = { 0, +1, -1, +1};
  const int yDirDelta[NDIRS] = {+1,  0, +1, +1};
  // You complete
  for (int dir = 0; dir < NDIRS; dir++) {
    int count = 1; // Start counting from the current piece

    // Check in the positive direction
    for (int i = 1; i < 4; i++) {
      int x = sx + i * xDirDelta[dir];
      int y = sy + i * yDirDelta[dir];
      if (x < 0 || x >= xdim || y < 0 || y >= ydim || board[y][x] != playerToValue[currentPlayer]) {
          break;
      }
      count++;
    }

    // Check in the negative direction
    for (int i = 1; i < 4; i++) {
      int x = sx - i * xDirDelta[dir];
      int y = sy - i * yDirDelta[dir];
      if (x < 0 || x >= xdim || y < 0 || y >= ydim || board[y][x] != playerToValue[currentPlayer]) {
          break;
      }
      count++;
    }
    // Found 4 in a row
    if (count >= 4) {
        return true;
    }
  }
  // No 4 in a row found
  return false;
}

bool isDraw(
  BoardValue** board,
  int ydim, int xdim)
{
  // You complete
  for (int y = 0; y < ydim; ++y) {
      for (int x = 0; x < xdim; ++x) {
        if (board[y][x] == BLANK) {
          // If any cell is empty, the game is not a draw
          return false;
        }
      }
    }
    // If all cells are filled and no winner, it's a draw
    return true;

}



bool getUserAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // Attempt to find a winning move for the AI
  for (int col = 0; col < xdim; ++col) {
    int row = findYValue(board, ydim, col);
    if (row != -1) { // Column is not full
      board[row][col] = playerToValue[currentPlayer]; // Temporarily place the piece
      if (hasWon(board, ydim, xdim, row, col, currentPlayer)) {
        *y = row;
        *x = col;
        // Winning move found
        return false;
      }
      // Undo the move
      board[row][col] = BLANK;
    }
  }

  // Block opponent's winning move
  int opponentPlayer = 1 - currentPlayer;
  for (int col = 0; col < xdim; ++col) {
    int row = findYValue(board, ydim, col);
    // Column is not full
    if (row != -1) {
      // Temporarily place the opponent's piece
      board[row][col] = playerToValue[opponentPlayer];
      if (hasWon(board, ydim, xdim, row, col, opponentPlayer)) {
        // Actually place AI's piece to block
        board[row][col] = playerToValue[currentPlayer];
        *y = row;
        *x = col;
        // Blocked the opponent's winning move
        return false;
      }
      // Undo the move
      board[row][col] = BLANK;
    }
  }

  // No immediate winning or blocking move found, choose a random move as fallback
  return getRandomAIInput(board, ydim, xdim, y, x, currentPlayer);
}

// Complete - Do not alter
bool getRandomAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // Allocate variable size array (we don't know xdim when we compile)
  int* possible = new int[xdim];
  int numX = 0;
  bool error = true;
  // find all the possible columns with a blank
  for(int i = 0; i < xdim; i++){
    if(board[ydim-1][i] == BLANK){
      possible[numX++] = i;
    }
  }
  // If at least 1 column has a blank
  if(numX != 0){
    // Choose one column
    *x = possible[rand()%numX];
    *y = findYValue(board, ydim, *x);
    board[*y][*x] = playerToValue[currentPlayer];
    error = false;  
  }
  delete [] possible;
  return error;
}