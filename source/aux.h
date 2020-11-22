#ifndef AUX_H
#define AUX_H

#include "chess.h"

void clear_input_buffer();
void clear_screen();
void pause_screen();
void print_board(square board[][BOARD_SIZE]);
void print_error_message(int message);
void print_final_board(square board[][BOARD_SIZE], char final);
void print_top_menu(char turn, bool check);

#endif
