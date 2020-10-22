#include "aux.h"
#include "chess.h"

#include <stdio.h>

void read_square(char *row, char *column){
	*column = getchar();

	*row = getchar();

	clear_input_buffer();
}

void validate_square(char *row, char *column){
	while(*column < 'a' || *column > 'h' || *row < '1' || *row > '8'){
		printf("\nYou've entered an invalid square! Enter a square again: ");
		read_square(row, column);
	}
}

bool do_player_actions(char action, char move){
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

bool verify_player_actions(char row, char column, char move){
	if(column == ':' && (row == 'r' || row == 'd'))
		return do_player_actions(row, move);
		
	return false;
}

void convert_inputs_user(char *row, char *column){
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
