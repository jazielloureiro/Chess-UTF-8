#ifndef CHESS_H
#define CHESS_H

#define BOARD_SIZE 8
#define IMAGE_SIZE 7

#include "input.h"
#include <stdbool.h>

#define BISHOP 'B'
#define KING   'K'
#define KNIGHT 'N'
#define PAWN   'P'
#define QUEEN  'Q'
#define ROOK   'R'

#define WHITE 'W'
#define BLACK 'B'

#define NO_PIECE 0

typedef struct{
	char image[IMAGE_SIZE];
	char name;
	char color;
} square;
	
typedef struct{
	square from;
	square to;
} last_state;

void init_board(square board[][BOARD_SIZE]);
void print_top_menu(char move, char *piece, bool check);
void print_board(square board[][BOARD_SIZE]);
void save_state_board(square board[][BOARD_SIZE], last_state *movement, inputs user);
void return_last_state(square board[][BOARD_SIZE], last_state movement, inputs user);

#endif
