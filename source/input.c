#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "input.h"

move_coord read_input(){
	const int8_t BUFFER_LENGTH = 6;
	char buffer[BUFFER_LENGTH];
	move_coord input;

	printf("\nEnter your movement: ");
	scanf("%5[^\n]s", buffer);
	clear_input_buffer();

	if(buffer[0] == ':' && buffer[1] == 'r')
		input.from_file = ASK_RESIGN;
	else if(buffer[0] == ':' && buffer[1] == 'd')
		input.from_file = ASK_DRAW;
	else{
		input.from_file = convert_file(buffer[0]);
		input.from_rank = convert_rank(buffer[1]);
		input.to_file = convert_file(buffer[3]);
		input.to_rank = convert_rank(buffer[4]);
	}

	return input;
}

int8_t convert_rank(char rank){
	if(rank < '1' || rank > '8')
		return INVALID_SQUARE;

	return '8' - rank;
}

int8_t convert_file(char file){
	if(file < 'a' || file > 'h')
		return INVALID_SQUARE;

	return file - 'a';
}

bool is_action(Player player){
	return player.move.from_file == ASK_RESIGN ||
	       player.move.from_file == ASK_DRAW;
}

bool is_action_confirmed(Player player){
	char choose;
	
	if(player.move.from_file == ASK_RESIGN)
		printf("\nDo you want to resign the game? [y/n] ");
	else
		printf("\nThe %s player offers a draw, does the %s player agree? [y/n] ",
		       player.turn == WHITE? "White" : "Black",
		       player.turn == WHITE? "Black" : "White");

	do{
		choose = getchar();
		clear_input_buffer();
	}while(choose != 'y' && choose != 'n');
	
	if(choose == 'y'){
		if(player.move.from_file == ASK_RESIGN)
			printf("End of the game! The %s player resigned the game!\n",
			       player.turn == WHITE? "White" : "Black");
		else
			printf("End of the game! Both players agreed to a draw!\n");
			
		pause_screen();
			       
		return true;
	}
	
	return false;
}

bool is_the_squares_valid(move_coord move_input){
	return move_input.from_file != INVALID_SQUARE &&
	       move_input.from_rank != INVALID_SQUARE &&
	       move_input.to_file != INVALID_SQUARE &&
	       move_input.to_rank != INVALID_SQUARE;
}

bool is_there_promotion(square board[][BOARD_SIZE], move_coord move){
	return board[move.to_rank][move.to_file].name == PAWN &&
	       (move.to_rank == 0 || move.to_rank == 7);
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
