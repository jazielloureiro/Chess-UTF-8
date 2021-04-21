#ifndef INPUT_H
#define INPUT_H

move_coord read_input();

int8_t convert_rank(char rank);

int8_t convert_file(char file);

bool is_action(Player player);

bool is_action_confirmed(Player player);

bool is_the_squares_valid(move_coord move_input);

bool is_there_promotion(square board[][BOARD_SIZE], move_coord move);

void promotion(square *piece, char turn);

#endif
