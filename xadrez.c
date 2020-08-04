#include <stdio.h>

#define TAM 8

typedef struct{
	char imagem[7];
	char peca[7];
	char cor[7];
} casa;

void inicializa_tabuleiro(casa tabuleiro[TAM][TAM]){
	int i, j;
	
	for(i = 0; i < 2; i++)
		for(j = 0; j < TAM; j++)
			sprintf(tabuleiro[i][j].cor, "Preto");
	
	sprintf(tabuleiro[0][0].imagem, "♜");
	sprintf(tabuleiro[0][0].peca, "Torre");

	sprintf(tabuleiro[0][1].imagem, "♞");
	sprintf(tabuleiro[0][1].peca, "Cavalo");
	
	sprintf(tabuleiro[0][2].imagem, "♝");
	sprintf(tabuleiro[0][2].peca, "Bispo");
	
	sprintf(tabuleiro[0][3].imagem, "♛");
	sprintf(tabuleiro[0][3].peca, "Dama");
	
	sprintf(tabuleiro[0][4].imagem, "♚");
	sprintf(tabuleiro[0][4].peca, "Rei");
	
	sprintf(tabuleiro[0][5].imagem, "♝");
	sprintf(tabuleiro[0][5].peca, "Bispo");

	sprintf(tabuleiro[0][6].imagem, "♞");
	sprintf(tabuleiro[0][6].peca, "Cavalo");
	
	sprintf(tabuleiro[0][7].imagem, "♜");
	sprintf(tabuleiro[0][7].peca, "Torre");
	
	for(i = 0; i < TAM; i++){
		sprintf(tabuleiro[1][i].imagem, "♟︎");
		sprintf(tabuleiro[1][i].peca, "Peao");
	}
	
	for(i = 2; i < 6; i++){
		for(j = 0; j < TAM; j++){
			sprintf(tabuleiro[i][j].imagem, " ");
			sprintf(tabuleiro[i][j].peca, "Nulo");
			sprintf(tabuleiro[i][j].cor, "Nulo");
		}
	}
	
	for(i = 6; i < TAM; i++)
		for(j = 0; j < TAM; j++)
			sprintf(tabuleiro[i][j].cor, "Branco");
	
	for(i = 0; i < TAM; i++){
		sprintf(tabuleiro[6][i].imagem, "♙");
		sprintf(tabuleiro[6][i].peca, "Peao");
	}
	
	sprintf(tabuleiro[7][0].imagem, "♖");
	sprintf(tabuleiro[7][0].peca, "Torre");

	sprintf(tabuleiro[7][1].imagem, "♘");
	sprintf(tabuleiro[7][1].peca, "Cavalo");
	
	sprintf(tabuleiro[7][2].imagem, "♗");
	sprintf(tabuleiro[7][2].peca, "Bispo");
	
	sprintf(tabuleiro[7][3].imagem, "♕");
	sprintf(tabuleiro[7][3].peca, "Dama");
	
	sprintf(tabuleiro[7][4].imagem, "♔");
	sprintf(tabuleiro[7][4].peca, "Rei");
	
	sprintf(tabuleiro[7][5].imagem, "♗");
	sprintf(tabuleiro[7][5].peca, "Bispo");

	sprintf(tabuleiro[7][6].imagem, "♘");
	sprintf(tabuleiro[7][6].peca, "Cavalo");
	
	sprintf(tabuleiro[7][7].imagem, "♖");
	sprintf(tabuleiro[7][7].peca, "Torre");
}

void imprime_tabuleiro(casa tabuleiro[TAM][TAM]){
	int i, j, k = 8;

	printf("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
	
	for(i = 0; i < TAM; i++){
		printf("%d │", k--);
	
		for(j = 0; j < TAM; j++)
			printf(" %s │", tabuleiro[i][j].imagem);
			
		putchar('\n');
		
		if(i < TAM - 1)
			printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}
	
	printf("  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "    a   b   c   d   e   f   g   h\n");
}

int main(){
	casa tabuleiro[TAM][TAM];

	inicializa_tabuleiro(tabuleiro);
	imprime_tabuleiro(tabuleiro);

	return 0;
}
