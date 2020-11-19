#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "endgame.h"
#include "movement.h"

bool will_king_be_in_check(square board[][BOARD_SIZE], History history, Player player){
	bool is_check;
	movement_squares move_squares, aux_squares;
	move_coordinates aux_move;

	save_move_squares(board, &move_squares, player.move);

	move_piece(board, player.move);

	if(history.has_en_passant_occurred){
		aux_move.from_row = history.last_input.to_row;
		aux_move.from_column = history.last_input.to_column;
		aux_move.to_row = history.last_input.to_row;
		aux_move.to_column = history.last_input.to_column;

		save_move_squares(board, &aux_squares, aux_move);

		en_passant(board, history);
	}

	if(history.castle.has_occurred){
		find_castle_rook(player.move, &aux_move);

		save_move_squares(board, &aux_squares, aux_move);

		move_piece(board, aux_move);
	}
			
	is_check = is_player_king_in_check(board, &history, player.turn);

	return_move_squares(board, move_squares, player.move);

	if(history.has_en_passant_occurred)
		return_move_squares(board, aux_squares, aux_move);

	if(history.castle.has_occurred)
		return_move_squares(board, aux_squares, aux_move);

	return is_check;
}

bool is_player_king_in_check(square board[][BOARD_SIZE], History *history, char turn){
	Player opponent;
	int i, j;

	opponent.turn = (turn == WHITE? BLACK : WHITE);
	
	// Searching where the king is
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == turn){
				opponent.move.to_row = i;
				opponent.move.to_column = j;
				break;
			}
		}
	}
	
	// Verify if the opponent's pieces are threatening the king
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == opponent.turn){
				opponent.move.from_row = i;
				opponent.move.from_column = j;

				if(is_piece_movement_compatible(board, history, opponent) &&
				   !is_jump_other_pieces(board, opponent.move)){
					history->check.from_row = i;
					history->check.from_column = j;
					history->check.to_row = opponent.move.to_row;
					history->check.to_column = opponent.move.to_column;
					
				   	return true;
				}
			}
		}
	}
				   	
	return false;
}

bool can_king_move(square board[][BOARD_SIZE], History history, char turn){
	int i, j;

	for(i = history.check.to_row - 1; i <= history.check.to_row + 1; i++){
		for(j = history.check.to_column - 1; j <= history.check.to_column + 1; j++){
			if(i >= 0 && i <= 7 && j >= 0 && j <= 7 &&
			   board[i][j].color != board[history.check.to_row][history.check.to_column].color){
				Player temp;

				temp.turn = turn;
				temp.move.to_row = i;
				temp.move.to_column = j;
				temp.move.from_row = history.check.to_row;
				temp.move.from_column = history.check.to_column;

				if(!will_king_be_in_check(board, history, temp))
				   	return true;
			}
		}
	}

	return false;
}

bool can_attacking_piece_be_captured(square board[][BOARD_SIZE], History history, char turn){
	int i, j;
	Player piece;

	piece.turn = turn;
	piece.move.to_row = history.check.from_row;
	piece.move.to_column = history.check.from_column;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == turn){
				piece.move.from_row = i;
				piece.move.from_column = j;
			
				if(is_piece_movement_compatible(board, &history, piece) &&
				   !is_jump_other_pieces(board, piece.move) &&
				   !will_king_be_in_check(board, history, piece))
				   	return true;
			}
		}
	}

	return false;
}

bool can_piece_cover_check(square board[][BOARD_SIZE], History history, char turn){
	char *i = &history.check.from_row, *j = &history.check.from_column;

	if(board[*i][*j].name != KNIGHT){
		do{
			advance_to(i, history.check.to_row);
			advance_to(j, history.check.to_column);
			
			if((*i) != history.check.to_row || (*j) != history.check.to_column)
				if(can_attacking_piece_be_captured(board, history, turn))
					return true;
		}while(*i != history.check.to_row || *j != history.check.to_column);
	}

	return false;
}

bool is_there_checkmate(square board[][BOARD_SIZE], History history, char turn){
	if(can_king_move(board, history, turn))
		return false;
	else if(can_piece_cover_check(board, history, turn))
		return false;
	else if(can_attacking_piece_be_captured(board, history, turn))
		return false;

	return true;
}

bool is_there_possible_move(square board[][BOARD_SIZE], History history, Player temp){
	int i, j;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color != temp.turn){
				temp.move.to_row = i;
				temp.move.to_column = j;

				if(is_piece_movement_compatible(board, &history, temp) &&
				   !is_jump_other_pieces(board, temp.move) &&
				   !will_king_be_in_check(board, history, temp))
					return true;
			}
		}
	}

	return false;
}

bool is_there_stalemate(square board[][BOARD_SIZE], History history, char turn){
	int i, j;

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == turn){
				Player temp;

				temp.turn = turn;
				temp.move.from_row = i;
				temp.move.from_column = j;
				
				if(is_there_possible_move(board, history, temp))
					return false;
			}
		}
	}

	return true;
}

bool is_there_threefold_repetition(History *history){
	square_hist *current_state = history->board[history->moves_counter - 1];
	int repetition_counter, i;

	for(repetition_counter = 1, i = 0; i < history->moves_counter - 1; i++){
		bool is_different;
		int j;

		for(is_different = false, j = 0; j < history->pieces_counter; j++){
			if(current_state[j].name   != history->board[i][j].name ||
			   current_state[j].color  != history->board[i][j].color ||
			   current_state[j].row    != history->board[i][j].row ||
			   current_state[j].column != history->board[i][j].column){
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
	bool has_white_sufficient_material = false, has_black_sufficient_material = false;

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
					has_white_sufficient_material = true;
				else
					has_black_sufficient_material = true;
			}
		}
	}

	if(white_minor_pieces >= 2)
		has_white_sufficient_material = true;
	if(black_minor_pieces >= 2)
		has_black_sufficient_material = true;

	if(has_white_sufficient_material || has_black_sufficient_material)
		return false;

	return true;
}

bool is_there_special_final(square board[][BOARD_SIZE], History *history){
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
