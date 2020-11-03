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
	char player_move = WHITE;

	init_board(board);
	init_history(board, &history);

	do{
		inputs user;
		last_state movement;
		bool is_need_reset_history = false;

		do{
			do{
				clear_screen();
				print_top_menu(player_move, "■", verify_check(board, player_move));
				print_board(board);

				printf("\nEnter the square of the piece you want to move: ");
				read_square(&user.from_row, &user.from_column);
				
				if(verify_player_actions(user.from_row, user.from_column, player_move))
					return;
				
				validate_square(&user.from_row, &user.from_column);

				convert_inputs_user(&user.from_row, &user.from_column);
			}while(board[user.from_row][user.from_column].color != player_move);

			selected_piece = &board[user.from_row][user.from_column];

			do{
				clear_screen();
				print_top_menu(player_move, selected_piece->image, verify_check(board, player_move));
				print_board(board);

				printf("\nEnter where you want to move the piece: ");
				read_square(&user.to_row, &user.to_column);
				
				if(verify_player_actions(user.to_row, user.to_column, player_move))
					return;
				
				validate_square(&user.to_row, &user.to_column);

				convert_inputs_user(&user.to_row, &user.to_column);
			}while(board[user.to_row][user.to_column].color == player_move);
			
		}while(!validate_movement(selected_piece->name, user, player_move) || verify_collision(selected_piece->name, board, user));

		save_state_board(board, &movement, user);

		move_piece(board, user);

		if(board[user.to_row][user.to_column].name == PAWN){
			is_need_reset_history = true;

			if(user.to_row == 0 || user.to_row == 7)
				promotion(&board[user.to_row][user.to_column], player_move);
		}
			
		if(verify_check(board, player_move)){
			return_last_state(board, movement, user);
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
