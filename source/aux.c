#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"

void clear_input_buffer(){
	char buffer;

	while((buffer = getchar()) != '\n' && buffer != EOF);
}

void clear_screen(){
	system("clear");
}

void pause_screen(){
	puts("\nPress Enter to continue");
	
	while(getchar() != '\n');
}

void print_board(square board[][BOARD_SIZE]){
	int i, j, k = 8;
	
	printf("\n\t  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

	for(i = 0; i < BOARD_SIZE; i++){
		printf("\t%d │", k--);

		for(j = 0; j < BOARD_SIZE; j++)
			printf(" %s │", board[i][j].image);

		putchar('\n');

		if(i < BOARD_SIZE - 1)
			printf("\t  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}

	printf("\t  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "\t    a   b   c   d   e   f   g   h\n");
}

void print_error_message(int message){
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
			break;
		case KING_IN_CHECK:
			puts("\nYour can't do this move, because your king will be in check.");
			break;
	}

	pause_screen();
}

void print_final_board(square board[][BOARD_SIZE], char final){
	clear_screen();
	putchar('\n');

	switch(final){
		case WHITE:
			puts("\t  ┌────────────┐      ┌───────────┐\n"
			     "\t  │ Black wins │      │ Checkmate │\n"
			     "\t  └────────────┘      └───────────┘");
			break;
		case BLACK:
			puts("\t  ┌────────────┐      ┌───────────┐\n"
			     "\t  │ White wins │      │ Checkmate │\n"
			     "\t  └────────────┘      └───────────┘");
			break;
		case FIFTY_MOVES:
			puts("\t      ┌──────┐  ┌─────────────┐\n"
			     "\t      │ Draw │  │ Fifty moves │\n"
			     "\t      └──────┘  └─────────────┘");
			break;
		case THREEFOLD_REP:
			puts("\t  ┌──────┐ ┌──────────────────────┐\n"
			     "\t  │ Draw │ │ Threefold repetition │\n"
			     "\t  └──────┘ └──────────────────────┘");
			break;
		case INSUFFICIENT_MAT:
			puts("\t  ┌──────┐┌───────────────────────┐\n"
			     "\t  │ Draw ││ Insufficient material │\n"
			     "\t  └──────┘└───────────────────────┘");
			break;
		case STALEMATE:
			puts("\t      ┌──────┐    ┌───────────┐\n"
			     "\t      │ Draw │    │ Stalemate │\n"
			     "\t      └──────┘    └───────────┘");
	}

	print_board(board);

	pause_screen();
}

void print_top_menu(char turn, bool check){
	putchar('\n');

	if(check){
		printf("\t  ┌───────────────┐   ┌───────────┐\n");
		printf("\t  │ %s to move │   │   Check   │\n", (turn == WHITE? "White" : "Black"));
		printf("\t  └───────────────┘   └───────────┘\n");
	}else{
		printf("\t\t  ┌───────────────┐\n");
		printf("\t\t  │ %s to move │\n", (turn == WHITE? "White" : "Black"));
		printf("\t\t  └───────────────┘\n");
	}
}
