#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

bool is_movement_valid(square board[][BOARD_SIZE], History *history, Player *player){
	if(player->move.from_file == ':')
		return false;

	convert_movement_input(&player->move);

	if(!is_the_squares_valid(player->move))
		puts("\nYou've entered an invalid square!");
	else if(board[player->move.from_rank][player->move.from_file].color != player->turn)
		puts("\nYou must choose a piece of your color.");
	else if(board[player->move.to_rank][player->move.to_file].color == player->turn)
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
	switch(board[player.move.from_rank][player.move.from_file].name){
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

bool is_bishop_movement_valid(move_coord move){
	return abs(move.to_rank - move.from_rank) ==
	       abs(move.to_file - move.from_file);
}

bool is_king_movement_valid(move_coord move){
	return move.from_rank - 1 <= move.to_rank &&
	       move.from_rank + 1 >= move.to_rank &&
	       move.from_file - 1 <= move.to_file &&
	       move.from_file + 1 >= move.to_file;
}

bool is_knight_movement_valid(move_coord move){
	const int DIFF_ROW = abs(move.to_rank - move.from_rank),
	          DIFF_COL = abs(move.to_file - move.from_file);

	return DIFF_ROW == 2 && DIFF_COL == 1 ||
	       DIFF_ROW == 1 && DIFF_COL == 2;
}

bool is_pawn_movement_valid(Player player){
	const int ORIGIN_ROW = (player.turn == WHITE? 6 : 1),
	          TWO_SQUARES = (player.turn == WHITE? -2 : 2),
	          ONE_SQUARE = (player.turn == WHITE? -1 : 1);

	return player.move.from_rank + ONE_SQUARE == player.move.to_rank &&
	       player.move.from_file - 1 <= player.move.to_file &&
	       player.move.from_file + 1 >= player.move.to_file ||
	       player.move.from_rank == ORIGIN_ROW &&
	       player.move.from_rank + TWO_SQUARES == player.move.to_rank &&
	       player.move.from_file == player.move.to_file;
}

bool is_queen_movement_valid(move_coord move){
	return is_bishop_movement_valid(move) ||
	       is_rook_movement_valid(move);
}

bool is_rook_movement_valid(move_coord move){
	return move.from_rank == move.to_rank ||
	       move.from_file == move.to_file;
}

bool is_castle_valid(square board[][BOARD_SIZE], History *history, Player player){
	const int START_ROW = (player.turn == WHITE? 7 : 0);
	const int START_KING_COL = 4;
	const int START_ROOK_COL = (START_KING_COL - 2 == player.move.to_file? 0 : 7);

	if(START_ROW != player.move.from_rank ||
	   START_ROW != player.move.to_rank ||
	   START_KING_COL != player.move.from_file ||
	   START_KING_COL - 2 != player.move.to_file &&
	   START_KING_COL + 2 != player.move.to_file)
		return false;

	return !has_castle_pieces_moved(history, START_ROW, START_KING_COL) &&
	       !are_there_pieces_between(board[START_ROW], START_KING_COL, START_ROOK_COL) &&
	       is_king_safe(board, history, player);
}

bool has_castle_pieces_moved(History *history, int rank, int rook_col){
	const int KING_COL = 4;

	for(h_board *aux = history->board; aux != NULL; aux = aux->prev)
		if((aux->player.move.from_file == KING_COL ||
		   aux->player.move.from_file == rook_col) &&
		   aux->player.move.from_rank == rank)
			return true;

	return false;
}

bool are_there_pieces_between(square rank[], char start, char end){
	char i = start;

	for(advance_to(&i, end); i != end; advance_to(&i, end))
		if(rank[i].name != EMPTY_P)
			return true;

	return false;
}

bool is_king_safe(square board[][BOARD_SIZE], History *history, Player player){
	if(is_player_king_in_check(board, history, player.turn))
		return false;

	do{
		advance_to(&player.move.from_file, player.move.to_file);

		if(will_king_be_in_check(board, *history, player))
			return false;
	}while(player.move.from_file != player.move.to_file);

	return true;
}

bool is_pawn_advance_valid(square board[][BOARD_SIZE], move_coord move){
	return move.from_file == move.to_file &&
	       board[move.to_rank][move.to_file].name == EMPTY_P;
}

bool is_pawn_capture_valid(square board[][BOARD_SIZE], move_coord move){
	return move.from_file != move.to_file &&
	       board[move.to_rank][move.to_file].name != EMPTY_P;
}

bool is_en_passant_valid(square board[][BOARD_SIZE], History *history, move_coord move){
	if(history->board == NULL)
		return false;

	int from_rank = history->board->player.move.from_rank,
	    from_file = history->board->player.move.from_file,
	    to_rank = history->board->player.move.to_rank,
	    to_file = history->board->player.move.to_file,
	    advance2Squares = (board[move.from_rank][move.from_file].color == WHITE? 2 : -2),
	    origin_rank = (board[move.from_rank][move.from_file].color == WHITE? 1 : 6);

	return board[to_rank][to_file].name == PAWN &&
	       from_rank + advance2Squares == to_rank &&
	       from_rank == origin_rank &&
	       from_file == to_file &&
	       to_rank == move.from_rank &&
	       (to_file - move.from_file == 1 ||
	       to_file - move.from_file == -1) &&
	       to_file == move.to_file;
}

bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coord move){
	if(board[move.from_rank][move.from_file].name != KNIGHT){
		char i = move.from_rank, j = move.from_file;
		
		for(advance_to(&i, move.to_rank), advance_to(&j, move.to_file);
		    i != move.to_rank || j != move.to_file;
		    advance_to(&i, move.to_rank), advance_to(&j, move.to_file))
			if(board[i][j].name != EMPTY_P)
				return true;
	}
	
	return false;
}

bool will_king_be_in_check(square board[][BOARD_SIZE], History history, Player player){
	bool is_check,
	     has_special_move = history.has_castle_occurred || history.has_en_passant_occurred;
	movement_squares move_squares, aux_squares;
	move_coord aux_move;

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
				opponent.move.to_rank = i;
				opponent.move.to_file = j;
				break;
			}
		}
	}
	
	// Verify if the opponent's pieces are threatening the king
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == opponent.turn){
				opponent.move.from_rank = i;
				opponent.move.from_file = j;

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
