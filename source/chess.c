#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
	history->moves_counter = 0;
	history->pieces_counter = MAX_PIECES;
	get_current_board(board, history);

	history->castle.has_left_black_rook_moved = false;
	history->castle.has_right_black_rook_moved = false;
	history->castle.has_black_king_moved = false;
	history->castle.has_left_white_rook_moved = false;
	history->castle.has_right_white_rook_moved = false;
	history->castle.has_white_king_moved = false;
	history->castle.has_occurred = false;

	history->has_en_passant_occurred = false;
}

bool has_pawn_moved(History history, move_coordinates move){
	int i, last_board;

	for(i = 0, last_board = history.moves_counter - 1;
	    i < history.pieces_counter;
	    i++){
		if(history.sqr_hist[last_board][i].row    == move.from_row &&
		   history.sqr_hist[last_board][i].column == move.from_column){
			if(history.sqr_hist[last_board][i].name == PAWN)
				return true;
			else
				break;
		}
	}

	return false;
}

void update_castle_history(square board[][BOARD_SIZE], History *history, move_coordinates move){
	if(board[move.to_row][move.to_column].name == KING){
		if(move.from_row == 0 && move.from_column == 4)
			history->castle.has_black_king_moved = true;
		else if(move.from_row == 7 && move.from_column == 4)
			history->castle.has_white_king_moved = true;
	}else if(board[move.to_row][move.to_column].name == ROOK){
		if(move.from_row == 0 && move.from_column == 0)
			history->castle.has_left_black_rook_moved = true;
		else if(move.from_row == 0 && move.from_column == 7)
			history->castle.has_right_black_rook_moved = true;
		else if(move.from_row == 7 && move.from_column == 0)
			history->castle.has_left_white_rook_moved = true;
		else if(move.from_row == 7 && move.from_column == 7)
			history->castle.has_right_white_rook_moved = true;
	}

	history->castle.has_occurred = false;
}

void update_last_input(move_coordinates *last_input, move_coordinates move){
	last_input->from_row    = move.from_row;
	last_input->from_column = move.from_column;
	last_input->to_row      = move.to_row;
	last_input->to_column   = move.to_column;
}

void update_history(square board[][BOARD_SIZE], History *history, move_coordinates move){
	if(count_pieces(board) < history->pieces_counter){
		history->pieces_counter--;
		history->moves_counter = 0;
	}else if(has_pawn_moved(*history, move))
		history->moves_counter = 0;

	get_current_board(board, history);

	update_castle_history(board, history, move);

	update_last_input(&history->last_input, move);

	history->has_en_passant_occurred = false;
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

void get_current_board(square board[][BOARD_SIZE], History *history){
	int i, j, sqr_counter, *move;

	move = &history->moves_counter;

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
}

int count_pieces(square board[][BOARD_SIZE]){
	int i, j, pieces_amount;

	for(i = 0, pieces_amount = 0; i < BOARD_SIZE; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			if(board[i][j].name != NO_PIECE)
				pieces_amount++;
	
	return pieces_amount;
}

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coordinates move_input){
	strcpy(move_squares->from.image, board[move_input.from_row][move_input.from_column].image);
	move_squares->from.name  = board[move_input.from_row][move_input.from_column].name;
	move_squares->from.color = board[move_input.from_row][move_input.from_column].color;
	
	strcpy(move_squares->to.image, board[move_input.to_row][move_input.to_column].image);
	move_squares->to.name  = board[move_input.to_row][move_input.to_column].name;
	move_squares->to.color = board[move_input.to_row][move_input.to_column].color;
}

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coordinates move_input){
	strcpy(board[move_input.from_row][move_input.from_column].image, move_squares.from.image);
	board[move_input.from_row][move_input.from_column].name = move_squares.from.name;
	board[move_input.from_row][move_input.from_column].color  = move_squares.from.color;
	
	strcpy(board[move_input.to_row][move_input.to_column].image, move_squares.to.image);
	board[move_input.to_row][move_input.to_column].name = move_squares.to.name;
	board[move_input.to_row][move_input.to_column].color  = move_squares.to.color;
}

void en_passant(square board[][BOARD_SIZE], History history){
	move_coordinates move;

	move.from_row = history.last_input.to_row;
	move.from_column = history.last_input.to_column;
	move.to_row = history.last_input.to_row;
	move.to_column = history.last_input.to_column;

	move_piece(board, move);
}

void castle(square board[][BOARD_SIZE], move_coordinates move){
	move_coordinates rook;

	if(move.to_row == 0){
		if(move.to_column == 2){
			rook.from_row = 0;
			rook.from_column = 0;
			rook.to_row = 0;
			rook.to_column = 3;
		}else{
			rook.from_row = 0;
			rook.from_column = 7;
			rook.to_row = 0;
			rook.to_column = 5;
		}
	}else{
		if(move.to_column == 2){
			rook.from_row = 7;
			rook.from_column = 0;
			rook.to_row = 7;
			rook.to_column = 3;
		}else{
			rook.from_row = 7;
			rook.from_column = 7;
			rook.to_row = 7;
			rook.to_column = 5;
		}
	}

	move_piece(board, rook);
}
