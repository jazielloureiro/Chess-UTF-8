#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "movement.h"

bool is_the_movement_valid(square board[][BOARD_SIZE], movement_input *move_input, char player_move){
	char message = VALID_MOVEMENT;

	convert_square_readed(&move_input->from_row, &move_input->from_column);
	convert_square_readed(&move_input->to_row, &move_input->to_column);

	if(!is_the_squares_valid(move_input))
		message = INVALID_SQUARE;
	else if(board[move_input->from_row][move_input->from_column].color != player_move)
		message = CHOOSE_WRONG_COLOR;
	else if(board[move_input->to_row][move_input->to_column].color == player_move)
		message = CAPTURE_OWN_PIECE;
	else if(!validate_movement(board, *move_input, player_move))
		message = INCOMPATIBLE_MOVE;
	else if(verify_collision(board, *move_input))
		message = JUMP_OTHER_PIECES;

	if(message != VALID_MOVEMENT){
		switch(message){
			case INVALID_SQUARE:
				puts("\nYou've entered an invalid square!");
				break;
			case CHOOSE_WRONG_COLOR:
				puts("\nYou must choose a piece of your color.");
				break;
			case CAPTURE_OWN_PIECE:
				puts("\nYou can't capture your own piece!");
				break;
			case INCOMPATIBLE_MOVE:
				puts("\nThis movement is incompatible with your piece.");
				break;
			case JUMP_OTHER_PIECES:
				puts("\nYour piece can't jump over other pieces!");
		}

		pause();

		return false;
	}

	return true;
}

bool validate_movement(square board[][BOARD_SIZE], movement_input user, char move){
	switch(board[user.from_row][user.from_column].name){
		case BISHOP: return validate_movement_bishop(user);
		case KING:   return validate_movement_king(user);
		case KNIGHT: return validate_movement_knight(user);
		case PAWN:   return validate_movement_pawn(user, move);
		case QUEEN:  return validate_movement_queen(user);
		case ROOK:   return validate_movement_rook(user);
	}
}

bool validate_movement_bishop(movement_input user){
	int i, j;

	// Verifying the top left diagonal
	for(i = user.from_row - 1, j = user.from_column - 1;
	    i >= 0 && j >= 0;
	    i--, j--)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the top right diagonal
	for(i = user.from_row - 1, j = user.from_column + 1;
	    i >= 0 && j <= 7;
	    i--, j++)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the bottom left diagonal
	for(i = user.from_row + 1, j = user.from_column - 1;
	    i <= 7 && j >= 0;
	    i++, j--)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the bottom right diagonal
	for(i = user.from_row + 1, j = user.from_column + 1;
	    i <= 7 && i <= 7;
	    i++, j++)
		if(i == user.to_row && j == user.to_column)
			return true;

	return false;
}

bool validate_movement_king(movement_input user){
	int i, j;

	for(i = user.from_row - 1; i <= user.from_row + 1; i++)
		for(j = user.from_column - 1; j <= user.from_column + 1; j++)
			if(i == user.to_row && j == user.to_column)
				return true;

	return false;
}

bool validate_movement_knight(movement_input user){
	int diff_row, diff_column;
	
	diff_row    = user.to_row    - user.from_row;
	diff_column = user.to_column - user.from_column;

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

bool validate_movement_pawn(movement_input user, char move){
	int advance2Squares = (move == WHITE? -2 : 2),
	    advance1Square  = (move == WHITE? -1 : 1);

	if(user.from_row == 6 || user.from_row == 1)
		if(user.from_row + advance2Squares == user.to_row &&
		   user.from_column == user.to_column)
		   	return true;
			
	if(user.from_row + advance1Square == user.to_row){
		int j;

		for(j = user.from_column - 1;
		    j <= user.from_column + 1;
		    j++)
			if(j == user.to_column)
		    		return true;
	}
	
	return false;
}

bool validate_movement_queen(movement_input user){
	if(validate_movement_rook(user) == true)
		return true;
	if(validate_movement_bishop(user) == true)
		return true;
		
	return false;
}

bool validate_movement_rook(movement_input user){
	if(user.from_row == user.to_row)
		return true;
	if(user.from_column == user.to_column)
		return true;

	return false;
}

bool verify_collision(square board[][BOARD_SIZE], movement_input user){
	if(board[user.from_row][user.from_column].name == PAWN){
		if(user.from_row  != user.to_row  &&
		   user.from_column != user.to_column &&
		   board[user.to_row][user.to_column].name == NO_PIECE)
		   	return true;
	}
	
	if(board[user.from_row][user.from_column].name != KNIGHT){
		int i = user.from_row, j = user.from_column;
		
		do{
			if(i < user.to_row)
				i++;
			else if(i > user.to_row)
				i--;
			
			if(j < user.to_column)
				j++;
			else if(j > user.to_column)
				j--;
			
			if(board[i][j].name != NO_PIECE && (i != user.to_row || j != user.to_column))
				return true;
		}while(i != user.to_row || j != user.to_column);
	}
	
	return false;
}

void move_piece(square board[][BOARD_SIZE], movement_input user){
	strcpy(board[user.to_row][user.to_column].image,
	       board[user.from_row][user.from_column].image);

	board[user.to_row][user.to_column].name = 
	board[user.from_row][user.from_column].name;

	board[user.to_row][user.to_column].color = 
	board[user.from_row][user.from_column].color;

	strcpy(board[user.from_row][user.from_column].image, " \0");

	board[user.from_row][user.from_column].name = NO_PIECE;

	board[user.from_row][user.from_column].color = NO_PIECE;
}

void promotion(square *piece, char move){
	char choose;

	puts("\nFor which piece do you want to promove? ");
	
	if(move == WHITE)
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
			sprintf(piece->image, move == WHITE? "♕" : "♛");
			piece->name = QUEEN;
			break;
		case '2':
			sprintf(piece->image, move == WHITE? "♖" : "♜");
			piece->name = ROOK;
			break;
		case '3':
			sprintf(piece->image, move == WHITE? "♗" : "♝");
			piece->name = BISHOP;
			break;
		case '4':
			sprintf(piece->image, move == WHITE? "♘" : "♞");
			piece->name = KNIGHT;
			break;
	}       
}
