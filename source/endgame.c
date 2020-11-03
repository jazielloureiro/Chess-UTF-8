#include <stdbool.h>
#include <stdio.h>

#include "chess.h"
#include "endgame.h"
#include "movement.h"

bool verify_check(square board[][BOARD_SIZE], char move){
	inputs check;
	char oponent_color = (move == WHITE? BLACK : WHITE);
	int i, j;
	
	// Searching where the king is
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == move){
				check.to_row = i;
				check.to_column = j;
				break;
			}
		}
	}
	
	// Verify if the oponent's pieces are threatening the king
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == oponent_color){
				check.from_row = i;
				check.from_column = j;
			
				if(validate_movement(board[i][j].name, check, move) &&
				   !verify_collision(board[i][j].name, board, check))
				   	return true;
			}
		}
	}
				   	
	return false;
}

bool special_finals(square board[][BOARD_SIZE]){

	return true;
}
