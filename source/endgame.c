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

bool is_there_threefold_repetition(History *history){
	square_hist *current_state = history->sqr_hist[history->moves_count - 1];
	int repetition_counter, i;

	for(repetition_counter = 1, i = 0; i < history->moves_count - 1; i++){
		bool is_different;
		int j;

		for(is_different = false, j = 0; j < history->pieces_count; j++){
			if(current_state[j].name   != history->sqr_hist[i][j].name ||
			   current_state[j].color  != history->sqr_hist[i][j].color ||
			   current_state[j].row    != history->sqr_hist[i][j].row ||
			   current_state[j].column != history->sqr_hist[i][j].column){
				is_different = true;
				break;
			}
		}

		if(!is_different)
			repetition_counter++;

		if(repetition_counter == 3)
			return true;
	}

	return false;
}

/*
 * TODO:
 * 1. 50 movimentos
 * 2. Tripla repetição
 * 3. Afogamento
 * 4. Material insuficiente
 */

bool special_finals(square board[][BOARD_SIZE], History *history){
	if(history->moves_count == MAX_MOVES)
		return true;
	else if(is_there_threefold_repetition(history))
		return true;

	return false;
}
