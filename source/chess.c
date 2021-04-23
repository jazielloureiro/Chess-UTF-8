#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "movement.h"
#include "input.h"

void init_board(square board[][BOARD_SIZE]){
	for(int8_t i = 0; i < 2; i++)
		for(int8_t j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = BLACK;

	board[0][0].image = "♜";
	board[0][0].piece = ROOK;

	board[0][1].image = "♞";
	board[0][1].piece = KNIGHT;

	board[0][2].image = "♝";
	board[0][2].piece = BISHOP;

	board[0][3].image = "♛";
	board[0][3].piece = QUEEN;

	board[0][4].image = "♚";
	board[0][4].piece = KING;

	board[0][5].image = "♝";
	board[0][5].piece = BISHOP;

	board[0][6].image = "♞";
	board[0][6].piece = KNIGHT;

	board[0][7].image = "♜";
	board[0][7].piece = ROOK;

	for(int8_t j = 0; j < BOARD_SIZE; j++){
		board[1][j].image = "♟";
		board[1][j].piece = PAWN;
	}

	for(int8_t i = 2; i < 6; i++){
		for(int8_t j = 0; j < BOARD_SIZE; j++){
			board[i][j].image = " ";
			board[i][j].piece = EMPTY;
			board[i][j].color = EMPTY;
		}
	}

	for(int8_t i = 6; i < BOARD_SIZE; i++)
		for(int8_t j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = WHITE;

	for(int8_t j = 0; j < BOARD_SIZE; j++){
		board[6][j].image = "♙";
		board[6][j].piece = PAWN;
	}

	board[7][0].image = "♖";
	board[7][0].piece = ROOK;

	board[7][1].image = "♘";
	board[7][1].piece = KNIGHT;

	board[7][2].image = "♗";
	board[7][2].piece = BISHOP;

	board[7][3].image = "♕";
	board[7][3].piece = QUEEN;

	board[7][4].image = "♔";
	board[7][4].piece = KING;

	board[7][5].image = "♗";
	board[7][5].piece = BISHOP;

	board[7][6].image = "♘";
	board[7][6].piece = KNIGHT;

	board[7][7].image = "♖";
	board[7][7].piece = ROOK;
}

void init_history(square board[][BOARD_SIZE], History *history){
	history->board = get_current_board(board);
	history->board->prev = NULL;

	history->has_castle_occurred = false;
	history->has_en_passant_occurred = false;

	history->moves_counter = 0;
}

void init_player(Player *player){
	player->turn = WHITE;
	player->is_in_check = false;
}

void update_history(square board[][BOARD_SIZE], History *history, Player player){
	history->board->player = player;

	if(board[player.move.from_rank][player.move.from_file].piece == PAWN ||
	   board[player.move.to_rank][player.move.to_file].piece != EMPTY)
		history->moves_counter = 0;

	history->moves_counter++;
}

void add_board_to_history(square board[][BOARD_SIZE], History *history){
	h_board *new = get_current_board(board);

	new->prev = history->board;
	history->board = new;
}

h_board *get_current_board(square board[][BOARD_SIZE]){
	h_board *hboard = malloc(sizeof(h_board));

	hboard->pieces_qty = count_pieces(board);

	hboard->pieces = malloc(sizeof(h_square) * hboard->pieces_qty);

	for(int8_t i = 0, sqr_counter = 0; i < BOARD_SIZE; i++){
		for(int8_t j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].piece != EMPTY){
				hboard->pieces[sqr_counter].piece = board[i][j].piece;
				hboard->pieces[sqr_counter].color = board[i][j].color;
				hboard->pieces[sqr_counter].rank = i;
				hboard->pieces[sqr_counter].file = j;
				sqr_counter++;
			}
		}
	}

	return hboard;
}

int8_t count_pieces(square board[][BOARD_SIZE]){
	int8_t pieces_qty = 0;

	for(int8_t i = 0; i < BOARD_SIZE; i++)
		for(int8_t j = 0; j < BOARD_SIZE; j++)
			if(board[i][j].piece != EMPTY)
				pieces_qty++;
	
	return pieces_qty;
}

void move_piece(square board[][BOARD_SIZE], move_coord move){
	board[move.to_rank][move.to_file].image =
	board[move.from_rank][move.from_file].image;

	board[move.to_rank][move.to_file].piece = 
	board[move.from_rank][move.from_file].piece;

	board[move.to_rank][move.to_file].color = 
	board[move.from_rank][move.from_file].color;

	board[move.from_rank][move.from_file].image = " ";

	board[move.from_rank][move.from_file].piece = EMPTY;

	board[move.from_rank][move.from_file].color = EMPTY;
}

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coord move){
	move_squares->from.image = board[move.from_rank][move.from_file].image;
	move_squares->from.piece = board[move.from_rank][move.from_file].piece;
	move_squares->from.color = board[move.from_rank][move.from_file].color;
	
	move_squares->to.image = board[move.to_rank][move.to_file].image;
	move_squares->to.piece = board[move.to_rank][move.to_file].piece;
	move_squares->to.color = board[move.to_rank][move.to_file].color;
}

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coord move){
	board[move.from_rank][move.from_file].image = move_squares.from.image;
	board[move.from_rank][move.from_file].piece = move_squares.from.piece;
	board[move.from_rank][move.from_file].color = move_squares.from.color;
	
	board[move.to_rank][move.to_file].image = move_squares.to.image;
	board[move.to_rank][move.to_file].piece = move_squares.to.piece;
	board[move.to_rank][move.to_file].color = move_squares.to.color;
}

move_coord get_castle_rook(move_coord move){
	move_coord rook;

	rook.from_rank = move.from_rank;
	rook.to_rank = move.to_rank;

	if(move.to_file == 2){
		rook.from_file = 0;
		rook.to_file = 3;
	}else{
		rook.from_file = 7;
		rook.to_file = 5;
	}

	return rook;
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
