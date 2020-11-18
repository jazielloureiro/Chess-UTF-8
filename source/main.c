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
	Player player;

	init_board(board);
	init_history(board, &history);
	player.turn = WHITE;

	do{
		bool is_check;

		is_check = is_player_king_in_check(board, &history, player.turn);

		if(is_check){
			if(is_there_checkmate(board, history, player.turn)){
				print_final_board(board, player.turn);
				return;
			}
		}else{
			if(is_there_stalemate(board, history, player.turn)){
				print_final_board(board, STALEMATE);
				return;
			}
		}

		do{
			clear_screen();
			print_top_menu(player.turn, is_check);
			print_board(board);

			read_movement_input(&player.move);

			if(is_there_player_action(player.move.from_row,
			                          player.move.from_column,
					          player.turn))
				return;
		}while(!is_movement_valid(board, &history, &player.move, player.turn));

		move_piece(board, player.move);

		if(history.has_en_passant_occurred)
			en_passant(board, history);

		if(history.castle.has_occurred)
			castle(board, player.move);

		if(is_there_promotion(board, player.move))
			promotion(&board[player.move.to_row][player.move.to_column], player.turn);

		update_history(board, &history, player.move);
		
		player.turn == WHITE? (player.turn = BLACK) : (player.turn = WHITE);
	}while(!is_there_special_final(board, &history));
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
	     
	pause_screen();
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
