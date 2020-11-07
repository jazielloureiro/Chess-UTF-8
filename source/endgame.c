#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "endgame.h"
#include "movement.h"

bool is_player_king_in_check(square board[][BOARD_SIZE], char move){
	movement_input check;
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
			
				if(is_piece_movement_compatible(board, check, move) &&
				   !is_jump_other_pieces(board, check))
				   	return true;
			}
		}
	}
				   	
	return false;
}

bool is_there_threefold_repetition(History *history){
	square_hist *current_state = history->sqr_hist[history->moves_counter - 1];
	int repetition_counter, i;

	for(repetition_counter = 1, i = 0; i < history->moves_counter - 1; i++){
		bool is_different;
		int j;

		for(is_different = false, j = 0; j < history->pieces_counter; j++){
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

bool is_there_insufficient_material(square board[][BOARD_SIZE]){
	int white_minor_pieces = 0, black_minor_pieces = 0, i, j;
	bool white_has_sufficient_material = false, black_has_sufficient_material = false;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == BISHOP || board[i][j].name == KNIGHT){
				if(board[i][j].color == WHITE)
					white_minor_pieces++;
				else
					black_minor_pieces++;
			}else if(board[i][j].name == QUEEN ||
			         board[i][j].name == ROOK ||
			         board[i][j].name == PAWN){
				if(board[i][j].color == WHITE)
					white_has_sufficient_material = true;
				else
					black_has_sufficient_material = true;
			}
		}
	}

	if(white_minor_pieces >= 2)
		white_has_sufficient_material = true;
	if(black_minor_pieces >= 2)
		black_has_sufficient_material = true;

	if(white_has_sufficient_material || black_has_sufficient_material)
		return false;

	return true;
}

bool is_there_special_finals(square board[][BOARD_SIZE], History *history){
	bool is_there_final = false;

	if(history->moves_counter == MAX_MOVES){
		puts("\nNo capture has been made and no pawn has been moved in the last fifty moves.");
		is_there_final = true;
	}else if(is_there_threefold_repetition(history)){
		puts("\nThe same board position has occurred three times.");
		is_there_final = true;
	}else if(is_there_insufficient_material(board)){
		puts("\nBoth the players have insufficient material to force a checkmate.");
		is_there_final = true;
	}

	if(is_there_final){
		puts("So, It's a draw!");
		pause();
	}

	return is_there_final;
}
