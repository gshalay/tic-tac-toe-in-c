/* File Name: ttt2.c
 * File Description: Contains helper functions to create and manage progress and results of tic-tac-toe games.
 * Student Name: Greg Shalay
 * UoG: 1096689
 */

#include "ttt.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define WIN_CONDITIONS 7 // Really 8, but 7 accounts for 0-based nature of arrays.
#define FORK_CONDITIONS 44
#define MAX_POSITIONS 8
#define FULL_BOARD 9 // Depth value of a full board.

#define CENTER_SQUARE 4
#define TOP_LEFT_SQUARE 0
#define TOP_RIGHT_SQUARE 2
#define BOTTOM_LEFT_SQUARE 6
#define BOTTOM_RIGHT_SQUARE 8

#define X_TURN 0
#define O_TURN 1
#define BOARD_STRING_SIZE 31

//Set the init variable in each data structure to 0 to indicate that the element is empty.
void init_boards(){
  for(int i = 0; i < HSIZE; i++){
    htable[i].init = '0';
  }
}

// Loops through the board string and counts the number of 'X' and 'O' markers and returns
// the total number of markers.
int depth(Board board){
  int numMarkers = 0;
  
  for(; *board; board++){
    if(*board == 'X' || *board == 'O'){
      numMarkers++;
    }
  }

  return numMarkers;
}

// Determines the winnder of the game based on the current stare of the game board. 
// Winner is indeterminate if the board provided is in progress. 
char winner(Board board){
  char gameOutcome = '?';
  
  for(int i = 0; i <= WIN_CONDITIONS; i++){
    if(board[pos2idx[WINS[i][0]]] == 'X' && board[pos2idx[WINS[i][1]]] == 'X' && board[pos2idx[WINS[i][2]]] == 'X'){
      gameOutcome = 'X';
      return gameOutcome;
    }
    else if(board[pos2idx[WINS[i][0]]] == 'O' && board[pos2idx[WINS[i][1]]] == 'O' && board[pos2idx[WINS[i][2]]] == 'O'){
      gameOutcome = 'O';
      return gameOutcome;
    }
  }
  
  if(gameOutcome == '?'){ // If no winner found...
    if(depth(board) == FULL_BOARD){ // ...Check for a tie game.
      gameOutcome = '-';
      return gameOutcome;
    }
  }
  
  // Return the outcome of the winner, a draw, or unknown.
  return gameOutcome;
}

// Determines the player turn based on modulo integer division.
// X always goes first.
char turn(Board board){
  int turn = depth(board) % 2;
  
  if(depth(board) == FULL_BOARD || winner(board) != '?'){
    return '-';
  }
  
  if(turn == X_TURN){
    return 'X';
  }
  else if(turn == O_TURN){
    return 'O';
  }
  else{
    fprintf(stderr, "Problem determining player turn.\n");
    exit(EXIT_FAILURE);
  }
}

// Sets some initial values for the appropriate boardNode in the htable.
// Does not set the score variable.
void init_board(Board board){
  int boardHash = board_hash(board);
  
  htable[boardHash].init = '1';
  htable[boardHash].turn = turn(board);
  htable[boardHash].depth = depth(board); 
  htable[boardHash].winner = winner(board); 
  
  strcpy(htable[boardHash].board, board);
}

// Implements a Directed Acyclic Graph (DAG) that connects and joins different game turn outcomes in a linear fashion.
// Douplicate game outcomes are ommitted if already present in the current graph.
void join_graph(Board board){

  int tableIndex = board_hash(board);
  int newTableIndex;
  
  Board newBoard;
  
  //Loop over all possible move positions.
  for(int i = 0; i <= MAX_POSITIONS; i++){
    int tttPos = board[pos2idx[i]];
    
    if(isdigit(tttPos)){ // Then there isn't a piece in this position.
      strcpy(newBoard, htable[tableIndex].board);
      newBoard[pos2idx[i]] = turn(board);
      
      newTableIndex = board_hash(newBoard);
      
      htable[tableIndex].move[i] = newTableIndex;
      
      //Check if a entry for that boardHash index in htable already exists. init.
      if(htable[newTableIndex].init == '0'){
        init_board(newBoard);
        join_graph(newBoard);
      }
      
      //Otherwise, just continue on.
      
    }
    else{  // Then this position already has a piece in it.
      htable[tableIndex].move[i] = -1;
    }
  }
}

// Determines if the player provided is able to win this turn. Returns true if the player can win or false if they cannot.
bool isPossibleToWinThisTurn(Board board, char player, int * vacantSpace){
  for(int i = 0; i <= WIN_CONDITIONS; i++){
    if(board[pos2idx[WINS[i][0]]] == player && board[pos2idx[WINS[i][1]]] == player && isdigit( board[ pos2idx[ WINS[i][2] ] ] )){
      *vacantSpace = WINS[i][2];
      return true;
    }
    else if(board[pos2idx[WINS[i][0]]] == player && board[pos2idx[WINS[i][2]]] == player && isdigit( board[ pos2idx[ WINS[i][1] ] ] )){
      *vacantSpace = WINS[i][1];
      return true;
    }
    else if(board[pos2idx[WINS[i][1]]] == player && board[pos2idx[WINS[i][2]]] == player && isdigit( board[ pos2idx[ WINS[i][0] ] ] )){
      *vacantSpace = WINS[i][0];
      return true;
    }
  } 
  
  *vacantSpace = -1;
  return false;
}

// Determines the upper or lower bound of an the move array depending on the value of mode.
// mode = 0 is min, mode = 1 is max.
int DetermineBestMoveIndex(struct BoardNode boardNode, int mode){
  int bestMoveVal = 0;
  int retval = 0;
  
  if(mode == 0){
    bestMoveVal = INT_MAX;
  }
  else if(mode == 1){
    bestMoveVal = INT_MIN;
  }
  else{
    fprintf(stderr, "Invalid mode passed to determine best move index.\n");
    exit(EXIT_FAILURE);
  }
  
  for(int i = MAX_POSITIONS; i >= 0; i--){
    if(boardNode.move[i] != -1){
      if(mode == 1 && htable[ boardNode.move[i] ].score > bestMoveVal){
        bestMoveVal = boardNode.move[i];
	retval = i;
      }
      else if(mode == 0 && htable[ boardNode.move[i] ].score < bestMoveVal){
        bestMoveVal = boardNode.move[i];
	retval = i;
      }
    }
  }
  
  return retval;
}

// Determines the upper or lower bound of an the move array depending on the value of mode.
// mode = 0 is O (min), mode = 1 is X (max).
int DetermineMoveLimit(struct BoardNode boardNode, int mode){
  int retval;
  
  if(mode == 0){
    retval = INT_MAX;
  }
  else if(mode == 1){
    retval = INT_MIN;
  }
  
  for(int i = MAX_POSITIONS; i >= 0; i--){
    if(boardNode.move[i] != -1){
      if(mode == 1 && htable[ boardNode.move[i] ].score  >= retval){
        retval = htable[ boardNode.move[i] ].score;
      }
      else if(mode == 0 && htable[ boardNode.move[i] ].score <= retval){
        retval = htable[ boardNode.move[i] ].score;
      }
    }
  }
  
  return retval;
}

// Determines if a particular square is vacant. 
// True means it is vacant, false means it is not.
bool isSquareVacant(Board board, int squareIndex){
  if(isdigit(board[pos2idx[squareIndex]])){
    return true;
  }
  
  return false;
}

// Determines if an enemy is in a corner and counteracts it by putting a piece in the opposite corner. 
// If an enemy is not in a corner, then if a corner is vacant, they put thier piece there.
int checkCorners(Board board, char opponent){
  bool enemyNotInCorner = false;
  
  if(!isSquareVacant(board, TOP_LEFT_SQUARE)){
    if(board[pos2idx[TOP_LEFT_SQUARE]] == opponent && isdigit(board[pos2idx[BOTTOM_RIGHT_SQUARE]])){
      return BOTTOM_RIGHT_SQUARE;
    }
  }
  else if(!isSquareVacant(board, TOP_RIGHT_SQUARE)){
    if(board[pos2idx[TOP_RIGHT_SQUARE]] == opponent && isdigit(board[pos2idx[BOTTOM_LEFT_SQUARE]])){
      return BOTTOM_LEFT_SQUARE;
    }
  }
  else if(!isSquareVacant(board, BOTTOM_LEFT_SQUARE)){
    if(board[pos2idx[BOTTOM_LEFT_SQUARE]] == opponent && isdigit(board[pos2idx[TOP_RIGHT_SQUARE]])){
      return TOP_RIGHT_SQUARE;
    }
  }
  else if(!isSquareVacant(board, BOTTOM_RIGHT_SQUARE)){
    if(board[pos2idx[BOTTOM_RIGHT_SQUARE]] == opponent && isdigit(board[pos2idx[TOP_LEFT_SQUARE]])){
      return TOP_LEFT_SQUARE;
    }
  }
  else{
    enemyNotInCorner = true;
  }
  
  if(enemyNotInCorner){
    if(isSquareVacant(board, TOP_LEFT_SQUARE)){
      return TOP_LEFT_SQUARE;
    }
    else if(isSquareVacant(board, TOP_RIGHT_SQUARE)){
      return TOP_RIGHT_SQUARE;
    }
    else if(isSquareVacant(board, BOTTOM_LEFT_SQUARE)){
      return BOTTOM_LEFT_SQUARE;
    }
    else if(isSquareVacant(board, BOTTOM_RIGHT_SQUARE)){
      return BOTTOM_RIGHT_SQUARE;
    }
  }
  
  return -1;
}


// Computes the scores of each individual board.
// 1 = X win
// 0 = draw
// -1 = O win
// If game in progress, the score is predicted by current board state one turn in advance. If O could win, then score is -1, the same is true for draws and X wins
// for their respective scores.
void compute_score(){
  int moveIndex = -1;
  int * movePtr = &moveIndex;

  for(int currentDepth = 9; currentDepth >= 0; currentDepth--){
    for(int i = 0; i < HSIZE; i++){
      if(htable[i].init == '1' && htable[i].depth == currentDepth){
        switch(htable[i].winner){
          case 'X':
            htable[i].score = 1;
            break;
          case 'O':
            htable[i].score = -1;
            break;
          case '-':
            htable[i].score = 0;
            break;
          case '?':
            if(htable[i].turn == 'X'){
	      if(isPossibleToWinThisTurn(htable[i].board, 'X', movePtr)){
	        htable[i].score = 1;
	      }
	      else if(!isPossibleToWinThisTurn(htable[i].board, 'O', movePtr) && !isPossibleToWinThisTurn(htable[i].board, 'X', movePtr)){
	        htable[i].score = 0;
	      }
	      else if(isPossibleToWinThisTurn(htable[i].board, 'O', movePtr)){
	        htable[i].score = -1;
	      }
	      else{
	        htable[i].score = DetermineMoveLimit(htable[i], 1);
	      }
	    }
	    
	    else if(htable[i].turn == 'O'){
	      if(isPossibleToWinThisTurn(htable[i].board, 'O', movePtr)){
	        htable[i].score = -1;
	      }
	      else if(!isPossibleToWinThisTurn(htable[i].board, 'O', movePtr) && !isPossibleToWinThisTurn(htable[i].board, 'X', movePtr)){
	        htable[i].score = 0;
	      }
	      else if(isPossibleToWinThisTurn(htable[i].board, 'X', movePtr)){
	        htable[i].score = 1;
	      }
	      else{
	        htable[i].score = DetermineMoveLimit(htable[i], 0);
	      }
	    }
    
            break;
          default:
            printf("winner: %c\n", htable[i].winner);
            fprintf(stderr, "Problem computing score.\n");
	    exit(EXIT_FAILURE);
            break;
        }
      }
    }  
  }
}

// Determines the best move space for the computer player. It takes into account the appropriate possible game scores and places pieces in strategic postions if
// they are vacant.
int best_move(int board){
  int bestMoveIndex = 0;
  int * movePtr = &bestMoveIndex;
  char opponent;
  
  if(htable[board].turn == 'X'){
    opponent = 'O';
    if(isPossibleToWinThisTurn(htable[board].board, 'X', movePtr)){
      return *movePtr;
    }
    else if(isPossibleToWinThisTurn(htable[board].board, opponent, movePtr)){
      return *movePtr;
    }
    else if(isSquareVacant(htable[board].board, CENTER_SQUARE)){
      bestMoveIndex = CENTER_SQUARE;
      return bestMoveIndex;
    }
    else{
      bestMoveIndex = checkCorners(htable[board].board, 'O');
    
      if(bestMoveIndex == -1){
        bestMoveIndex = DetermineBestMoveIndex(htable[board], 1);
      }
      
      return bestMoveIndex;
    }
  }
  else if(htable[board].turn == 'O'){
    opponent = 'X';
    if(isPossibleToWinThisTurn(htable[board].board, 'O', movePtr)){
      return *movePtr;
    }
    else if(isPossibleToWinThisTurn(htable[board].board, opponent, movePtr)){
      return *movePtr;
    }
    else if(isSquareVacant(htable[board].board, CENTER_SQUARE)){
      bestMoveIndex = CENTER_SQUARE;
      return bestMoveIndex;
    }
    else{
      bestMoveIndex = checkCorners(htable[board].board, 'X');
    
      if(bestMoveIndex == -1){
        bestMoveIndex = DetermineBestMoveIndex(htable[board], 0);
      }
      
      return bestMoveIndex;
    }
  }
  
  return bestMoveIndex;
}