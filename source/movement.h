#ifndef MOVEMENT_H
#define MOVEMENT_H

bool is_movement_valid(square board[][BOARD_SIZE], History *history, Player *player);
bool is_piece_movement_compatible(square board[][BOARD_SIZE], History *history, Player player);
bool is_bishop_movement_valid(move_coordinates move);
bool is_king_movement_valid(move_coordinates move);
bool are_there_pieces_between(square row[], char start, char end);
bool is_king_safe(square board[][BOARD_SIZE], History *history, Player player);
bool is_castle_valid(square board[][BOARD_SIZE], History *history, Player player);
bool is_knight_movement_valid(move_coordinates move);
bool is_pawn_movement_valid(Player player);
bool is_pawn_capture_valid(square board[][BOARD_SIZE], History *history, move_coordinates move);
bool is_en_passant_valid(square board[][BOARD_SIZE], History *history, move_coordinates move);
bool is_queen_movement_valid(move_coordinates move);
bool is_rook_movement_valid(move_coordinates move);
bool is_jump_other_pieces(square board[][BOARD_SIZE], move_coordinates move);
void move_piece(square board[][BOARD_SIZE], move_coordinates move);
void promotion(square *piece, char turn);

#endif
