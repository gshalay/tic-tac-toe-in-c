/* File Name: show_node.c
 * File Description: Prints each node by its hash index (by using every command-line argument beyond the first).
 * Student Name: Greg Shalay
 * UoG: 1096689
 */

#include "ttt.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char * argv[]){ 
  int currentNodeIndex = 0;
  
  init_boards();
  
  init_board(START_BOARD);
  
  join_graph(START_BOARD);
  
  compute_score();
  
  for(int i = 1; i < argc; i++){ // i = 1 since argc[0] is the program name.
    currentNodeIndex = atoi(argv[i]);
    print_node( htable[ currentNodeIndex ]) ;
  }
}