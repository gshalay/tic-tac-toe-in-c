/* File Name: count_nodes.c
 * File Description: Counts the number of non-empty nodes in the board table by looping through the htable.
 * Student Name: Greg Shalay
 * UoG: 1096689
 */

#include "ttt.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char * argv[]){ 
  int nodes = 0;
  
  init_boards();
  
  init_board(START_BOARD);
  
  join_graph(START_BOARD);
  
  for(int i = 0; i < HSIZE; i++){
    if(htable[i].init == '1'){
      nodes++;
    }
  }
  
  printf("%d\n", nodes);
  
}