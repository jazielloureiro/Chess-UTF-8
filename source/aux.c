#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

void clear_input_buffer(){
	char buffer;

	while((buffer = getchar()) != '\n' && buffer != EOF);
}

void clear_screen(){
	system("clear");
}

void pause(){
	puts("\nPress Enter to continue");
	
	while(getchar() != '\n');
}
