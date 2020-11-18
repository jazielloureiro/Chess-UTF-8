#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"

void read_movement_input(move_coordinates *move_input){
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

bool is_the_squares_valid(move_coordinates *move_input){
	if(move_input->from_column == INVALID_SQUARE ||
	   move_input->from_row    == INVALID_SQUARE ||
	   move_input->to_column   == INVALID_SQUARE ||
	   move_input->to_row      == INVALID_SQUARE)
	   	return false;

	return true;
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

bool is_there_player_action(Player player){
	if(player.move.from_column == ':' &&
	   (player.move.from_row == 'r' ||
	   player.move.from_row == 'd'))
		return is_player_action_valid(player.move.from_row, player.turn);
		
	return false;
}

void convert_row(char *row){
	switch(*row){
		case '8': *row = 0; break;
		case '7': *row = 1; break;
		case '6': *row = 2; break;
		case '5': *row = 3; break;
		case '4': *row = 4; break;
		case '3': *row = 5; break;
		case '2': *row = 6; break;
		case '1': *row = 7; break;
		default:  *row = INVALID_SQUARE;
	}
}

void convert_column(char *column){
	if(*column < 'a' || *column > 'h')
		*column = INVALID_SQUARE;
	else
		*column -= 'a';
}

void convert_movement_input(move_coordinates *move){
	convert_row(&move->from_row);
	convert_column(&move->from_column);
	convert_row(&move->to_row);
	convert_column(&move->to_column);
}

bool is_there_promotion(square board[][BOARD_SIZE], move_coordinates move){
	if(board[move.to_row][move.to_column].name == PAWN &&
	   (move.to_row == 0 || move.to_row == 7))
		return true;

	return false;
}
