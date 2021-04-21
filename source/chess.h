#ifndef CHESS_H
#define CHESS_H

#define BOARD_SIZE 8

enum pieces{
	EMPTY = ' ',
	BISHOP = 'B',
	KING = 'K',
	KNIGHT = 'N',
	PAWN = 'P',
	QUEEN = 'Q',
	ROOK = 'R'
};

enum colors{
	WHITE = 'W',
	BLACK = 'B'
};

enum flags{
	ASK_RESIGN = -3,
	ASK_DRAW,
	INVALID_SQUARE,
	FIFTY_MOVES,
	THREEFOLD_REP,
	INSUFFICIENT_MAT,
	STALEMATE
};

typedef struct{
	char *image;
	char piece;
	char color;
} square;

typedef struct{
	int8_t from_rank : 4;
	int8_t from_file : 4;
	int8_t to_rank : 4;
	int8_t to_file : 4;
} move_coord;

typedef struct{
	move_coord move;
	char turn;
	bool is_in_check;
} Player;
	
typedef struct{
	square from;
	square to;
} movement_squares;

typedef struct{
	char piece;
	char color;
	int8_t rank : 4;
	int8_t file : 4;
} h_square;
	
typedef struct H_board{
	int8_t pieces_qty;
	h_square *pieces;
	Player player;
	struct H_board *prev;
} h_board;

typedef struct{
	h_board *board;
	int moves_counter;
	bool has_castle_occurred, has_en_passant_occurred;
	move_coord last_check;
} History;

void init_board(square board[][BOARD_SIZE]);

void init_history(History *history);

void init_player(Player *player);

void update_history(square board[][BOARD_SIZE], History *history, Player player);

void get_current_board(square board[][BOARD_SIZE], History *history);

int8_t count_pieces(square board[][BOARD_SIZE]);

void move_piece(square board[][BOARD_SIZE], move_coord move);

void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coord move_input);

void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coord move_input);

void find_castle_rook(move_coord move, move_coord *rook);

void castle(square board[][BOARD_SIZE], move_coord move);

void advance_to(char *from, char to);

void free_history(h_board *aux);

#endif
