#include <stdbool.h>
#include <stdio.h>

#include "aux.h"
#include "chess.h"
#include "endgame.h"
#include "input.h"
#include "movement.h"

void play(){
	square board[BOARD_SIZE][BOARD_SIZE], *selected_piece;
	History history;
	castle_pieces_history castle_hist;
	char player_move = WHITE;

	init_board(board);
	init_history(board, &history);
	init_castle_history(&castle_hist);

	do{
		movement_input move_input;
		last_state movement;
		bool is_movement_valid, is_need_reset_history = false;

		do{
			is_movement_valid = false;

			clear_screen();
			print_top_menu(player_move, verify_check(board, player_move));
			print_board(board);

			read_movement_input(&move_input);

			if(has_player_action(move_input.from_row,
			                     move_input.from_column)){
				if(is_player_action_valid(move_input.from_row,
							  player_move)){
					return;
				}
			}

			if(!is_the_squares_valid(&move_input)){
				puts("\nYou've entered an invalid square!");
				pause();
				continue;
			}

			convert_square_readed(&move_input.from_row, &move_input.from_column);
			convert_square_readed(&move_input.to_row, &move_input.to_column);

			if(board[move_input.from_row][move_input.from_column].color != player_move){
				puts("\nPlease, choose a piece of your color.");
				pause();
			}else if(board[move_input.to_row][move_input.to_column].color == player_move){
				puts("\nYou can't capture your own piece!");
				pause();
			}else if(!validate_movement(board, move_input, player_move)){
				puts("\nThis movement is incompatible with your piece.");
				pause();
			}else if(verify_collision(board, move_input)){
				puts("\nYour piece can't jump over other pieces!");
				pause();
			}else
				is_movement_valid = true;
		}while(!is_movement_valid);

		save_state_board(board, &movement, move_input);

		move_piece(board, move_input);

		if(board[move_input.to_row][move_input.to_column].name == PAWN){
			is_need_reset_history = true;

			if(move_input.to_row == 0 || move_input.to_row == 7)
				promotion(&board[move_input.to_row][move_input.to_column], player_move);
		}
			
		if(verify_check(board, player_move)){
			return_last_state(board, movement, move_input);
			continue;
		}

		if(count_pieces(board) < history.pieces_count){
			history.pieces_count--;
			is_need_reset_history = true;
		}

		if(is_need_reset_history)
			history.moves_count = 0;

		get_current_board(board, &history);
		
		player_move == WHITE? (player_move = BLACK) : (player_move = WHITE);
	}while(!special_finals(board, &history));
}

void help(){
	clear_screen();
	
	puts("Para fazer um movimento ou selecionar um peça digite a letra e o número da casa, sem o uso de espaço (Exemplo: a1)\n"
	     "Aqui vai algumas regras e dicas básicas:\n\n"
	     "1. Peão\n"
	     "Se o peão chegar ao outro lado do board poderá ser promovido a uma peça que será de sua choose.\n"
	     "O peão só pode seguir em linha reta e para frente, podendo andar duas casas somente em sua primeira jogada,\n"
	     "ele captura apenas na diagonal.\n\n"
	     "2. Cavalo\n"
	     "Anda sempre em formato de 'L', ele sempre captura no final do L e qualquer peça no caminho é pulada.\n\n"
	     "3. Torre\n"
	     "Sempre anda em linha reta sendo para frente, trás, esquerda ou direita,\n"
	     "podendo parar em qualquer lugar para capturar um peça, porém ele para onde capturar.\n\n"
	     "4. Bispo\n"
	     "Anda sempre na diagonal, e assim como a torre pode parar em seu percurso para capturar.\n\n"
	     "5. Dama\n"
	     "Anda igual torre e bispo, podendo andar pelas diagonais ou em linha reta,\n"
	     "e assim como eles, pode parar para capturar durante o percurso.\n\n"
	     "6. Rei\n"
	     "E por último mas não menos importante, e de fato uma peça muito importante pois com a morte do rei é game over\n"
	     "indepedente de quantas peças foram capturadas por você.\n"
	     "O rei assim como a dama pode andar para qualquer direção, porém só pode andar um casa por vez.\n\n"
	     "Aqui estão todos os conhecimentos que você vai precisar, caso tente qualquer movimento desses porém nada ocolorra,\n"
	     "quer dizer que você está em xeque e o rei está em perigo.\n"
	     "Para pedir um empate durante a partida digite :e\n"
	     "Para desistir da partida digite :d");
	     
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
