#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"

void read_movement_input(movement_input *move_input){
	char space_input;

	printf("\nEnter your movement: ");

	read_square(&move_input->from_row, &move_input->from_column);

	if(move_input->from_column == ':'){
		clear_input_buffer();
		return;
	}

	space_input = getchar();

	read_square(&move_input->to_row, &move_input->to_column);

	clear_input_buffer();
}

void read_square(char *row, char *column){
	*column = getchar();
	*row = getchar();
}

bool is_the_squares_valid(movement_input *move_input){
	if(is_square_valid(move_input->from_row, move_input->from_column) &&
	   is_square_valid(move_input->to_row, move_input->to_column))
	   	return true;

	return false;
}

bool is_square_valid(char row, char column){
	if(column < 'a' || column > 'h' || row < '1' || row > '8')
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

bool has_player_action(char row, char column){
	if(column == ':' && (row == 'r' || row == 'd'))
		return true;
		
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
		case '1': *row = 7;
	}

	*column -= 'a';
}
