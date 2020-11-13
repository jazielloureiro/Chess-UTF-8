#ifndef INPUT_H
#define INPUT_H

void read_movement_input(move_coordinates *move_input);
bool is_the_squares_valid(move_coordinates *move_input);
bool is_player_action_valid(char action, char move);
bool has_player_action(char row, char column, char move);
void convert_square_readed(char *row, char *column);
bool is_there_promotion(square board[][BOARD_SIZE], move_coordinates move);

#endif
