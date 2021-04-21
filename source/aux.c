#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"

void clear_input_buffer(){
	char buffer;

	while((buffer = getchar()) != '\n' && buffer != EOF);
}

void clear_screen(){
	#if defined(_WIN32)
		system("cls");
	#else
		system("clear");
	#endif
}

void pause_screen(){
	puts("\nPress Enter to continue");
	
	while(getchar() != '\n');
}

void print_board(square board[][BOARD_SIZE]){
	printf("\n\t  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

	for(int8_t i = 0, k = 8; i < BOARD_SIZE; i++, k--){
		printf("\t%d │", k);

		for(int8_t j = 0; j < BOARD_SIZE; j++)
			printf(" %s │", board[i][j].image);

		putchar('\n');

		if(i < BOARD_SIZE - 1)
			printf("\t  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}

	printf("\t  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "\t    a   b   c   d   e   f   g   h\n");
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
	if(check){
		printf("\n\t  ┌───────────────┐   ┌───────────┐\n"
		       "\t  │ %s to move │   │   Check   │\n"
		       "\t  └───────────────┘   └───────────┘\n",
		       (turn == WHITE? "White" : "Black"));
	}else{
		printf("\n\t          ┌───────────────┐\n"
		       "\t          │ %s to move │\n"
		       "\t          └───────────────┘\n",
		       (turn == WHITE? "White" : "Black"));
	}
}
