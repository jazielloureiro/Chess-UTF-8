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

#define INVALID_SQUARE     -1
#define CHOOSE_WRONG_COLOR -2
#define CAPTURE_OWN_PIECE  -3
#define INCOMPATIBLE_MOVE  -4
#define JUMP_OTHER_PIECES  -5
#define KING_IN_CHECK      -6
#define FIFTY_MOVES        -7
#define THREEFOLD_REP      -8
#define INSUFFICIENT_MAT   -9

typedef struct{
	char image[IMAGE_SIZE];
	char name;
	char color;
} square;
	
typedef struct{
	square from;
	square to;
} movement_squares;

typedef struct{
	char name;
	char color;
	char row;
	char column;
} square_hist;
	
typedef struct{
	square_hist sqr_hist[MAX_MOVES][MAX_PIECES];
	int pieces_counter;
	int moves_counter;
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
void print_message(int message);
void get_current_board(square board[][BOARD_SIZE], History *history);
int count_pieces(square board[][BOARD_SIZE]);
void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coordinates move_input);
void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coordinates move_input);

#endif
