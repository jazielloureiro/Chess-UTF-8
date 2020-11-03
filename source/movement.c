#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aux.h"
#include "chess.h"
#include "input.h"
#include "movement.h"

bool validate_movement(char selected_piece, inputs user, char move){
	switch(selected_piece){
		case BISHOP: return validate_movement_bishop(user);
		case KING:   return validate_movement_king(user);
		case KNIGHT: return validate_movement_knight(user);
		case PAWN:   return validate_movement_pawn(user, move);
		case QUEEN:  return validate_movement_queen(user);
		case ROOK:   return validate_movement_rook(user);
	}
}

bool validate_movement_bishop(inputs user){
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

bool validate_movement_king(inputs user){
	int i, j;

	for(i = user.from_row - 1; i <= user.from_row + 1; i++)
		for(j = user.from_column - 1; j <= user.from_column + 1; j++)
			if(i == user.to_row && j == user.to_column)
				return true;

	return false;
}

bool validate_movement_knight(inputs user){
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

bool validate_movement_pawn(inputs user, char move){
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

bool validate_movement_queen(inputs user){
	if(validate_movement_rook(user) == true)
		return true;
	if(validate_movement_bishop(user) == true)
		return true;
		
	return false;
}

bool validate_movement_rook(inputs user){
	if(user.from_row == user.to_row)
		return true;
	if(user.from_column == user.to_column)
		return true;

	return false;
}

bool verify_collision(char selected_piece, square board[][BOARD_SIZE], inputs user){
	if(selected_piece == PAWN){
		if(user.from_row  != user.to_row  &&
		   user.from_column != user.to_column &&
		   board[user.to_row][user.to_column].name == NO_PIECE)
		   	return true;
	}
	
	if(selected_piece != KNIGHT){
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

void move_piece(square board[][BOARD_SIZE], inputs user){
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
