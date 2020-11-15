#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "endgame.h"
#include "movement.h"

bool is_king_will_be_in_check(square board[][BOARD_SIZE], History *history, char turn, move_coordinates move_input){
	bool is_check;
	movement_squares move_squares, aux_squares;
	move_coordinates check, aux_move;

	save_move_squares(board, &move_squares, move_input);

	move_piece(board, move_input);

	if(history->has_en_passant_occurred){
		aux_move.from_row = history->last_input.to_row;
		aux_move.from_column = history->last_input.to_column;
		aux_move.to_row = history->last_input.to_row;
		aux_move.to_column = history->last_input.to_column;

		save_move_squares(board, &aux_squares, aux_move);

		en_passant(board, *history);
	}

	if(history->castle.has_occurred){
		if(move_input.to_row == 0){
			if(move_input.to_column == 2){
				aux_move.from_row = 0;
				aux_move.from_column = 0;
				aux_move.to_row = 0;
				aux_move.to_column = 3;
			}else{
				aux_move.from_row = 0;
				aux_move.from_column = 7;
				aux_move.to_row = 0;
				aux_move.to_column = 5;
			}
		}else{
			if(move_input.to_column == 2){
				aux_move.from_row = 7;
				aux_move.from_column = 0;
				aux_move.to_row = 7;
				aux_move.to_column = 3;
			}else{
				aux_move.from_row = 7;
				aux_move.from_column = 7;
				aux_move.to_row = 7;
				aux_move.to_column = 5;
			}
		}

		save_move_squares(board, &aux_squares, aux_move);

		move_piece(board, aux_move);
	}
			
	is_check = is_player_king_in_check(board, history, turn, &check);

	return_move_squares(board, move_squares, move_input);

	if(history->has_en_passant_occurred)
		return_move_squares(board, aux_squares, aux_move);

	if(history->castle.has_occurred)
		return_move_squares(board, aux_squares, aux_move);

	return is_check;
}

bool is_player_king_in_check(square board[][BOARD_SIZE], History *history, char turn, move_coordinates *check){
	char opponent_color = (turn == WHITE? BLACK : WHITE);
	int i, j;
	
	// Searching where the king is
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == turn){
				check->to_row = i;
				check->to_column = j;
				break;
			}
		}
	}
	
	// Verify if the oponent's pieces are threatening the king
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == opponent_color){
				check->from_row = i;
				check->from_column = j;

				if(is_piece_movement_compatible(board, history, *check, opponent_color) &&
				   !is_jump_other_pieces(board, *check))
				   	return true;
			}
		}
	}
				   	
	return false;
}

bool can_king_move(square board[][BOARD_SIZE], History *history, move_coordinates check, char turn){
	int i, j;

	for(i = check.to_row - 1; i <= check.to_row + 1; i++){
		for(j = check.to_column - 1; j <= check.to_column + 1; j++){
			if(i >= 0 && i <= 7 && j >= 0 && j <= 7 &&
			   board[i][j].color != board[check.to_row][check.to_column].color){
				move_coordinates temp_move;
				movement_squares temp_squares;

				temp_move.to_row = i;
				temp_move.to_column = j;
				temp_move.from_row = check.to_row;
				temp_move.from_column = check.to_column;

				if(!is_king_will_be_in_check(board, history, turn, temp_move))
				   	return true;
			}
		}
	}

	return false;
}

bool can_attacking_piece_be_captured(square board[][BOARD_SIZE], History *history, move_coordinates check, char turn){
	int i, j;
	move_coordinates att_piece;

	att_piece.to_row = check.from_row;
	att_piece.to_column = check.from_column;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == turn){
				att_piece.from_row = i;
				att_piece.from_column = j;
			
				if(is_piece_movement_compatible(board, history, att_piece, turn) &&
				   !is_jump_other_pieces(board, att_piece) &&
				   !is_king_will_be_in_check(board, history, turn, att_piece))
				   	return true;
			}
		}
	}

	return false;
}

bool can_piece_cover_check(square board[][BOARD_SIZE], History *history, move_coordinates check, char turn){
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
				if(can_attacking_piece_be_captured(board, history, check, turn))
					return true;
		}while((*i) != check.to_row || (*j) != check.to_column);
	}

	return false;
}

bool has_checkmate(square board[][BOARD_SIZE], History *history, move_coordinates check, char turn){
	if(can_king_move(board, history, check, turn))
		return false;
	else if(can_piece_cover_check(board, history, check, turn))
		return false;
	else if(can_attacking_piece_be_captured(board, history, check, turn))
		return false;

	return true;
}

bool is_there_possible_move(square board[][BOARD_SIZE], History *history, move_coordinates movement, char turn){
	int i, j;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color != turn){
				movement.to_row = i;
				movement.to_column = j;

				if(is_piece_movement_compatible(board, history, movement, turn) &&
				   !is_jump_other_pieces(board, movement) &&
				   !is_king_will_be_in_check(board, history, turn, movement))
					return true;
			}
		}
	}

	return false;
}

bool has_stalemate(square board[][BOARD_SIZE], History *history, char turn){
	int i, j;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == turn){
				move_coordinates movement;

				movement.from_row = i;
				movement.from_column = j;
				
				if(is_there_possible_move(board, history, movement, turn))
					return false;
			}
		}
	}

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
	if(history->moves_counter == MAX_MOVES){
		print_final_board(board, FIFTY_MOVES);
		return true;
	}else if(is_there_threefold_repetition(history)){
		print_final_board(board, THREEFOLD_REP);
		return true;
	}else if(is_there_insufficient_material(board)){
		print_final_board(board, INSUFFICIENT_MAT);
		return true;
	}

	return false;
}
