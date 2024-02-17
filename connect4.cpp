#include <iostream>
#include <iomanip>
#include <cstring>
#include "c4lib.h"
using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 4){
    cout << "Please provide the board dimensions, either 0P|1P|2P, and an optional seed" << endl;
    return 1;
  }
  int ydim, xdim, seed;
  BoardValue** board = NULL;

  ydim = atoi(argv[1]);
  xdim = atoi(argv[2]);

  if(argc >= 5)
    seed = atoi(argv[4]);
	else 
		seed = time(0);

  srand(seed);


  // Do not alter - determine the player modes
  int numP = 2;
  if(strcmp(argv[3],"1P") == 0 || strcmp(argv[3], "1p") == 0){
    numP = 1;
  }
  else if(strcmp(argv[3],"0P") == 0 || strcmp(argv[3], "0p") == 0){ 
    numP = 0;
  }
  else if(strcmp(argv[3],"test") == 0){ 
    numP = -1;
  }
  cout << "Num players: " << numP << endl;

  // Any initialization code
  board = allocateBoard(ydim,xdim);

  // Do not alter these declarations 
  int turn = 0;        // Number of turns for the game 
  int player = 0;      // MUST alternate between 0 (red) and 1 (yellow) 
                       // to represent the two players
  bool error = false;  // input error?
  enum gameState {COMPLETE, INPROGRESS};
  enum winState {WINRED, WINYELLOW, DRAW, UNDECIDED};
  //takes in player number and turns it to the winner
  enum winState playerToWinState[3] = {WINRED, WINYELLOW, DRAW};

  gameState gamestate = INPROGRESS;
  winState gamewin = UNDECIDED;
  
  while(gamestate){
    // ------------------------------------------------------------
    // Do not alter
    
    // Increment the turn and print the board
    turn++;
    printBoard(board, ydim, xdim);
    // Get the input, check for errors, a winner or a draw
    int x, y;
    if(numP == -1) {
      // Random vs. Random
      error = getRandomAIInput(board, ydim, xdim, &y, &x, player);
    }
    else if(numP == 0 && player == 0){
      // Random vs. User AI
      error = getRandomAIInput(board, ydim, xdim, &y, &x, player);
    }
    else if( (numP == 0 || numP == 1) && player == 1 ){
      // Random vs. User AI ..or.. Human vs. User AI
      error = getUserAIInput(board, ydim, xdim, &y, &x, player);
    }
    else {
      // Human vs. user AI ..or.. Human vs. Human
      error = getNextHumanInput(board, ydim, xdim, &y, &x, player);
    }
    // ------------------------------------------------------------

    // Add necessary code to deal with errors and determine if 
    // there is a winner or a draw. Then change to the next player
    // and repeat

    if(!error){
      if (hasWon(board, ydim, xdim, y, x, player)){
        gamewin = playerToWinState[player];
        gamestate = COMPLETE;
      }
      else if (isDraw(board, ydim, xdim)){
        //Draw
        gamewin = playerToWinState[2];
        gamestate = COMPLETE;
      }
    }
    else{
      //there is an error
      gamestate = COMPLETE;
    }
    player = (player+1)%2;
  }


  // Only fill in the conditions of the code below.  
	//  This code will output the expected messages but you
	//  have to complete the conditions based on your 
	//  implementation above.
  
  if(gamewin == WINYELLOW) {
    cout << "Yellow wins" <<  endl;
  }
  else if(gamewin == WINRED) {
    cout <<  "Red wins" <<  endl;
  }
  else if(gamewin == DRAW) {
    cout <<  "Draw" <<  endl;
  }
  else if(error == true) {
    cout <<  "Early exit" <<  endl;
  }
  cout <<  "Last turn " << turn <<  endl;

  deallocateBoard(board, ydim);
  return 0;
}