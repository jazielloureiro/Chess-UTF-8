#ifndef ENDGAME_H
#define ENDGAME_H

bool is_player_king_in_check(square board[][BOARD_SIZE], char move);
bool is_there_threefold_repetition(History *history);
bool is_there_insufficient_material(square board[][BOARD_SIZE]);
bool is_there_special_finals(square board[][BOARD_SIZE], History *history);

#endif
