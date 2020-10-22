#ifndef MOVEMENT_H
#define MOVEMENT_H

bool validate_movement(char selected_piece, inputs user, char move);
bool validate_movement_bishop(inputs user);
bool validate_movement_king(inputs user);
bool validate_movement_knight(inputs user);
bool validate_movement_pawn(inputs user, char move);
bool validate_movement_queen(inputs user);
bool validate_movement_rook(inputs user);
bool verify_collision(char selected_piece, square board[][BOARD_SIZE], inputs user);
void move_piece(square board[][BOARD_SIZE], inputs user);
void promotion(square *piece, char move);

#endif
