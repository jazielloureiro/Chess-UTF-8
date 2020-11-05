#ifndef CHESS_H
#define CHESS_H

#include "input.h"

#define BOARD_SIZE 8
#define IMAGE_SIZE 7

#define BISHOP 'B'
#define KING   'K'
#define KNIGHT 'N'
#define PAWN   'P'
#define QUEEN  'Q'
#define ROOK   'R'

#define WHITE 'W'
#define BLACK 'B'

#define NO_PIECE 0

#define MAX_MOVES 100
#define MAX_PIECES 32

typedef struct{
	char image[IMAGE_SIZE];
	char name;
	char color;
} square;
	
typedef struct{
	square from;
	square to;
} last_state;

typedef struct{
	char name;
	char color;
	char row;
	char column;
} square_hist;
	
typedef struct{
	square_hist sqr_hist[MAX_MOVES][MAX_PIECES];
	int pieces_count;
	int moves_count;
} History;

typedef struct{
	bool is_left_black_rook_moved;
	bool is_right_black_rook_moved;
	bool is_black_king_moved;
	bool is_left_white_rook_moved;
	bool is_right_white_rook_moved;
	bool is_white_king_moved;
} castle_pieces_history;

void init_board(square board[][BOARD_SIZE]);
void init_history(square board[][BOARD_SIZE], History *history);
void init_castle_history(castle_pieces_history *castle_hist);
void print_top_menu(char move, bool check);
void print_board(square board[][BOARD_SIZE]);
void get_current_board(square board[][BOARD_SIZE], History *history);
int count_pieces(square board[][BOARD_SIZE]);
void save_state_board(square board[][BOARD_SIZE], last_state *movement, movement_input user);
void return_last_state(square board[][BOARD_SIZE], last_state movement, movement_input user);

#endif
