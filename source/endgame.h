#ifndef ENDGAME_H
#define ENDGAME_H

bool is_king_will_be_in_check(square board[][BOARD_SIZE], char move, move_coordinates move_input);
bool is_player_king_in_check(square board[][BOARD_SIZE], char move, move_coordinates *check);
bool can_king_move(square board[][BOARD_SIZE], move_coordinates check, char move);
bool can_attacking_piece_be_captured(square board[][BOARD_SIZE], move_coordinates check, char move);
bool can_piece_cover_check(square board[][BOARD_SIZE], move_coordinates check, char move);
bool has_checkmate(square board[][BOARD_SIZE], move_coordinates check, char move);
bool is_there_possible_move(square board[][BOARD_SIZE], move_coordinates movement, char player_move);
bool has_stalemate(square board[][BOARD_SIZE], char player_move);
bool is_there_threefold_repetition(History *history);
bool is_there_insufficient_material(square board[][BOARD_SIZE]);
bool is_there_special_finals(square board[][BOARD_SIZE], History *history);

#endif
