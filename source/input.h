#ifndef INPUT_H
#define INPUT_H

void read_movement_input(move_coordinates *move_input);
bool is_the_squares_valid(move_coordinates *move_input);
bool is_player_action_valid(char action, char turn);
bool is_there_player_action(Player player);
void convert_row(char *row);
void convert_column(char *column);
void convert_movement_input(move_coordinates *move);
bool is_there_promotion(square board[][BOARD_SIZE], move_coordinates move);

#endif
