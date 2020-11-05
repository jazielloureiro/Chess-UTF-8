#ifndef INPUT_H
#define INPUT_H

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} movement_input;

void read_movement_input(movement_input *move_input);
void read_square(char *row, char *column);
bool is_the_squares_valid(movement_input *move_input);
bool is_square_valid(char row, char column);
bool is_player_action_valid(char action, char move);
bool has_player_action(char row, char column);
void convert_square_readed(char *row, char *column);

#endif
