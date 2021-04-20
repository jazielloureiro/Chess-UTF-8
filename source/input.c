#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"

void read_movement_input(move_coord *move_input){
	printf("\nEnter your movement: ");

	move_input->from_column = getchar();
	move_input->from_row = getchar();

	if(move_input->from_column == ':'){
		clear_input_buffer();
		return;
	}

	getchar();

	move_input->to_column = getchar();
	move_input->to_row = getchar();

	clear_input_buffer();
}

bool is_there_player_action(Player player){
	if(player.move.from_column == ':' &&
	   (player.move.from_row == 'r' ||
	   player.move.from_row == 'd'))
		return is_player_action_valid(player.move.from_row, player.turn);
		
	return false;
}

bool is_player_action_valid(char action, char turn){
	char choose;
	
	if(action == 'r')
		printf("\nDo you want to resign the game? [y/n] ");
	else
		printf("\nThe %s player offers a draw, does the %s player agree? [y/n] ",
		       turn == WHITE? "White" : "Black",
		       turn == WHITE? "Black" : "White");

	do{
		choose = getchar();
		clear_input_buffer();
	}while(choose != 'y' && choose != 'n');
	
	if(choose == 'y'){
		if(action == 'r')
			printf("End of the game! The %s player resigned the game!\n",
			       turn == WHITE? "White" : "Black");
		else
			printf("End of the game! Both players agreed to a draw!\n");
			
		pause_screen();
			       
		return true;
	}
	
	return false;
}

void convert_movement_input(move_coord *move){
	move->from_row = convert_row(move->from_row);
	move->from_column = convert_column(move->from_column);
	move->to_row = convert_row(move->to_row);
	move->to_column = convert_column(move->to_column);
}

char convert_row(char row){
	if(row < '1' || row > '8')
		return INVALID_SQUARE;

	return '8' - row;
}

char convert_column(char column){
	if(column < 'a' || column > 'h')
		return INVALID_SQUARE;

	return column - 'a';
}

bool is_the_squares_valid(move_coord move_input){
	return move_input.from_column != INVALID_SQUARE &&
	       move_input.from_row != INVALID_SQUARE &&
	       move_input.to_column != INVALID_SQUARE &&
	       move_input.to_row != INVALID_SQUARE;
}

bool is_there_promotion(square board[][BOARD_SIZE], move_coord move){
	return board[move.to_row][move.to_column].name == PAWN &&
	       (move.to_row == 0 || move.to_row == 7);
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
			piece->image = (turn == WHITE? "♕" : "♛");
			piece->name = QUEEN;
			break;
		case '2':
			piece->image = (turn == WHITE? "♖" : "♜");
			piece->name = ROOK;
			break;
		case '3':
			piece->image = (turn == WHITE? "♗" : "♝");
			piece->name = BISHOP;
			break;
		case '4':
			piece->image = (turn == WHITE? "♘" : "♞");
			piece->name = KNIGHT;
	}       
}
