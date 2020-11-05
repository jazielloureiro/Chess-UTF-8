#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"

void init_board(square board[][BOARD_SIZE]){
	int i, j;

	for(i = 0; i < 2; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = BLACK;

	sprintf(board[0][0].image, "♜");
	board[0][0].name = ROOK;

	sprintf(board[0][1].image, "♞");
	board[0][1].name = KNIGHT;

	sprintf(board[0][2].image, "♝");
	board[0][2].name = BISHOP;

	sprintf(board[0][3].image, "♛");
	board[0][3].name = QUEEN;

	sprintf(board[0][4].image, "♚");
	board[0][4].name = KING;

	sprintf(board[0][5].image, "♝");
	board[0][5].name = BISHOP;

	sprintf(board[0][6].image, "♞");
	board[0][6].name = KNIGHT;

	sprintf(board[0][7].image, "♜");
	board[0][7].name = ROOK;

	for(j = 0; j < BOARD_SIZE; j++){
		sprintf(board[1][j].image, "♟");
		board[1][j].name = PAWN;
	}

	for(i = 2; i < 6; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			sprintf(board[i][j].image, " ");
			board[i][j].name = NO_PIECE;
			board[i][j].color = NO_PIECE;
		}
	}

	for(i = 6; i < BOARD_SIZE; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = WHITE;

	for(j = 0; j < BOARD_SIZE; j++){
		sprintf(board[6][j].image, "♙");
		board[6][j].name = PAWN;
	}

	sprintf(board[7][0].image, "♖");
	board[7][0].name = ROOK;

	sprintf(board[7][1].image, "♘");
	board[7][1].name = KNIGHT;

	sprintf(board[7][2].image, "♗");
	board[7][2].name = BISHOP;

	sprintf(board[7][3].image, "♕");
	board[7][3].name = QUEEN;

	sprintf(board[7][4].image, "♔");
	board[7][4].name = KING;

	sprintf(board[7][5].image, "♗");
	board[7][5].name = BISHOP;

	sprintf(board[7][6].image, "♘");
	board[7][6].name = KNIGHT;

	sprintf(board[7][7].image, "♖");
	board[7][7].name = ROOK;
}

void init_history(square board[][BOARD_SIZE], History *history){
	history->moves_count = 0;
	history->pieces_count = MAX_PIECES;
	get_current_board(board, history);
}

void init_castle_history(castle_pieces_history *castle_hist){
	castle_hist->is_left_black_rook_moved = false;
	castle_hist->is_right_black_rook_moved = false;
	castle_hist->is_black_king_moved = false;
	castle_hist->is_left_white_rook_moved = false;
	castle_hist->is_right_white_rook_moved = false;
	castle_hist->is_white_king_moved = false;
}

void print_top_menu(char move, bool check){
	if(check){
		putchar('\n');
		printf("\t  ┌───────────────┐   ┌───────────┐\n");
		printf("\t  │ %s to move │   │   Check   │\n", (move == WHITE? "White" : "Black"));
		printf("\t  └───────────────┘   └───────────┘\n\n");
	}else{
		putchar('\n');
		printf("\t\t  ┌───────────────┐\n");
		printf("\t\t  │ %s to move │\n", (move == WHITE? "White" : "Black"));
		printf("\t\t  └───────────────┘\n\n");
	}
}

void print_board(square board[][BOARD_SIZE]){
	int i, j, k = 8;
	
	printf("\t  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

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

void get_current_board(square board[][BOARD_SIZE], History *history){
	int i, j, sqr_counter, *move;

	move = &history->moves_count;

	for(i = 0, sqr_counter = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name != NO_PIECE){
				history->sqr_hist[*move][sqr_counter].name = board[i][j].name;
				history->sqr_hist[*move][sqr_counter].color = board[i][j].color;
				history->sqr_hist[*move][sqr_counter].row = i;
				history->sqr_hist[*move][sqr_counter].column = j;
				sqr_counter++;
			}
		}
	}

	(*move)++;

	/*
	for(i = 0; i < *move; i++){
		printf("X----------Move %d----------X\n", i);

		for(j = 0; j < sqr_counter; j++){
			printf("Piecename: %c\n", history->sqr_hist[i][j].name);
			printf("Color: %c\n", history->sqr_hist[i][j].color);
			printf("Row: %d\n", history->sqr_hist[i][j].row);
			printf("Column: %d\n", history->sqr_hist[i][j].column);
			putchar('\n');
		}
	}
	getchar();
	*/
}

int count_pieces(square board[][BOARD_SIZE]){
	int i, j, pieces_amount;

	for(i = 0, pieces_amount = 0; i < BOARD_SIZE; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			if(board[i][j].name != NO_PIECE)
				pieces_amount++;
	
	return pieces_amount;
}

void save_state_board(square board[][BOARD_SIZE], last_state *movement, movement_input user){
	strcpy(movement->from.image, board[user.from_row][user.from_column].image);
	movement->from.name  = board[user.from_row][user.from_column].name;
	movement->from.color = board[user.from_row][user.from_column].color;
	
	strcpy(movement->to.image, board[user.to_row][user.to_column].image);
	movement->to.name  = board[user.to_row][user.to_column].name;
	movement->to.color = board[user.to_row][user.to_column].color;
}

void return_last_state(square board[][BOARD_SIZE], last_state movement, movement_input user){
	strcpy(board[user.from_row][user.from_column].image, movement.from.image);
	board[user.from_row][user.from_column].name = movement.from.name;
	board[user.from_row][user.from_column].color  = movement.from.color;
	
	strcpy(board[user.to_row][user.to_column].image, movement.to.image);
	board[user.to_row][user.to_column].name = movement.to.name;
	board[user.to_row][user.to_column].color  = movement.to.color;
}
