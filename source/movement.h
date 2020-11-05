#ifndef MOVEMENT_H
#define MOVEMENT_H

bool validate_movement(square board[][BOARD_SIZE], movement_input user, char move);
bool validate_movement_bishop(movement_input user);
bool validate_movement_king(movement_input user);
bool validate_movement_knight(movement_input user);
bool validate_movement_pawn(movement_input user, char move);
bool validate_movement_queen(movement_input user);
bool validate_movement_rook(movement_input user);
bool verify_collision(square board[][BOARD_SIZE], movement_input user);
void move_piece(square board[][BOARD_SIZE], movement_input user);
void promotion(square *piece, char move);

#endif
