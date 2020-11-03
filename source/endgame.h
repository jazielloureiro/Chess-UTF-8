#ifndef ENDGAME_H
#define ENDGAME_H

bool verify_check(square board[][BOARD_SIZE], char move);
bool is_there_threefold_repetition(History *history);
bool special_finals(square board[][BOARD_SIZE], History *history);

#endif
