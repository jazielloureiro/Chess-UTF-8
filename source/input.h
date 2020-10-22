#ifndef INPUT_H
#define INPUT_H

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} inputs;

void read_square(char *row, char *column);
void validate_square(char *row, char *column);
bool do_player_actions(char action, char move);
bool verify_player_actions(char row, char column, char move);
void convert_inputs_user(char *row, char *column);

#endif
