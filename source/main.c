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
	castle_pieces_history castle_hist;
	char player_move = WHITE;

	init_board(board);
	init_history(board, &history);
	init_castle_history(&castle_hist);

	do{
		movement_input move_input, check;
		movement_squares move_squares;
		bool is_king_in_check, is_need_reset_history = false;

		is_king_in_check = is_player_king_in_check(board, player_move, &check);

		if(is_king_in_check){
			if(has_checkmate(board, check, player_move)){
				clear_screen();
				print_board(board);
				pause();
				return;
			}
		}


		do{
			clear_screen();
			print_top_menu(player_move, is_king_in_check);
			print_board(board);

			read_movement_input(&move_input);

			if(has_player_action(move_input.from_row,
			                     move_input.from_column,
					     player_move))
				return;
		}while(!is_basic_movement_valid(board, &move_input, player_move));

		save_move_squares(board, &move_squares, move_input);

		move_piece(board, move_input);

		if(board[move_input.to_row][move_input.to_column].name == PAWN){
			is_need_reset_history = true;

			if(move_input.to_row == 0 || move_input.to_row == 7)
				promotion(&board[move_input.to_row][move_input.to_column], player_move);
		}
			
		if(is_player_king_in_check(board, player_move, &check)){
			puts("\nYour can't do this move, because your king will be in check.");
			pause();

			return_move_squares(board, move_squares, move_input);
			continue;
		}

		if(count_pieces(board) < history.pieces_counter){
			history.pieces_counter--;
			is_need_reset_history = true;
		}

		if(is_need_reset_history)
			history.moves_counter = 0;

		get_current_board(board, &history);
		
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
