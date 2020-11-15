#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

void play(){
	square board[BOARD_SIZE][BOARD_SIZE];
	History history;
	char player_move = WHITE;

	init_board(board);
	init_history(board, &history);

	do{
		move_coordinates move_input, check;
		bool is_check;

		is_check = is_player_king_in_check(board, &history, player_move, &check);

		if(is_check){
			if(has_checkmate(board, &history, check, player_move)){
				print_final_board(board, player_move);
				return;
			}
		}else{
			if(has_stalemate(board, &history, player_move)){
				print_final_board(board, STALEMATE);
				return;
			}
		}

		do{
			clear_screen();
			print_top_menu(player_move, is_check);
			print_board(board);

			read_movement_input(&move_input);

			if(has_player_action(move_input.from_row,
			                     move_input.from_column,
					     player_move))
				return;
		}while(!is_movement_valid(board, &history, &move_input, player_move));

		move_piece(board, move_input);

		if(history.has_en_passant_occurred)
			en_passant(board, history);

		if(history.castle.has_occurred)
			castle(board, move_input);

		if(is_there_promotion(board, move_input))
			promotion(&board[move_input.to_row][move_input.to_column], player_move);

		update_history(board, &history, move_input);
		
		player_move == WHITE? (player_move = BLACK) : (player_move = WHITE);
	}while(!is_there_special_finals(board, &history));
}

void help(){
	clear_screen();
	
	puts("How to move the pieces:\n"
	     "\tType the coordinate of the origin square and the coordinate of the destination square\n"
	     "\twith a character between them. Examples: e2 e4, c7-c5, g1$f3.\n\n"
	     "How to resign:\n"
	     "\tType: :r\n\n"
	     "How to ask for a draw:\n"
	     "\tType: :d");
	     
	pause();
}

int main(){
	char menu_choose;

	do{
		clear_screen();

		printf("\tChess UTF-8\n1. Play\n2. Help\n0. Exit\n\n> ");
		menu_choose = getchar();
		clear_input_buffer();

		if(menu_choose == '1')
			play();
		else if(menu_choose == '2')
			help();
	}while(menu_choose != '0');

	return 0;
}
