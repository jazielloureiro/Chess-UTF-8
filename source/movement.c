#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

bool is_movement_valid(square board[][BOARD_SIZE], History *history, Player *player){
	if(player->move.from_column == ':')
		return false;

	convert_movement_input(&player->move);

	if(!is_the_squares_valid(&player->move)){
		print_error_message(INVALID_SQUARE);
		return false;
	}else if(board[player->move.from_row][player->move.from_column].color != player->turn){
		print_error_message(CHOOSE_WRONG_COLOR);
		return false;
	}else if(board[player->move.to_row][player->move.to_column].color == player->turn){
		print_error_message(CAPTURE_OWN_PIECE);
		return false;
	}else if(!is_piece_movement_compatible(board, history, *player)){
		print_error_message(INCOMPATIBLE_MOVE);
		return false;
	}else if(is_jump_other_pieces(board, player->move)){
		print_error_message(JUMP_OTHER_PIECES);
		return false;
	}else if(will_king_be_in_check(board, *history, *player)){
		print_error_message(KING_IN_CHECK);
		return false;
	}

	return true;
}

bool is_piece_movement_compatible(square board[][BOARD_SIZE], History *history, Player player){
	switch(board[player.move.from_row][player.move.from_column].name){
		case BISHOP:
			return is_bishop_movement_valid(player.move);
		case KING:{
			bool is_valid = is_king_movement_valid(player.move);

			if(!is_valid)
				is_valid = is_castle_valid(board, history, player);

			return is_valid;
		}case KNIGHT:
			return is_knight_movement_valid(player.move);
		case PAWN:{
			bool is_valid = is_pawn_movement_valid(player);

			if(is_valid)
				is_valid = is_pawn_capture_valid(board, history, player.move);

			return is_valid;
		}case QUEEN:
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

bool are_there_pieces_between(square row[], char start, char end){
	char i = start;

	for(advance_to(&i, end); i != end; advance_to(&i, end))
		if(row[i].name != NO_PIECE)
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

bool is_castle_valid(square board[][BOARD_SIZE], History *history, Player player){
	const int king_column = 4;
	int row, rook_column;
	bool is_valid = true;

	if(player.turn == WHITE &&
	   !history->castle.has_white_king_moved &&
	   player.move.to_row == 7){
		row = 7;

		if(!history->castle.has_left_white_rook_moved && player.move.to_column == 2)
			rook_column = 0;
		else if(!history->castle.has_right_white_rook_moved && player.move.to_column == 6)
			rook_column = 7;
		else
			is_valid = false;
	}else if(player.turn == BLACK &&
	         !history->castle.has_black_king_moved &&
	         player.move.to_row == 0){
		row = 0;

		if(!history->castle.has_left_black_rook_moved && player.move.to_column == 2)
			rook_column = 0;
		else if(!history->castle.has_right_black_rook_moved && player.move.to_column == 6)
			rook_column = 7;
		else
			is_valid = false;
	}else
		is_valid = false;

	if(is_valid &&
	   !are_there_pieces_between(board[row], king_column, rook_column) &&
	   is_king_safe(board, history, player)){
	   	history->castle.has_occurred = true;
		return true;
	}

	return false;
}

bool is_knight_movement_valid(move_coordinates move){
	int diff_row = abs(move.to_row - move.from_row),
	    diff_column = abs(move.to_column - move.from_column);

	return diff_row == 2 && diff_column == 1 ||
	       diff_row == 1 && diff_column == 2;
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

bool is_pawn_capture_valid(square board[][BOARD_SIZE], History *history, move_coordinates move){
	if(move.from_column == move.to_column &&
	   board[move.to_row][move.to_column].name == NO_PIECE)
		return true;

	if(move.from_column != move.to_column &&
	   board[move.to_row][move.to_column].name != NO_PIECE)
	   	return true;

	if(is_en_passant_valid(board, history, move))
		return true;

	return false;
}

bool is_en_passant_valid(square board[][BOARD_SIZE], History *history, move_coordinates move){
	int from_row = history->last_input.from_row,
	    from_column = history->last_input.from_column,
	    to_row = history->last_input.to_row,
	    to_column = history->last_input.to_column,
	    advance2Squares = (board[move.from_row][move.from_column].color == WHITE? 2 : -2),
	    origin_row = (board[move.from_row][move.from_column].color == WHITE? 1 : 6);

	if(board[to_row][to_column].name == PAWN &&
	   from_row + advance2Squares == to_row &&
	   from_row == origin_row &&
	   from_column == to_column &&
	   to_row == move.from_row &&
	   (to_column - move.from_column == 1 ||
	   to_column - move.from_column == -1) &&
	   to_column == move.to_column){
		history->has_en_passant_occurred = true;
	   	return true;
	}

	return false;
}

bool is_queen_movement_valid(move_coordinates move){
	return is_bishop_movement_valid(move) ||
	       is_rook_movement_valid(move);
}

bool is_rook_movement_valid(move_coordinates move){
	return move.from_row == move.to_row ||
	       move.from_column == move.to_column;
}

bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coordinates move){
	if(board[move.from_row][move.from_column].name != KNIGHT){
		char i = move.from_row, j = move.from_column;
		
		for(advance_to(&i, move.to_row), advance_to(&j, move.to_column);
		    i != move.to_row || j != move.to_column;
			advance_to(&i, move.to_row), advance_to(&j, move.to_column))
			if(board[i][j].name != NO_PIECE)
				return true;
	}
	
	return false;
}

void move_piece(square board[][BOARD_SIZE], move_coordinates move){
	strcpy(board[move.to_row][move.to_column].image,
	       board[move.from_row][move.from_column].image);

	board[move.to_row][move.to_column].name = 
	board[move.from_row][move.from_column].name;

	board[move.to_row][move.to_column].color = 
	board[move.from_row][move.from_column].color;

	strcpy(board[move.from_row][move.from_column].image, " \0");

	board[move.from_row][move.from_column].name = NO_PIECE;

	board[move.from_row][move.from_column].color = NO_PIECE;
}

void promotion(square *piece, char turn){
	char choose;

	puts("\nFor which piece do you want to promove?");
	
	if(turn == WHITE)
		puts("1. ♕\n2. ♖\n3. ♗\n4. ♘\n");
	else
		puts("1. ♛\n2. ♜\n3. ♝\n4. ♞\n");
	       
	do{
		printf("> ");
		choose = getchar();
		clear_input_buffer();
	}while(choose < '1' || choose > '4');
	       
	switch(choose){
		case '1':
			sprintf(piece->image, turn == WHITE? "♕" : "♛");
			piece->name = QUEEN;
			break;
		case '2':
			sprintf(piece->image, turn == WHITE? "♖" : "♜");
			piece->name = ROOK;
			break;
		case '3':
			sprintf(piece->image, turn == WHITE? "♗" : "♝");
			piece->name = BISHOP;
			break;
		case '4':
			sprintf(piece->image, turn == WHITE? "♘" : "♞");
			piece->name = KNIGHT;
	}       
}
