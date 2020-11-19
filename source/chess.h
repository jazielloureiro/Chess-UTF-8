#ifndef CHESS_H
#define CHESS_H

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
#define STALEMATE          -10

typedef struct{
	char image[IMAGE_SIZE];
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
} square_hist;
	
typedef struct{
	bool has_left_black_rook_moved;
	bool has_right_black_rook_moved;
	bool has_black_king_moved;
	bool has_left_white_rook_moved;
	bool has_right_white_rook_moved;
	bool has_white_king_moved;
	bool has_occurred;
} castle_pieces_history;

typedef struct{
	square_hist board[MAX_MOVES][MAX_PIECES];
	int pieces_counter;
	int moves_counter;
	move_coordinates check;
	castle_pieces_history castle;
	move_coordinates last_input;
	bool has_en_passant_occurred;
} History;

void init_board(square board[][BOARD_SIZE]);
void init_history(square board[][BOARD_SIZE], History *history);
void update_castle_history(square board[][BOARD_SIZE], History *history, move_coordinates move);
bool has_pawn_moved(History history, move_coordinates move);
void update_history(square board[][BOARD_SIZE], History *history, move_coordinates move);
void print_top_menu(char turn, bool check);
void print_board(square board[][BOARD_SIZE]);
void print_error_message(int message);
void print_final_board(square board[][BOARD_SIZE], char final);
void get_current_board(square board[][BOARD_SIZE], History *history);
int count_pieces(square board[][BOARD_SIZE]);
void save_move_squares(square board[][BOARD_SIZE], movement_squares *move_squares, move_coordinates move_input);
void return_move_squares(square board[][BOARD_SIZE], movement_squares move_squares, move_coordinates move_input);
void en_passant(square board[][BOARD_SIZE], History history);
void find_castle_rook(move_coordinates move, move_coordinates *rook);
void castle(square board[][BOARD_SIZE], move_coordinates move);
void advance_to(char *from, char to);

#endif
