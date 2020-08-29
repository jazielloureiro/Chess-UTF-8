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

//Estrutura para armazenar as informações de cada casa do tabuleiro
typedef struct{
	char imagem[7]; //Como os caracteres especiais ocupam mais de um byte, é necessário usar um vetor para guardar eles
	char peca;      //Aqui será guardado o nome de cada peça
	char cor;       //E aqui será guardado a cor
} casa;
	
//Estrutura para armazenar as entradas do usurário
typedef struct{
	char linha_origem;
	char coluna_origem;
	char linha_destino;
	char coluna_destino;
} entradas;

/*----------Funções Auxiliares----------*/
//Essa função deve funcionar tanto no Linux como no Windows
void limpa_tela(){
	system("clear || cls");
}

//Função usada para limpar o lixo que fica no buffer após alguma leitura
void limpa_buffer(){
	char buffer;
	
	while((buffer = getchar()) != '\n' && buffer != EOF);
}
/*--------------------------------------*/

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

void recebe_entradas_usuario(entradas *usuario){
	printf("\nDigite as coordenadas da peça que deseja mover: ");
	
	//Recebe a coluna da peça que ele deseja mover
	usuario->coluna_origem = getchar();
	
	//Recebe a linha da peça que ele deseja mover
	usuario->linha_origem = getchar();
	
	//Limpa o lixo no buffer
	limpa_buffer();
	
	//Checa se o usuário digitou algum valor errado
	while(usuario->coluna_origem < 'a' || usuario->coluna_origem > 'h' || usuario->linha_origem < '1' || usuario->linha_origem > '8'){
		printf("\nVocê digitou uma coordenada inválida! Digite novamente: ");
	
		usuario->coluna_origem = getchar();
	
		usuario->linha_origem = getchar();
	
		limpa_buffer();
	}
	//O loop irá se repetir até o usuário entrar com uma coordenada válida
	
	//Agora é fazer a mesma coisa para receber as coordenadas para onde ele quer mover
	printf("\nDigite para você onde deseja mover tal peça: ");
	
	usuario->coluna_destino = getchar();
	
	usuario->linha_destino = getchar();
	
	limpa_buffer();
	
	//Checa se o usuário digitou algum valor errado
	while(usuario->coluna_destino < 'a' || usuario->coluna_destino > 'h' || usuario->linha_destino < '1' || usuario->linha_destino > '8'){
		printf("\nVocê digitou uma coordenada inválida! Digite novamente: ");
	
		usuario->coluna_destino = getchar();
	
		usuario->linha_destino = getchar();
	
		limpa_buffer();
	}
}

void tratamento_entradas_usuario(entradas *usuario){
	//Converte a linha de origem
	switch(usuario->linha_origem){
		case '8': usuario->linha_origem = 0; break;
		case '7': usuario->linha_origem = 1; break;
		case '6': usuario->linha_origem = 2; break;
		case '5': usuario->linha_origem = 3; break;
		case '4': usuario->linha_origem = 4; break;
		case '3': usuario->linha_origem = 5; break;
		case '2': usuario->linha_origem = 6; break;
		case '1': usuario->linha_origem = 7; break;
	}
	
	//Converte a coluna de origem
	usuario->coluna_origem -= 'a';
	
	//Converte a linha de destino
	switch(usuario->linha_destino){
		case '8': usuario->linha_destino = 0; break;
		case '7': usuario->linha_destino = 1; break;
		case '6': usuario->linha_destino = 2; break;
		case '5': usuario->linha_destino = 3; break;
		case '4': usuario->linha_destino = 4; break;
		case '3': usuario->linha_destino = 5; break;
		case '2': usuario->linha_destino = 6; break;
		case '1': usuario->linha_destino = 7; break;
	}
	
	//Converte a coluna de destino
	usuario->coluna_destino -= 'a';
}

void movimenta_peca(casa tabuleiro[TAM][TAM], entradas *usuario){
	//Copia a imagem da peça da casa de origem para a casa destino
	strcpy(tabuleiro[usuario->linha_destino][usuario->coluna_destino].imagem, tabuleiro[usuario->linha_origem][usuario->coluna_origem].imagem);
	
	//Copia o nome da peça da casa de origem para a casa destino
	tabuleiro[usuario->linha_destino][usuario->coluna_destino].peca = tabuleiro[usuario->linha_origem][usuario->coluna_origem].peca;
	
	//Copia a cor da peça da casa de origem para a casa destino
	tabuleiro[usuario->linha_destino][usuario->coluna_destino].cor = tabuleiro[usuario->linha_origem][usuario->coluna_origem].cor;

	//Anula a imagem na casa de origem
	strcpy(tabuleiro[usuario->linha_origem][usuario->coluna_origem].imagem, " \0");

	//Anula o nome da peça na casa de origem
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].peca = NULO;

	//Anula a cor da peça na casa de origem
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].cor = NULO;
}

/*----------Funções do Menu----------*/
void jogar(){
	//Declaração da matriz do tabuleiro
	casa tabuleiro[TAM][TAM];

	//Declaração da estrutura que armazenará as entradas do usuário
	entradas usuario;
	
	bool rodando = true;

	inicializa_tabuleiro(tabuleiro);
	
	do{
		limpa_tela();
		imprime_tabuleiro(tabuleiro);
		
		//Irá receber as coordenadas da peça que irá se mover e para onde ela vai
		recebe_entradas_usuario(&usuario);
		
		//Converte os valores que o usuário digitou em valores válidos para a matriz. Exemplo: 'a' vai virar 0, e assim por diante
		tratamento_entradas_usuario(&usuario);
		
		//Realizará o movimento
		movimenta_peca(tabuleiro, &usuario);
	}while(rodando);
}
/*-----------------------------------*/

int main(){
	char escolha;
	
	//Imprimindo o menu
	do{
		limpa_tela();
		
		printf("\tXadrez UTF-8\n1. Jogar\n0. Sair\n\n> ");
		escolha = getchar();
		limpa_buffer();
	
		if(escolha == '1')
			jogar();
	}while(escolha != '0');
	
	return 0;
}