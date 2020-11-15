#ifndef MOVEMENT_H
#define MOVEMENT_H

bool is_movement_valid(square board[][BOARD_SIZE], History *history, move_coordinates *move_input, char player_move);
bool is_piece_movement_compatible(square board[][BOARD_SIZE], History *history, move_coordinates move_input, char move);
bool is_bishop_movement_valid(move_coordinates move_input);
bool is_king_movement_valid(move_coordinates move_input);
bool are_there_pieces_between(square board[][BOARD_SIZE], int start, int end, int row);
bool is_king_safe(square board[][BOARD_SIZE], History *history, move_coordinates move, char player_move);
bool is_castle_valid(square board[][BOARD_SIZE], History *history, move_coordinates move, char player_move);
bool is_knight_movement_valid(move_coordinates move_input);
bool is_pawn_movement_valid(move_coordinates move_input, char move);
bool is_pawn_capture_valid(square board[][BOARD_SIZE], History *history, move_coordinates move_input);
bool is_en_passant_valid(square board[][BOARD_SIZE], History *history, move_coordinates move);
bool is_queen_movement_valid(move_coordinates move_input);
bool is_rook_movement_valid(move_coordinates move_input);
bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coordinates move_input);
void move_piece(square board[][BOARD_SIZE], move_coordinates move_input);
void promotion(square *piece, char move);

#endif
