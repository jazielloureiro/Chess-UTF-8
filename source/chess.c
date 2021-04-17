#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "movement.h"
#include "input.h"

void init_board(square board[][BOARD_SIZE]){
	int i, j;

	for(i = 0; i < 2; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = BLACK;

	board[0][0].image = "♜";
	board[0][0].name = ROOK;

	board[0][1].image = "♞";
	board[0][1].name = KNIGHT;

	board[0][2].image = "♝";
	board[0][2].name = BISHOP;

	board[0][3].image = "♛";
	board[0][3].name = QUEEN;

	board[0][4].image = "♚";
	board[0][4].name = KING;

	board[0][5].image = "♝";
	board[0][5].name = BISHOP;

	board[0][6].image = "♞";
	board[0][6].name = KNIGHT;

	board[0][7].image = "♜";
	board[0][7].name = ROOK;

	for(j = 0; j < BOARD_SIZE; j++){
		board[1][j].image = "♟";
		board[1][j].name = PAWN;
	}

	for(i = 2; i < 6; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			board[i][j].image = " ";
			board[i][j].name = NO_PIECE;
			board[i][j].color = NO_PIECE;
		}
	}

	for(i = 6; i < BOARD_SIZE; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = WHITE;

	for(j = 0; j < BOARD_SIZE; j++){
		board[6][j].image = "♙";
		board[6][j].name = PAWN;
	}

	board[7][0].image = "♖";
	board[7][0].name = ROOK;

	board[7][1].image = "♘";
	board[7][1].name = KNIGHT;

	board[7][2].image = "♗";
	board[7][2].name = BISHOP;

	board[7][3].image = "♕";
	board[7][3].name = QUEEN;

	board[7][4].image = "♔";
	board[7][4].name = KING;

	board[7][5].image = "♗";
	board[7][5].name = BISHOP;

	board[7][6].image = "♘";
	board[7][6].name = KNIGHT;

	board[7][7].image = "♖";
	board[7][7].name = ROOK;
}

void init_history(History *history){
	history->board = NULL;

	history->moves_counter = 0;
}

void update_history(square board[][BOARD_SIZE], History *history, Player player){
	if(board[player.move.from_row][player.move.from_column].name == PAWN ||
	   board[player.move.to_row][player.move.to_column].name != NO_PIECE)
		history->moves_counter = 0;

	get_current_board(board, history);

	history->board->player = player;

	history->moves_counter++;
}

void get_current_board(square board[][BOARD_SIZE], History *history){
	h_board *Hboard = malloc(sizeof(h_board));

	Hboard->pieces_qty = count_pieces(board);

	Hboard->pieces = malloc(sizeof(h_square) * Hboard->pieces_qty);

	for(int i = 0, sqr_counter = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name != NO_PIECE){
				Hboard->pieces[sqr_counter].name = board[i][j].name;
				Hboard->pieces[sqr_counter].color = board[i][j].color;
				Hboard->pieces[sqr_counter].row = i;
				Hboard->pieces[sqr_counter].column = j;
				sqr_counter++;
			}
		}
	}

	Hboard->prev = history->board;
	history->board = Hboard;
}

int count_pieces(square board[][BOARD_SIZE]){
	int pieces_qty = 0;

	for(int i = 0; i < BOARD_SIZE; i++)
		for(int j = 0; j < BOARD_SIZE; j++)
			if(board[i][j].name != NO_PIECE)
				pieces_qty++;
	
	return pieces_qty;
}

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coordinates move){
	move_squares->from.image = board[move.from_row][move.from_column].image;
	move_squares->from.name  = board[move.from_row][move.from_column].name;
	move_squares->from.color = board[move.from_row][move.from_column].color;
	
	move_squares->to.image = board[move.to_row][move.to_column].image;
	move_squares->to.name  = board[move.to_row][move.to_column].name;
	move_squares->to.color = board[move.to_row][move.to_column].color;
}

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coordinates move){
	board[move.from_row][move.from_column].image = move_squares.from.image;
	board[move.from_row][move.from_column].name = move_squares.from.name;
	board[move.from_row][move.from_column].color = move_squares.from.color;
	
	board[move.to_row][move.to_column].image = move_squares.to.image;
	board[move.to_row][move.to_column].name = move_squares.to.name;
	board[move.to_row][move.to_column].color  = move_squares.to.color;
}

void find_castle_rook(move_coordinates move, move_coordinates *rook){
	if(move.to_row == 0){
		rook->from_row = 0;
		rook->to_row = 0;

		if(move.to_column == 2){
			rook->from_column = 0;
			rook->to_column = 3;
		}else{
			rook->from_column = 7;
			rook->to_column = 5;
		}
	}else{
		rook->from_row = 7;
		rook->to_row = 7;

		if(move.to_column == 2){
			rook->from_column = 0;
			rook->to_column = 3;
		}else{
			rook->from_column = 7;
			rook->to_column = 5;
		}
	}
}

void castle(square board[][BOARD_SIZE], move_coordinates move){
	move_coordinates rook;

	find_castle_rook(move, &rook);

	move_piece(board, rook);
}

void advance_to(char *from, char to){
	if(*from < to)
		(*from)++;
	else if(*from > to)
		(*from)--;
}
