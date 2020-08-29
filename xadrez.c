#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TAM 8

//Constantes para identificar cada peça
#define BISPO 'B'
#define CAVALO 'C'
#define DAMA 'D'
#define PEAO 'P'
#define REI 'R'
#define TORRE 'T'

//Constante para identificar a cor de cada peça
#define BRANCO 'B'
#define PRETO 'P'

//Quando a casa estiver vazia essa constante será usada para identificá-la
#define NULO 'N'

bool rodando = true;

typedef struct{
	char imagem[7]; //Como os caracteres especiais ocupam mais de um byte, é necessário usar um vetor para guardar eles
	char peca;      //Aqui será guardado o nome de cada peça
	char cor;       //E aqui será guardado a cor
} casa;

void inicializa_tabuleiro(casa tabuleiro[TAM][TAM]){
	int i, j;
	
	//Inicializando cada peça preta com a cor preto
	for(i = 0; i < 2; i++)
		for(j = 0; j < TAM; j++)
			tabuleiro[i][j].cor = PRETO;
			
	//Como cada caractere especial é guardado em um vetor, é necessário usar essa função
	sprintf(tabuleiro[0][0].imagem, "♜");
	tabuleiro[0][0].peca = TORRE;

	sprintf(tabuleiro[0][1].imagem, "♞");
	tabuleiro[0][1].peca = CAVALO;
	
	sprintf(tabuleiro[0][2].imagem, "♝");
	tabuleiro[0][2].peca = BISPO;
	
	sprintf(tabuleiro[0][3].imagem, "♛");
	tabuleiro[0][3].peca = DAMA;
	
	sprintf(tabuleiro[0][4].imagem, "♚");
	tabuleiro[0][4].peca = REI;
	
	sprintf(tabuleiro[0][5].imagem, "♝");
	tabuleiro[0][5].peca = BISPO;

	sprintf(tabuleiro[0][6].imagem, "♞");
	tabuleiro[0][6].peca = CAVALO;
	
	sprintf(tabuleiro[0][7].imagem, "♜");
	tabuleiro[0][7].peca = TORRE;
	
	for(i = 0; i < TAM; i++){
		sprintf(tabuleiro[1][i].imagem, "♟");
		tabuleiro[1][i].peca = PEAO;
	}
	
	//Inicializando as casas vazias do meio do tabuleiro
	for(i = 2; i < 6; i++){
		for(j = 0; j < TAM; j++){
			sprintf(tabuleiro[i][j].imagem, " ");
			tabuleiro[i][j].peca = NULO;
			tabuleiro[i][j].peca = NULO;
		}
	}
	
	//Inicializando cada peça branca com a cor branco
	for(i = 6; i < TAM; i++)
		for(j = 0; j < TAM; j++)
			tabuleiro[i][j].cor = BRANCO;
	
	for(i = 0; i < TAM; i++){
		sprintf(tabuleiro[6][i].imagem, "♙");
		tabuleiro[6][i].peca = PEAO;
	}
	
	sprintf(tabuleiro[7][0].imagem, "♖");
	tabuleiro[7][0].peca = TORRE;

	sprintf(tabuleiro[7][1].imagem, "♘");
	tabuleiro[7][1].peca = CAVALO;
	
	sprintf(tabuleiro[7][2].imagem, "♗");
	tabuleiro[7][2].peca = BISPO;
	
	sprintf(tabuleiro[7][3].imagem, "♕");
	tabuleiro[7][3].peca = DAMA;
	
	sprintf(tabuleiro[7][4].imagem, "♔");
	tabuleiro[7][4].peca = REI;
	
	sprintf(tabuleiro[7][5].imagem, "♗");
	tabuleiro[7][5].peca = BISPO;

	sprintf(tabuleiro[7][6].imagem, "♘");
	tabuleiro[7][6].peca = CAVALO;
	
	sprintf(tabuleiro[7][7].imagem, "♖");
	tabuleiro[7][7].peca = TORRE;
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

void movimentar_peca(casa tabuleiro[TAM][TAM]){ // posteriormente adicionar os casos de uso
	casa salvarpeca; // instancia da peca a ser movida
	int line, pieceline, piececolumn;
	char column;
	printf("\n\tDigite as coordenadas da peça que deseja mover:");
	printf("\n\tLinhas 1-8:");
	scanf("%d",&line);
	printf("\n\tColunas a-h:");
	scanf(" %c",&column);
	// definição da linha final
	//pieceline = line -1;
	switch (line)
	{
	case 8:
		pieceline = 0;
		break;
	case 7:
		pieceline = 1;
		break;
	case 6:
		pieceline = 2;
		break;
	case 5:
		pieceline = 3;
		break;
	case 4:
		pieceline = 4;
		break;
	case 3:
		pieceline = 5;
		break;
	case 2:
		pieceline = 6;
		break;
	case 1:
		pieceline = 7;
		break;
	default:
		pieceline = -1;
		break;
	}
	// definição da coluna final
	switch (column)
	{
	case 'a':
		piececolumn = 0;
		break;
	case 'b':
		piececolumn = 1;
		break;
	case 'c':
		piececolumn = 2;
		break;
	case 'd':
		piececolumn = 3;
		break;
	case 'e':
		piececolumn = 4;
		break;
	case 'f':
		piececolumn = 5;
		break;
	case 'g':
		piececolumn = 6;
		break;
	case 'h':
		piececolumn = 7;
		break;
	default:
		piececolumn = -1;
		break;
	}

	int tomoveline, tomovecolumn;
	printf("\n\tDigite para onde deseja mover tal peça:");
	printf("\n\tLinhas 1-8:");
	scanf("%d",&line);
	printf("\n\tColunas a-h:");
	scanf(" %c",&column);
	// definição da linha final
	//tomoveline = line -1;
	// definição da coluna final
	switch (line)
	{
	case 8:
		tomoveline = 0;
		break;
	case 7:
		tomoveline = 1;
		break;
	case 6:
		tomoveline = 2;
		break;
	case 5:
		tomoveline = 3;
		break;
	case 4:
		tomoveline = 4;
		break;
	case 3:
		tomoveline = 5;
		break;
	case 2:
		tomoveline = 6;
		break;
	case 1:
		tomoveline = 7;
		break;
	default:
		tomoveline = -1;
		break;
	}
	switch (column)
	{
	case 'a':
		tomovecolumn = 0;
		break;
	case 'b':
		tomovecolumn = 1;
		break;
	case 'c':
		tomovecolumn = 2;
		break;
	case 'd':
		tomovecolumn = 3;
		break;
	case 'e':
		tomovecolumn = 4;
		break;
	case 'f':
		tomovecolumn = 5;
		break;
	case 'g':
		tomovecolumn = 6;
		break;
	case 'h':
		tomovecolumn = 7;
		break;
	default:
		tomovecolumn = -1;
		break;
	}
	
	strcpy(tabuleiro[tomoveline][tomovecolumn].imagem, tabuleiro[pieceline][piececolumn].imagem);
	tabuleiro[tomoveline][tomovecolumn].peca = tabuleiro[pieceline][piececolumn].peca;
	tabuleiro[tomoveline][tomovecolumn].cor = tabuleiro[pieceline][piececolumn].cor;

	sprintf(tabuleiro[pieceline][piececolumn].imagem, " ");
	tabuleiro[pieceline][piececolumn].peca = NULO;
	tabuleiro[pieceline][piececolumn].cor = NULO;
	
	/*/ atribuindo os valores atuais do tabuleiro na instancia da peca
	salvarpeca.peca = tabuleiro[pieceline][piececolumn].peca;
	salvarpeca.cor = tabuleiro[pieceline][piececolumn].cor;
	sprintf(salvarpeca.imagem[7], tabuleiro[pieceline][piececolumn].imagem[7]);

	// retornando ao tabuleiro os valores atualizados
	
	sprintf(tabuleiro[tomoveline][tomovecolumn].imagem[7], salvarpeca.imagem[7]);
	tabuleiro[tomoveline][tomovecolumn].peca = salvarpeca.peca;
	tabuleiro[tomoveline][tomovecolumn].cor = salvarpeca.cor;*/
	
	system("clear || cls");
	imprime_tabuleiro(tabuleiro);
}

int main(){
	casa tabuleiro[TAM][TAM];

	inicializa_tabuleiro(tabuleiro);
	imprime_tabuleiro(tabuleiro);
	/*
	 Para movimentar uma peça precisamos da posição atual da qual a peça está, o que é definido por suas coordenadas, para realizar a movimentação da peça o usuário terá que digitar as coordenadas atuais da peça e para onde quer movê-la. 
	 Como realizar isto:
	 O tabuleiro se manterá intácto e abaixo dele aparecerá um chat para escrever a posição e para onde quer mover, quando finalizado a tela atualizará, o tabuleiro se manterá no mesmo lugar porém com a peça atualizada e as perguntas resetarão.
	 O movimento começa pelas brancas e as pretas em sequência.
	*/
	while (rodando)
	{
		movimentar_peca(tabuleiro);
	}
	
	return 0;
}
