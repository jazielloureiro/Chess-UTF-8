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
			board[i][j].name = EMPTY;
			board[i][j].color = EMPTY;
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

	history->has_castle_occurred = false;
	history->has_en_passant_occurred = false;

	history->moves_counter = 0;
}

void init_player(Player *player){
	player->turn = WHITE;
	player->is_in_check = false;
}

void update_history(square board[][BOARD_SIZE], History *history, Player player){
	if(board[player.move.from_rank][player.move.from_file].name == PAWN ||
	   board[player.move.to_rank][player.move.to_file].name != EMPTY)
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
			if(board[i][j].name != EMPTY){
				Hboard->pieces[sqr_counter].name = board[i][j].name;
				Hboard->pieces[sqr_counter].color = board[i][j].color;
				Hboard->pieces[sqr_counter].rank = i;
				Hboard->pieces[sqr_counter].file = j;
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
			if(board[i][j].name != EMPTY)
				pieces_qty++;
	
	return pieces_qty;
}

void move_piece(square board[][BOARD_SIZE], move_coord move){
	board[move.to_rank][move.to_file].image =
	board[move.from_rank][move.from_file].image;

	board[move.to_rank][move.to_file].name = 
	board[move.from_rank][move.from_file].name;

	board[move.to_rank][move.to_file].color = 
	board[move.from_rank][move.from_file].color;

	board[move.from_rank][move.from_file].image = " ";

	board[move.from_rank][move.from_file].name = EMPTY;

	board[move.from_rank][move.from_file].color = EMPTY;
}

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coord move){
	move_squares->from.image = board[move.from_rank][move.from_file].image;
	move_squares->from.name = board[move.from_rank][move.from_file].name;
	move_squares->from.color = board[move.from_rank][move.from_file].color;
	
	move_squares->to.image = board[move.to_rank][move.to_file].image;
	move_squares->to.name = board[move.to_rank][move.to_file].name;
	move_squares->to.color = board[move.to_rank][move.to_file].color;
}

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coord move){
	board[move.from_rank][move.from_file].image = move_squares.from.image;
	board[move.from_rank][move.from_file].name = move_squares.from.name;
	board[move.from_rank][move.from_file].color = move_squares.from.color;
	
	board[move.to_rank][move.to_file].image = move_squares.to.image;
	board[move.to_rank][move.to_file].name = move_squares.to.name;
	board[move.to_rank][move.to_file].color = move_squares.to.color;
}

void find_castle_rook(move_coord move, move_coord *rook){
	if(move.to_rank == 0){
		rook->from_rank = 0;
		rook->to_rank = 0;

		if(move.to_file == 2){
			rook->from_file = 0;
			rook->to_file = 3;
		}else{
			rook->from_file = 7;
			rook->to_file = 5;
		}
	}else{
		rook->from_rank = 7;
		rook->to_rank = 7;

		if(move.to_file == 2){
			rook->from_file = 0;
			rook->to_file = 3;
		}else{
			rook->from_file = 7;
			rook->to_file = 5;
		}
	}
}

void castle(square board[][BOARD_SIZE], move_coord move){
	move_coord rook;

	find_castle_rook(move, &rook);

	move_piece(board, rook);
}

void advance_to(char *from, char to){
	if(*from < to)
		(*from)++;
	else if(*from > to)
		(*from)--;
}

void free_history(h_board *aux){
	if(aux != NULL){
		free_history(aux->prev);
		free(aux->pieces);
		free(aux);
	}
}
