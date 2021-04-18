#ifndef CHESS_H
#define CHESS_H

#define BOARD_SIZE 8

#define BISHOP 'B'
#define KING   'K'
#define KNIGHT 'N'
#define PAWN   'P'
#define QUEEN  'Q'
#define ROOK   'R'

#define WHITE 'W'
#define BLACK 'B'

#define NO_PIECE 0

#define INVALID_SQUARE     -1
#define CHOOSE_WRONG_COLOR -2
#define CAPTURE_OWN_PIECE  -3
#define INCOMPATIBLE_MOVE  -4
#define JUMP_OTHER_PIECES  -5
#define KING_IN_CHECK      -6
#define FIFTY_MOVES        -7
#define THREEFOLD_REP      -8
#define INSUFFICIENT_MAT   -9
#define STALEMATE          -10

typedef struct{
	char *image;
	char name;
	char color;
} square;

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} move_coordinates;

typedef struct{
	move_coordinates move;
	char turn;
	bool is_in_check;
} Player;
	
typedef struct{
	square from;
	square to;
} movement_squares;

typedef struct{
	char name;
	char color;
	char row;
	char column;
} h_square;
	
typedef struct H_board{
	int pieces_qty;
	h_square *pieces;
	Player player;
	struct H_board *prev;
} h_board;

typedef struct{
	h_board *board;
	int moves_counter;
	bool has_castle_occurred, has_en_passant_occurred;
	move_coordinates last_check;
} History;

void init_board(square board[][BOARD_SIZE]);

void init_history(History *history);

void init_player(Player *player);

void update_history(square board[][BOARD_SIZE], History *history, Player player);

void get_current_board(square board[][BOARD_SIZE], History *history);

int count_pieces(square board[][BOARD_SIZE]);

void move_piece(square board[][BOARD_SIZE], move_coordinates move);

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coordinates move_input);

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coordinates move_input);

void find_castle_rook(move_coordinates move, move_coordinates *rook);

void castle(square board[][BOARD_SIZE], move_coordinates move);

void advance_to(char *from, char to);

void free_history(h_board *aux);

#endif
