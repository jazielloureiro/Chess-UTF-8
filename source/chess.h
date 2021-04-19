#ifndef CHESS_H
#define CHESS_H

#define BOARD_SIZE 8

typedef enum{
	EMPTY_P,
	BISHOP = 'B',
	KING = 'K',
	KNIGHT = 'N',
	PAWN = 'P',
	QUEEN = 'Q',
	ROOK = 'R'
} Piece;

typedef enum{
	EMPTY_C,
	WHITE = 'W',
	BLACK = 'B'
} Color;

enum flags{
	INVALID_SQUARE = -1,
	CHOOSE_WRONG_COLOR,
	CAPTURE_OWN_PIECE,
	INCOMPATIBLE_MOVE,
	JUMP_OTHER_PIECES,
	KING_IN_CHECK,
	FIFTY_MOVES,
	THREEFOLD_REP,
	INSUFFICIENT_MAT,
	STALEMATE
};

typedef struct{
	char *image;
	Piece name;
	Color color;
} square;

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} move_coordinates;

typedef struct{
	move_coordinates move;
	Color turn;
	bool is_in_check;
} Player;
	
typedef struct{
	square from;
	square to;
} movement_squares;

typedef struct{
	Piece name;
	Color color;
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
