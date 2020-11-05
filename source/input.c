#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"

void read_movement_input(movement_input *move_input){
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

bool is_the_squares_valid(movement_input *move_input){
	if(move_input->from_column == INVALID_SQUARE ||
	   move_input->from_row    == INVALID_SQUARE ||
	   move_input->to_column   == INVALID_SQUARE ||
	   move_input->to_row      == INVALID_SQUARE)
	   	return false;

	return true;
}

bool is_player_action_valid(char action, char move){
	char choose;
	
	if(action == 'r')
		printf("Do you want to resign the game? [y/n] ");
	else
		printf("The %s player offers a draw, does the %s player agree? [y/n] ",
		       move == WHITE? "White" : "Black",
		       move == WHITE? "Black" : "White");

	do{
		choose = getchar();
		clear_input_buffer();
	}while(choose != 'y' && choose != 'n');
	
	if(choose == 'y'){
		if(action == 'r')
			printf("End of the game! The %s player resigned the game!\n",
			       move == WHITE? "White" : "Black");
		else
			printf("End of the game! Both players agreed to a draw!\n");
			
		pause();
			       
		return true;
	}
	
	return false;
}

bool has_player_action(char row, char column, char move){
	if(column == ':' && (row == 'r' || row == 'd'))
		return is_player_action_valid(row, move);
		
	return false;
}

void convert_square_readed(char *row, char *column){
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

	if(*column < 'a' || *column > 'h')
		*column = INVALID_SQUARE;
	else
		*column -= 'a';
}
