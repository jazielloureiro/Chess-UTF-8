#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

bool is_movement_valid(square board[][BOARD_SIZE], History *history, move_coordinates *move_input, char turn){
	convert_square_readed(&move_input->from_row, &move_input->from_column);
	convert_square_readed(&move_input->to_row, &move_input->to_column);

	if(!is_the_squares_valid(move_input)){
		print_error_message(INVALID_SQUARE);
		return false;
	}else if(board[move_input->from_row][move_input->from_column].color != turn){
		print_error_message(CHOOSE_WRONG_COLOR);
		return false;
	}else if(board[move_input->to_row][move_input->to_column].color == turn){
		print_error_message(CAPTURE_OWN_PIECE);
		return false;
	}else if(!is_piece_movement_compatible(board, history, *move_input, turn)){
		print_error_message(INCOMPATIBLE_MOVE);
		return false;
	}else if(is_jump_other_pieces(board, *move_input)){
		print_error_message(JUMP_OTHER_PIECES);
		return false;
	}else if(will_king_be_in_check(board, *history, turn, *move_input)){
		print_error_message(KING_IN_CHECK);
		return false;
	}

	return true;
}

bool is_piece_movement_compatible(square board[][BOARD_SIZE], History *history, move_coordinates move_input, char turn){
	switch(board[move_input.from_row][move_input.from_column].name){
		case BISHOP:
			return is_bishop_movement_valid(move_input);
		case KING:{
			bool is_valid = is_king_movement_valid(move_input);

			if(!is_valid)
				is_valid = is_castle_valid(board, history, move_input, turn);

			return is_valid;
		}case KNIGHT:
			return is_knight_movement_valid(move_input);
		case PAWN:{
			bool is_valid = is_pawn_movement_valid(move_input, turn);

			if(is_valid)
				is_valid = is_pawn_capture_valid(board, history, move_input);

			return is_valid;
		}case QUEEN:
			return is_queen_movement_valid(move_input);
		case ROOK:
			return is_rook_movement_valid(move_input);
	}
}

bool is_bishop_movement_valid(move_coordinates move_input){
	int i, j;

	// Verifying the top left diagonal
	for(i = move_input.from_row - 1, j = move_input.from_column - 1;
	    i >= 0 && j >= 0;
	    i--, j--)
		if(i == move_input.to_row && j == move_input.to_column)
			return true;

	// Verifying the top right diagonal
	for(i = move_input.from_row - 1, j = move_input.from_column + 1;
	    i >= 0 && j <= 7;
	    i--, j++)
		if(i == move_input.to_row && j == move_input.to_column)
			return true;

	// Verifying the bottom left diagonal
	for(i = move_input.from_row + 1, j = move_input.from_column - 1;
	    i <= 7 && j >= 0;
	    i++, j--)
		if(i == move_input.to_row && j == move_input.to_column)
			return true;

	// Verifying the bottom right diagonal
	for(i = move_input.from_row + 1, j = move_input.from_column + 1;
	    i <= 7 && i <= 7;
	    i++, j++)
		if(i == move_input.to_row && j == move_input.to_column)
			return true;

	return false;
}

bool is_king_movement_valid(move_coordinates move_input){
	int i, j;

	for(i = move_input.from_row - 1; i <= move_input.from_row + 1; i++)
		for(j = move_input.from_column - 1; j <= move_input.from_column + 1; j++)
			if(i == move_input.to_row && j == move_input.to_column)
				return true;

	return false;
}

bool are_there_pieces_between(square board[][BOARD_SIZE], int start, int end, int row){
	int i;

	for(i = start + 1; i < end; i++)
		if(board[row][i].name != NO_PIECE)
			return true;

	return false;
}

bool is_king_safe(square board[][BOARD_SIZE], History *history, move_coordinates move, char turn){
	int i;
	move_coordinates temp;

	if(is_player_king_in_check(board, history, turn))
		return false;

	do{
		if(move.from_column < move.to_column)
			move.from_column++;
		else
			move.from_column--;

		if(will_king_be_in_check(board, *history, turn, move))
			return false;
	}while(move.from_column != move.to_column);

	return true;
}

bool is_castle_valid(square board[][BOARD_SIZE], History *history, move_coordinates move, char turn){
	if(turn == WHITE &&
	   !history->castle.has_white_king_moved &&
	   move.to_row == 7){
		if(!history->castle.has_left_white_rook_moved && move.to_column == 2){
			if(!are_there_pieces_between(board, 0, move.from_column, 7) &&
			   is_king_safe(board, history, move, turn)){
			   	history->castle.has_occurred = true;
				return true;
			}
		}else if(!history->castle.has_right_white_rook_moved && move.to_column == 6){
			if(!are_there_pieces_between(board, move.from_column, 7, 7) &&
			   is_king_safe(board, history, move, turn)){
			   	history->castle.has_occurred = true;
				return true;
			}
		}
	}else if(turn == BLACK &&
	         !history->castle.has_black_king_moved &&
	         move.to_row == 0){
		if(!history->castle.has_left_black_rook_moved && move.to_column == 2){
			if(!are_there_pieces_between(board, 0, move.from_column, 0) &&
			   is_king_safe(board, history, move, turn)){
			   	history->castle.has_occurred = true;
				return true;
			}
		}else if(!history->castle.has_right_black_rook_moved && move.to_column == 6){
			if(!are_there_pieces_between(board, move.from_column, 7, 0) &&
			   is_king_safe(board, history, move, turn)){
			   	history->castle.has_occurred = true;
				return true;
			}
		}
	}

	return false;
}

bool is_knight_movement_valid(move_coordinates move_input){
	int diff_row, diff_column;
	
	diff_row    = move_input.to_row    - move_input.from_row;
	diff_column = move_input.to_column - move_input.from_column;

	if(diff_row == 2  && diff_column == 1  ||
	   diff_row == 2  && diff_column == -1 ||
	   diff_row == -2 && diff_column == 1  ||
	   diff_row == -2 && diff_column == -1 ||
	   diff_row == 1  && diff_column == 2  ||
	   diff_row == 1  && diff_column == -2 ||
	   diff_row == -1 && diff_column == 2  ||
	   diff_row == -1 && diff_column == -2)
		return true;

	return false;
}

bool is_pawn_movement_valid(move_coordinates move_input, char turn){
	int advance2Squares = (turn == WHITE? -2 : 2),
	    advance1Square  = (turn == WHITE? -1 : 1);

	if(move_input.from_row == 6 || move_input.from_row == 1)
		if(move_input.from_row + advance2Squares == move_input.to_row &&
		   move_input.from_column == move_input.to_column)
		   	return true;
			
	if(move_input.from_row + advance1Square == move_input.to_row){
		int j;

		for(j = move_input.from_column - 1;
		    j <= move_input.from_column + 1;
		    j++)
			if(j == move_input.to_column)
		    		return true;
	}
	
	return false;
}

bool is_pawn_capture_valid(square board[][BOARD_SIZE], History *history, move_coordinates move_input){
	if(move_input.from_column == move_input.to_column &&
	   board[move_input.to_row][move_input.to_column].name == NO_PIECE)
		return true;

	if(move_input.from_column != move_input.to_column &&
	   board[move_input.to_row][move_input.to_column].name != NO_PIECE)
	   	return true;

	if(is_en_passant_valid(board, history, move_input))
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

bool is_queen_movement_valid(move_coordinates move_input){
	if(is_rook_movement_valid(move_input))
		return true;
	if(is_bishop_movement_valid(move_input))
		return true;
		
	return false;
}

bool is_rook_movement_valid(move_coordinates move_input){
	if(move_input.from_row    == move_input.to_row ||
	   move_input.from_column == move_input.to_column)
		return true;

	return false;
}

bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coordinates move_input){
	if(board[move_input.from_row][move_input.from_column].name != KNIGHT){
		int i = move_input.from_row, j = move_input.from_column;
		
		do{
			if(i < move_input.to_row)
				i++;
			else if(i > move_input.to_row)
				i--;
			
			if(j < move_input.to_column)
				j++;
			else if(j > move_input.to_column)
				j--;
			
			if(board[i][j].name != NO_PIECE && (i != move_input.to_row || j != move_input.to_column))
				return true;
		}while(i != move_input.to_row || j != move_input.to_column);
	}
	
	return false;
}

void move_piece(square board[][BOARD_SIZE], move_coordinates move_input){
	strcpy(board[move_input.to_row][move_input.to_column].image,
	       board[move_input.from_row][move_input.from_column].image);

	board[move_input.to_row][move_input.to_column].name = 
	board[move_input.from_row][move_input.from_column].name;

	board[move_input.to_row][move_input.to_column].color = 
	board[move_input.from_row][move_input.from_column].color;

	strcpy(board[move_input.from_row][move_input.from_column].image, " \0");

	board[move_input.from_row][move_input.from_column].name = NO_PIECE;

	board[move_input.from_row][move_input.from_column].color = NO_PIECE;
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
