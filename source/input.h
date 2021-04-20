#ifndef INPUT_H
#define INPUT_H

void read_movement_input(move_coord *move_input);

bool is_there_player_action(Player player);

bool is_player_action_valid(char action, char turn);

void convert_movement_input(move_coord *move);

char convert_rank(char rank);

char convert_file(char file);

bool is_the_squares_valid(move_coord move_input);

bool is_there_promotion(square board[][BOARD_SIZE], move_coord move);

void promotion(square *piece, char turn);

#endif
