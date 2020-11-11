#ifndef INPUT_H
#define INPUT_H

#define VALID_MOVEMENT      0
#define INVALID_SQUARE     -1
#define CHOOSE_WRONG_COLOR -2
#define CAPTURE_OWN_PIECE  -3
#define INCOMPATIBLE_MOVE  -4
#define JUMP_OTHER_PIECES  -5
#define KING_IN_CHECK      -6

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} movement_input;

void read_movement_input(movement_input *move_input);
bool is_the_squares_valid(movement_input *move_input);
bool is_player_action_valid(char action, char move);
bool has_player_action(char row, char column, char move);
void convert_square_readed(char *row, char *column);

#endif
