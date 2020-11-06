#ifndef MOVEMENT_H
#define MOVEMENT_H

bool is_basic_movement_valid(square board[][BOARD_SIZE], movement_input *move_input, char player_move);
bool is_piece_movement_compatible(square board[][BOARD_SIZE], movement_input move_input, char move);
bool is_bishop_movement_valid(movement_input move_input);
bool is_king_movement_valid(movement_input move_input);
bool is_knight_movement_valid(movement_input move_input);
bool is_pawn_movement_valid(movement_input move_input, char move);
bool is_queen_movement_valid(movement_input move_input);
bool is_rook_movement_valid(movement_input move_input);
bool is_jump_other_pieces(square board[][BOARD_SIZE], movement_input move_input);
void move_piece(square board[][BOARD_SIZE], movement_input move_input);
void promotion(square *piece, char move);

#endif
