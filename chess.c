#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8
#define IMAGE_SIZE 7

#define BISHOP 'B'
#define KING   'K'
#define KNIGHT 'N'
#define PAWN   'P'
#define QUEEN  'Q'
#define ROOK   'R'

#define WHITE 'W'
#define BLACK 'B'

#define NO_PIECE 0

typedef struct{
	char image[IMAGE_SIZE];
	char name;
	char color;
} square;

typedef struct{
	char from_row;
	char from_column;
	char to_row;
	char to_column;
} inputs;
	
typedef struct{
	square from;
	square to;
} last_state;

/*----------Aux functions----------*/
void clear_screen(){
	system("clear");
}

void clear_input_buffer(){
	char buffer;

	while ((buffer = getchar()) != '\n' && buffer != EOF);
}

void pause(){
	puts("\nPress Enter to continue");
	
	while(getchar() != '\n');
}

void init_board(square board[][BOARD_SIZE]){
	int i, j;

	for(i = 0; i < 2; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = BLACK;

	sprintf(board[0][0].image, "♜");
	board[0][0].name = ROOK;

	sprintf(board[0][1].image, "♞");
	board[0][1].name = KNIGHT;

	sprintf(board[0][2].image, "♝");
	board[0][2].name = BISHOP;

	sprintf(board[0][3].image, "♛");
	board[0][3].name = QUEEN;

	sprintf(board[0][4].image, "♚");
	board[0][4].name = KING;

	sprintf(board[0][5].image, "♝");
	board[0][5].name = BISHOP;

	sprintf(board[0][6].image, "♞");
	board[0][6].name = KNIGHT;

	sprintf(board[0][7].image, "♜");
	board[0][7].name = ROOK;

	for(j = 0; j < BOARD_SIZE; j++){
		sprintf(board[1][j].image, "♟");
		board[1][j].name = PAWN;
	}

	for(i = 2; i < 6; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			sprintf(board[i][j].image, " ");
			board[i][j].name = NO_PIECE;
			board[i][j].color = NO_PIECE;
		}
	}

	for(i = 6; i < BOARD_SIZE; i++)
		for(j = 0; j < BOARD_SIZE; j++)
			board[i][j].color = WHITE;

	for(j = 0; j < BOARD_SIZE; j++){
		sprintf(board[6][j].image, "♙");
		board[6][j].name = PAWN;
	}

	sprintf(board[7][0].image, "♖");
	board[7][0].name = ROOK;

	sprintf(board[7][1].image, "♘");
	board[7][1].name = KNIGHT;

	sprintf(board[7][2].image, "♗");
	board[7][2].name = BISHOP;

	sprintf(board[7][3].image, "♕");
	board[7][3].name = QUEEN;

	sprintf(board[7][4].image, "♔");
	board[7][4].name = KING;

	sprintf(board[7][5].image, "♗");
	board[7][5].name = BISHOP;

	sprintf(board[7][6].image, "♘");
	board[7][6].name = KNIGHT;

	sprintf(board[7][7].image, "♖");
	board[7][7].name = ROOK;
}

void print_top_menu(char move, char *piece, bool check){
	if(check){
		putchar('\n');
		printf("\t┌───────────────┐\t┌───────────────────────┐\t┌───────┐\n");
		printf("\t│ %s moves\t│\t│ Selected piece: %s\t│\t│ Check │\n", (move == WHITE? "White" : "Black"), piece);
		printf("\t└───────────────┘\t└───────────────────────┘\t└───────┘\n\n");
	}else{
		putchar('\n');
		printf("\t┌───────────────┐\t┌───────────────────────┐\n");
		printf("\t│ %s moves\t│\t│ Selected piece: %s\t│\n", (move == WHITE? "White" : "Black"), piece);
		printf("\t└───────────────┘\t└───────────────────────┘\n\n");
	}
}

void print_board(square board[][BOARD_SIZE]){
	int i, j, k = 8;
	
	printf("\t  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

	for(i = 0; i < BOARD_SIZE; i++){
		printf("\t%d │", k--);

		for(j = 0; j < BOARD_SIZE; j++)
			printf(" %s │", board[i][j].image);

		putchar('\n');

		if(i < BOARD_SIZE - 1)
			printf("\t  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}

	printf("\t  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "\t    a   b   c   d   e   f   g   h\n");
}
/*--------------------------------------*/

/*----------Movement functions----------*/
void read_square(char *row, char *column){
	*column = getchar();

	*row = getchar();

	clear_input_buffer();
}

bool do_player_actions(char action, char move){
	char choose;
	
	if(action == 'r')
		printf("Do you want to resign the game? [y/n] ");
	else
		printf("The %s player offers a draw, does the %s player agree? [y/n] ",
		       move == WHITE? "White" : "Black",
		       move == WHITE? "Black" : "White");

	do{
		choose = getchar();
		clear_input_buffer();
	}while(choose != 'y' && choose != 'n');
	
	if(choose == 'y'){
		if(action == 'r')
			printf("End of the game! The %s player resigned the game!\n",
			       move == WHITE? "White" : "Black");
		else
			printf("End of the game! Both players agreed to a draw!\n");
			
		pause();
			       
		return true;
	}
	
	return false;
}

bool verify_player_actions(char row, char column, char move){
	if(column == ':' && (row == 'r' || row == 'd'))
		return do_player_actions(row, move);
		
	return false;
}

void validate_square(char *row, char *column){
	while(*column < 'a' || *column > 'h' || *row < '1' || *row > '8'){
		printf("\nYou've entered an invalid square! Enter a square again: ");
		read_square(row, column);
	}
}

void convert_inputs_user(char *row, char *column){
	switch(*row){
		case '8': *row = 0; break;
		case '7': *row = 1; break;
		case '6': *row = 2; break;
		case '5': *row = 3; break;
		case '4': *row = 4; break;
		case '3': *row = 5; break;
		case '2': *row = 6; break;
		case '1': *row = 7;
	}

	*column -= 'a';
}

bool validate_movement_pawn(inputs user, char move){
	int advance2Squares = (move == WHITE? -2 : 2),
	    advance1Square  = (move == WHITE? -1 : 1);

	if(user.from_row == 6 || user.from_row == 1)
		if(user.from_row + advance2Squares == user.to_row &&
		   user.from_column == user.to_column)
		   	return true;
			
	if(user.from_row + advance1Square == user.to_row){
		int j;

		for(j = user.from_column - 1;
		    j <= user.from_column + 1;
		    j++)
			if(j == user.to_column)
		    		return true;
	}
	
	return false;
}

bool validate_movement_rook(inputs user){
	if(user.from_row == user.to_row)
		return true;
	if(user.from_column == user.to_column)
		return true;

	return false;
}

bool validate_movement_knight(inputs user){
	int diff_row, diff_column;
	
	diff_row    = user.to_row    - user.from_row;
	diff_column = user.to_column - user.from_column;

	if(diff_row == 2  && diff_column == 1  ||
	   diff_row == 2  && diff_column == -1 ||
	   diff_row == -2 && diff_column == 1  ||
	   diff_row == -2 && diff_column == -1 ||
	   diff_row == 1  && diff_column == 2  ||
	   diff_row == 1  && diff_column == -2 ||
	   diff_row == -1 && diff_column == 2  ||
	   diff_row == -1 && diff_column == -2)
		return true;

	return false;
}

bool validate_movement_bishop(inputs user){
	int i, j;

	// Verifying the top left diagonal
	for(i = user.from_row - 1, j = user.from_column - 1;
	    i >= 0 && j >= 0;
	    i--, j--)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the top right diagonal
	for(i = user.from_row - 1, j = user.from_column + 1;
	    i >= 0 && j <= 7;
	    i--, j++)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the bottom left diagonal
	for(i = user.from_row + 1, j = user.from_column - 1;
	    i <= 7 && j >= 0;
	    i++, j--)
		if(i == user.to_row && j == user.to_column)
			return true;

	// Verifying the bottom right diagonal
	for(i = user.from_row + 1, j = user.from_column + 1;
	    i <= 7 && i <= 7;
	    i++, j++)
		if(i == user.to_row && j == user.to_column)
			return true;

	return false;
}

bool validate_movement_king(inputs user){
	int i, j;

	for(i = user.from_row - 1; i <= user.from_row + 1; i++)
		for(j = user.from_column - 1; j <= user.from_column + 1; j++)
			if(i == user.to_row && j == user.to_column)
				return true;

	return false;
}

bool validate_movement_queen(inputs user){
	if(validate_movement_rook(user) == true)
		return true;
	if(validate_movement_bishop(user) == true)
		return true;
		
	return false;
}

bool validate_movement(char selected_piece, inputs user, char move){
	switch(selected_piece){
		case PAWN:   return validate_movement_pawn(user, move);
		case ROOK:   return validate_movement_rook(user);
		case KNIGHT: return validate_movement_knight(user);
		case BISHOP: return validate_movement_bishop(user);
		case KING:   return validate_movement_king(user);
		case QUEEN:  return validate_movement_queen(user);
	}
}

bool verify_collision(char selected_piece, square board[][BOARD_SIZE], inputs user){
	if(selected_piece == PAWN){
		if(user.from_row  != user.to_row  &&
		   user.from_column != user.to_column &&
		   board[user.to_row][user.to_column].name == NO_PIECE)
		   	return true;
	}
	
	if(selected_piece != KNIGHT){
		int i = user.from_row, j = user.from_column;
		
		do{
			if(i < user.to_row)
				i++;
			else if(i > user.to_row)
				i--;
			
			if(j < user.to_column)
				j++;
			else if(j > user.to_column)
				j--;
			
			if(board[i][j].name != NO_PIECE && (i != user.to_row || j != user.to_column))
				return true;
		}while(i != user.to_row || j != user.to_column);
	}
	
	return false;
}

void move_piece(square board[][BOARD_SIZE], inputs user){
	strcpy(board[user.to_row][user.to_column].image,
	       board[user.from_row][user.from_column].image);

	board[user.to_row][user.to_column].name = 
	board[user.from_row][user.from_column].name;

	board[user.to_row][user.to_column].color = 
	board[user.from_row][user.from_column].color;

	strcpy(board[user.from_row][user.from_column].image, " \0");

	board[user.from_row][user.from_column].name = NO_PIECE;

	board[user.from_row][user.from_column].color = NO_PIECE;
}

void promotion(square *piece, char move){
	char choose;

	puts("\nFor which piece do you want to promove? ");
	
	if(move == WHITE)
		puts("1. ♕\n2. ♖\n3. ♗\n4. ♘\n");
	else
		puts("1. ♛\n2. ♜\n3. ♝\n4. ♞\n");
	       
	do{
		printf("> ");
		choose = getchar();
		clear_input_buffer();
	}while(choose < '1' || choose > '4');
	       
	switch(choose){
		case '1':
			sprintf(piece->image, move == WHITE? "♕" : "♛");
			piece->name = QUEEN;
			break;
		case '2':
			sprintf(piece->image, move == WHITE? "♖" : "♜");
			piece->name = ROOK;
			break;
		case '3':
			sprintf(piece->image, move == WHITE? "♗" : "♝");
			piece->name = BISHOP;
			break;
		case '4':
			sprintf(piece->image, move == WHITE? "♘" : "♞");
			piece->name = KNIGHT;
			break;
	}       
}
/*-----------------------------------*/

void save_state_board(square board[][BOARD_SIZE], last_state *movement, inputs user){
	strcpy(movement->from.image, board[user.from_row][user.from_column].image);
	movement->from.name  = board[user.from_row][user.from_column].name;
	movement->from.color = board[user.from_row][user.from_column].color;
	
	strcpy(movement->to.image, board[user.to_row][user.to_column].image);
	movement->to.name  = board[user.to_row][user.to_column].name;
	movement->to.color = board[user.to_row][user.to_column].color;
}

void return_last_state(square board[][BOARD_SIZE], last_state movement, inputs user){
	strcpy(board[user.from_row][user.from_column].image, movement.from.image);
	board[user.from_row][user.from_column].name = movement.from.name;
	board[user.from_row][user.from_column].color  = movement.from.color;
	
	strcpy(board[user.to_row][user.to_column].image, movement.to.image);
	board[user.to_row][user.to_column].name = movement.to.name;
	board[user.to_row][user.to_column].color  = movement.to.color;
}

bool verify_check(square board[][BOARD_SIZE], char move){
	inputs check;
	char oponent_color = (move == WHITE? BLACK : WHITE);
	int i, j;
	
	// Searching where the king is
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].name == KING && board[i][j].color == move){
				check.to_row = i;
				check.to_column = j;
				break;
			}
		}
	}
	
	// Verify if the oponent's pieces are threatening the king
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(board[i][j].color == oponent_color){
				check.from_row = i;
				check.from_column = j;
			
				if(validate_movement(board[i][j].name, check, move) &&
				   !verify_collision(board[i][j].name, board, check))
				   	return true;
			}
		}
	}
				   	
	return false;
}

/*----------Menu functions----------*/
void play(){
	square board[BOARD_SIZE][BOARD_SIZE];

	inputs user;
	
	last_state movement;

	char player_move = WHITE;
	
	square *selected_piece;

	init_board(board);

	do{
		do{
			do{
				clear_screen();
				print_top_menu(player_move, "■", verify_check(board, player_move));
				print_board(board);

				printf("\nEnter the square of the piece you want to move: ");
				read_square(&user.from_row, &user.from_column);
				
				if(verify_player_actions(user.from_row, user.from_column, player_move) == true)
					return;
				
				validate_square(&user.from_row, &user.from_column);

				convert_inputs_user(&user.from_row, &user.from_column);
			}while(board[user.from_row][user.from_column].color != player_move);

			selected_piece = &board[user.from_row][user.from_column];

			do{
				clear_screen();
				print_top_menu(player_move, selected_piece->image, verify_check(board, player_move));
				print_board(board);

				printf("\nEnter where you want to move the piece: ");
				read_square(&user.to_row, &user.to_column);
				
				if(verify_player_actions(user.to_row, user.to_column, player_move) == true)
					return;
				
				validate_square(&user.to_row, &user.to_column);

				convert_inputs_user(&user.to_row, &user.to_column);
			}while(board[user.to_row][user.to_column].color == player_move);
			
		}while(!validate_movement(selected_piece->name, user, player_move) || verify_collision(selected_piece->name, board, user));

		save_state_board(board, &movement, user);

		move_piece(board, user);
		
		if(selected_piece->name == PAWN && (user.to_row == 0 || user.to_row == 7))
			promotion(&board[user.to_row][user.to_column], player_move);
			
		if(verify_check(board, player_move) == true){
			return_last_state(board, movement, user);
			continue;
		}

		player_move == WHITE? (player_move = BLACK) : (player_move = WHITE);
	}while(true);
}

void help(){
	clear_screen();
	
	puts("Para fazer um movimento ou selecionar um peça digite a letra e o número da casa, sem o uso de espaço (Exemplo: a1)\n"
	     "Aqui vai algumas regras e dicas básicas:\n\n"
	     "1. Peão\n"
	     "Se o peão chegar ao outro lado do board poderá ser promovido a uma peça que será de sua choose.\n"
	     "O peão só pode seguir em linha reta e para frente, podendo andar duas casas somente em sua primeira jogada,\n"
	     "ele captura apenas na diagonal.\n\n"
	     "2. Cavalo\n"
	     "Anda sempre em formato de 'L', ele sempre captura no final do L e qualquer peça no caminho é pulada.\n\n"
	     "3. Torre\n"
	     "Sempre anda em linha reta sendo para frente, trás, esquerda ou direita,\n"
	     "podendo parar em qualquer lugar para capturar um peça, porém ele para onde capturar.\n\n"
	     "4. Bispo\n"
	     "Anda sempre na diagonal, e assim como a torre pode parar em seu percurso para capturar.\n\n"
	     "5. Dama\n"
	     "Anda igual torre e bispo, podendo andar pelas diagonais ou em linha reta,\n"
	     "e assim como eles, pode parar para capturar durante o percurso.\n\n"
	     "6. Rei\n"
	     "E por último mas não menos importante, e de fato uma peça muito importante pois com a morte do rei é game over\n"
	     "indepedente de quantas peças foram capturadas por você.\n"
	     "O rei assim como a dama pode andar para qualquer direção, porém só pode andar um casa por vez.\n\n"
	     "Aqui estão todos os conhecimentos que você vai precisar, caso tente qualquer movimento desses porém nada ocolorra,\n"
	     "quer dizer que você está em xeque e o rei está em perigo.\n"
	     "Para pedir um empate durante a partida digite :e\n"
	     "Para desistir da partida digite :d");
	     
	pause();
}
/*-----------------------------------*/

int main(){
	char menu_choose;

	do{
		clear_screen();

		printf("\tChess UTF-8\n1. Play\n2. Help\n0. Exit\n\n> ");
		menu_choose = getchar();
		clear_input_buffer();

		if(menu_choose == '1')
			play();
		else if(menu_choose == '2')
			help();
	}while(menu_choose != '0');

	return 0;
}
