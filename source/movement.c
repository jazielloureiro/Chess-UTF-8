#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

bool is_movement_valid(square board[][BOARD_SIZE], History *history, Player *player){
	if(player->move.from_column == ':')
		return false;

	convert_movement_input(&player->move);

	if(!is_the_squares_valid(&player->move))
		puts("\nYou've entered an invalid square!");
	else if(board[player->move.from_row][player->move.from_column].color != player->turn)
		puts("\nYou must choose a piece of your color.");
	else if(board[player->move.to_row][player->move.to_column].color == player->turn)
		puts("\nYou can't capture your own piece!");
	else if(!is_piece_movement_compatible(board, history, *player))
		puts("\nThis movement is incompatible with your piece.");
	else if(is_jump_other_pieces(board, player->move))
		puts("\nYour piece can't jump over other pieces!");
	else if(will_king_be_in_check(board, *history, *player))
		puts("\nYour can't do this move, because your king will be in check.");
	else
		return true;

	pause_screen();

	return false;
}

bool is_piece_movement_compatible(square board[][BOARD_SIZE], History *history, Player player){
	switch(board[player.move.from_row][player.move.from_column].name){
		case BISHOP:
			return is_bishop_movement_valid(player.move);
		case KING:
			history->has_castle_occurred = is_castle_valid(board, history, player);

			return is_king_movement_valid(player.move) ||
			       history->has_castle_occurred;
		case KNIGHT:
			return is_knight_movement_valid(player.move);
		case PAWN:
			history->has_en_passant_occurred = is_en_passant_valid(board, history, player.move);

			return is_pawn_movement_valid(player) &&
			       (is_pawn_advance_valid(board, player.move) ||
			       is_pawn_capture_valid(board, player.move) ||
			       history->has_en_passant_occurred);
		case QUEEN:
			return is_queen_movement_valid(player.move);
		case ROOK:
			return is_rook_movement_valid(player.move);
	}
}

bool is_bishop_movement_valid(move_coordinates move){
	return abs(move.to_row - move.from_row) ==
	       abs(move.to_column - move.from_column);
}

bool is_king_movement_valid(move_coordinates move){
	return move.from_row - 1 <= move.to_row &&
	       move.from_row + 1 >= move.to_row &&
	       move.from_column - 1 <= move.to_column &&
	       move.from_column + 1 >= move.to_column;
}

bool is_knight_movement_valid(move_coordinates move){
	const int DIFF_ROW = abs(move.to_row - move.from_row),
	          DIFF_COL = abs(move.to_column - move.from_column);

	return DIFF_ROW == 2 && DIFF_COL == 1 ||
	       DIFF_ROW == 1 && DIFF_COL == 2;
}

bool is_pawn_movement_valid(Player player){
	const int ORIGIN_ROW = (player.turn == WHITE? 6 : 1),
	          TWO_SQUARES = (player.turn == WHITE? -2 : 2),
	          ONE_SQUARE = (player.turn == WHITE? -1 : 1);

	return player.move.from_row + ONE_SQUARE == player.move.to_row &&
	       player.move.from_column - 1 <= player.move.to_column &&
	       player.move.from_column + 1 >= player.move.to_column ||
	       player.move.from_row == ORIGIN_ROW &&
	       player.move.from_row + TWO_SQUARES == player.move.to_row &&
	       player.move.from_column == player.move.to_column;
}

bool is_queen_movement_valid(move_coordinates move){
	return is_bishop_movement_valid(move) ||
	       is_rook_movement_valid(move);
}

bool is_rook_movement_valid(move_coordinates move){
	return move.from_row == move.to_row ||
	       move.from_column == move.to_column;
}

bool is_castle_valid(square board[][BOARD_SIZE], History *history, Player player){
	const int START_ROW = (player.turn == WHITE? 7 : 0);
	const int START_KING_COL = 4;
	const int START_ROOK_COL = (START_KING_COL - 2 == player.move.to_column? 0 : 7);

	if(START_ROW != player.move.from_row ||
	   START_ROW != player.move.to_row ||
	   START_KING_COL != player.move.from_column ||
	   START_KING_COL - 2 != player.move.to_column &&
	   START_KING_COL + 2 != player.move.to_column)
		return false;

	return !has_castle_pieces_moved(history, START_ROW, START_KING_COL) &&
	       !are_there_pieces_between(board[START_ROW], START_KING_COL, START_ROOK_COL) &&
	       is_king_safe(board, history, player);
}

bool has_castle_pieces_moved(History *history, int row, int rook_col){
	const int KING_COL = 4;

	for(h_board *aux = history->board; aux != NULL; aux = aux->prev)
		if((aux->player.move.from_column == KING_COL ||
		   aux->player.move.from_column == rook_col) &&
		   aux->player.move.from_row == row)
			return true;

	return false;
}

bool are_there_pieces_between(square row[], char start, char end){
	char i = start;

	for(advance_to(&i, end); i != end; advance_to(&i, end))
		if(row[i].name != EMPTY_P)
			return true;

	return false;
}

bool is_king_safe(square board[][BOARD_SIZE], History *history, Player player){
	if(is_player_king_in_check(board, history, player.turn))
		return false;

	do{
		advance_to(&player.move.from_column, player.move.to_column);

		if(will_king_be_in_check(board, *history, player))
			return false;
	}while(player.move.from_column != player.move.to_column);

	return true;
}

bool is_pawn_advance_valid(square board[][BOARD_SIZE], move_coordinates move){
	return move.from_column == move.to_column &&
	       board[move.to_row][move.to_column].name == EMPTY_P;
}

bool is_pawn_capture_valid(square board[][BOARD_SIZE], move_coordinates move){
	return move.from_column != move.to_column &&
	       board[move.to_row][move.to_column].name != EMPTY_P;
}

bool is_en_passant_valid(square board[][BOARD_SIZE], History *history, move_coordinates move){
	if(history->board == NULL)
		return false;

	int from_row = history->board->player.move.from_row,
	    from_column = history->board->player.move.from_column,
	    to_row = history->board->player.move.to_row,
	    to_column = history->board->player.move.to_column,
	    advance2Squares = (board[move.from_row][move.from_column].color == WHITE? 2 : -2),
	    origin_row = (board[move.from_row][move.from_column].color == WHITE? 1 : 6);

	return board[to_row][to_column].name == PAWN &&
	       from_row + advance2Squares == to_row &&
	       from_row == origin_row &&
	       from_column == to_column &&
	       to_row == move.from_row &&
	       (to_column - move.from_column == 1 ||
	       to_column - move.from_column == -1) &&
	       to_column == move.to_column;
}

bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coordinates move){
	if(board[move.from_row][move.from_column].name != KNIGHT){
		char i = move.from_row, j = move.from_column;
		
		for(advance_to(&i, move.to_row), advance_to(&j, move.to_column);
		    i != move.to_row || j != move.to_column;
		    advance_to(&i, move.to_row), advance_to(&j, move.to_column))
			if(board[i][j].name != EMPTY_P)
				return true;
	}
	
	return false;
}

bool will_king_be_in_check(square board[][BOARD_SIZE], History history, Player player){
	bool is_check,
	     has_special_move = history.has_castle_occurred || history.has_en_passant_occurred;
	movement_squares move_squares, aux_squares;
	move_coordinates aux_move;

	save_move_squares(board, &move_squares, player.move);

	move_piece(board, player.move);

	if(has_special_move){
		if(history.has_castle_occurred)
			find_castle_rook(player.move, &aux_move);
		else
			aux_move = history.board->player.move;

		save_move_squares(board, &aux_squares, aux_move);

		move_piece(board, aux_move);
	}

	is_check = is_player_king_in_check(board, &history, player.turn);

	return_move_squares(board, move_squares, player.move);

	if(has_special_move)
		return_move_squares(board, aux_squares, aux_move);

	return is_check;
}

bool is_player_king_in_check(square board[][BOARD_SIZE], History *history, char turn){
	Player opponent;

	opponent.turn = (turn == WHITE? BLACK : WHITE);
	
	// Searching where the king is
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == turn){
				opponent.move.to_row = i;
				opponent.move.to_column = j;
				break;
			}
		}
	}
	
	// Verify if the opponent's pieces are threatening the king
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == opponent.turn){
				opponent.move.from_row = i;
				opponent.move.from_column = j;

				if(is_piece_movement_compatible(board, history, opponent) &&
				   !is_jump_other_pieces(board, opponent.move)){
					history->last_check = opponent.move;
					
				   	return true;
				}
			}
		}
	}
				   	
	return false;
}
