#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "endgame.h"
#include "movement.h"

bool is_player_king_in_check(square board[][BOARD_SIZE], char move, movement_input *check){
	char oponent_color = (move == WHITE? BLACK : WHITE);
	int i, j;
	
	// Searching where the king is
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == move){
				check->to_row = i;
				check->to_column = j;
				break;
			}
		}
	}
	
	// Verify if the oponent's pieces are threatening the king
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == oponent_color){
				check->from_row = i;
				check->from_column = j;
			
				if(is_piece_movement_compatible(board, *check, move) &&
				   !is_jump_other_pieces(board, *check))
				   	return true;
			}
		}
	}
				   	
	return false;
}

bool is_attacking_piece_can_be_captured(square board[][BOARD_SIZE], movement_input check, char move){
	int i, j;
	movement_input att_piece;

	att_piece.to_row = check.from_row;
	att_piece.to_column = check.from_column;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == move){
				att_piece.from_row = i;
				att_piece.from_column = j;
			
				if(is_piece_movement_compatible(board, att_piece, move) &&
				   !is_jump_other_pieces(board, att_piece)){
					movement_squares temp_squares;

					save_move_squares(board, &temp_squares, att_piece);

					move_piece(board, att_piece);

					if(is_player_king_in_check(board, move, &check)){
						return_move_squares(board, temp_squares, att_piece);
						continue;
					}else{
						return_move_squares(board, temp_squares, att_piece);
				   		return true;
					}
				}
			}
		}
	}

	return false;
}

bool is_king_can_move(square board[][BOARD_SIZE], movement_input check, char move){
	int i, j;

	for(i = check.to_row - 1; i <= check.to_row + 1; i++){
		for(j = check.to_column - 1; j <= check.to_column + 1; j++){
			if(i >= 0 && i <= 7 && j >= 0 && j <= 7 &&
			   board[i][j].color != board[check.to_row][check.to_column].color){
				movement_input temp_move, temp_check;
				movement_squares temp_squares;

				temp_move.to_row = i;
				temp_move.to_column = j;
				temp_move.from_row = check.to_row;
				temp_move.from_column = check.to_column;

				save_move_squares(board, &temp_squares, temp_move);

				move_piece(board, temp_move);

				if(is_player_king_in_check(board, move, &temp_check)){
					return_move_squares(board, temp_squares, temp_move);
					continue;
				}else{
					return_move_squares(board, temp_squares, temp_move);
				   	return true;
				}
			}
		}
	}

	return false;
}

bool can_piece_cover_check(square board[][BOARD_SIZE], movement_input check, char move){
	char *i = &check.from_row, *j = &check.from_column;

	if(board[*i][*j].name != KNIGHT){
		do{
			if(*i < check.to_row)
				(*i)++;
			else if(*i > check.to_row)
				(*i)--;
			
			if(*j < check.to_column)
				(*j)++;
			else if(*j > check.to_column)
				(*j)--;
			
			if((*i) != check.to_row || (*j) != check.to_column)
				if(is_attacking_piece_can_be_captured(board, check, move))
					return true;
		}while((*i) != check.to_row || (*j) != check.to_column);
	}

	return false;
}

bool has_checkmate(square board[][BOARD_SIZE], movement_input check, char move){
	if(is_king_can_move(board, check, move))
		return false;
	else if(can_piece_cover_check(board, check, move))
		return false;
	else if(is_attacking_piece_can_be_captured(board, check, move))
		return false;

	return true;
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
