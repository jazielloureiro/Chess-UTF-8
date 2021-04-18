#ifndef ENDGAME_H
#define ENDGAME_H

bool is_game_done(square board[][BOARD_SIZE], History *history, Player *player);

bool is_checkmate(square board[][BOARD_SIZE], History history, char turn);

bool can_king_move(square board[][BOARD_SIZE], History history, char turn);

bool can_attacking_piece_be_captured(square board[][BOARD_SIZE], History history, char turn);

bool can_piece_cover_check(square board[][BOARD_SIZE], History history, char turn);

bool is_stalemate(square board[][BOARD_SIZE], History history, char turn);

bool is_there_possible_move(square board[][BOARD_SIZE], History history, Player temp);

bool is_threefold_repetition(square board[][BOARD_SIZE], History *history);

bool is_insufficient_material(square board[][BOARD_SIZE]);

#endif
