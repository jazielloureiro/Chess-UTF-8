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
	/*Aqui será guardado a imagem de cada peça.
	  Como os caracteres especiais ocupam mais de um byte, é necessário usar um vetor para guardar eles*/
	char imagem[7];
	
	//Aqui será guardado o nome de cada peça
	char peca;
	
	//Aqui será guardado a cor de cada peça
	char cor;
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

//Essa função inicia a matriz do tabuleiro colocando cada peça no seu devido lugar
void inicializa_tabuleiro(casa tabuleiro[TAM][TAM]){
	int i, j;
	
	//Inicializando todas as peças pretas com a cor preto
	for(i = 0; i < 2; i++)
		for(j = 0; j < TAM; j++)
			tabuleiro[i][j].cor = PRETO;
	
	//Inicializando a torre preta da coluna a, com imagem e nome
	sprintf(tabuleiro[0][0].imagem, "♜");
	tabuleiro[0][0].peca = TORRE;

	//Inicializando o cavalo preto da coluna b, com imagem e nome
	sprintf(tabuleiro[0][1].imagem, "♞");
	tabuleiro[0][1].peca = CAVALO;
	
	//Inicializando o bispo preto da coluna c, com imagem e nome
	sprintf(tabuleiro[0][2].imagem, "♝");
	tabuleiro[0][2].peca = BISPO;
	
	//Inicializando a dama preta, com imagem e nome
	sprintf(tabuleiro[0][3].imagem, "♛");
	tabuleiro[0][3].peca = DAMA;
	
	//Inicializando o rei preto, com imagem e nome
	sprintf(tabuleiro[0][4].imagem, "♚");
	tabuleiro[0][4].peca = REI;
	
	//Inicializando o bispo preto da coluna f, com imagem e nome
	sprintf(tabuleiro[0][5].imagem, "♝");
	tabuleiro[0][5].peca = BISPO;

	//Inicializando o cavalo preto da coluna g, com imagem e nome
	sprintf(tabuleiro[0][6].imagem, "♞");
	tabuleiro[0][6].peca = CAVALO;
	
	//Inicializando a torre petra da coluna h, com imagem e nome
	sprintf(tabuleiro[0][7].imagem, "♜");
	tabuleiro[0][7].peca = TORRE;
	
	//Inicializando todos os peões pretos, com imagem e nome
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
	
	//Inicializando todas as peças brancas com a cor branco
	for(i = 6; i < TAM; i++)
		for(j = 0; j < TAM; j++)
			tabuleiro[i][j].cor = BRANCO;
	
	//Inicializando todos os peões brancos, com imagem e nome
	for(i = 0; i < TAM; i++){
		sprintf(tabuleiro[6][i].imagem, "♙");
		tabuleiro[6][i].peca = PEAO;
	}
	
	//Inicializando a torre branca da coluna a, com imagem e nome
	sprintf(tabuleiro[7][0].imagem, "♖");
	tabuleiro[7][0].peca = TORRE;

	//Inicializando o cavalo branco da coluna b, com imagem e nome
	sprintf(tabuleiro[7][1].imagem, "♘");
	tabuleiro[7][1].peca = CAVALO;
	
	//Inicializando o bispo branco da coluna c, com imagem e nome
	sprintf(tabuleiro[7][2].imagem, "♗");
	tabuleiro[7][2].peca = BISPO;
	
	//Inicializando a dama branca, com imagem e nome
	sprintf(tabuleiro[7][3].imagem, "♕");
	tabuleiro[7][3].peca = DAMA;
	
	//Inicializando o rei branco, com imagem e nome
	sprintf(tabuleiro[7][4].imagem, "♔");
	tabuleiro[7][4].peca = REI;
	
	//Inicializando o bispo branco da coluna f, com imagem e nome
	sprintf(tabuleiro[7][5].imagem, "♗");
	tabuleiro[7][5].peca = BISPO;

	//Inicializando o cavalo branco da coluna g, com imagem e nome
	sprintf(tabuleiro[7][6].imagem, "♘");
	tabuleiro[7][6].peca = CAVALO;
	
	//Inicializando a torre branca da coluna h, com imagem e nome
	sprintf(tabuleiro[7][7].imagem, "♖");
	tabuleiro[7][7].peca = TORRE;
}

//Essa função imprime a situação atual do tabuleiro
void imprime_tabuleiro(casa tabuleiro[TAM][TAM], char vez, char peca[7]){
	//O i será usado para acessar as linhas da matriz
	//O j será usado para acessar as colunas da matriz
	//O k será usado para mostrar o número de cada linha do tabuleiro, semelhante ao a, b, c... Que ficam embaixo do tabuleiro
	int i, j, k = 8;

	//Imprime a parte de cima do tabuleiro
	printf("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
	
	for(i = 0; i < TAM; i++){
		//Imprime o número da linha do tabuleiro
		printf("%d │", k--);
	
		//Imprime todas as imagens das casas da linha i do tabuleiro
		for(j = 0; j < TAM; j++)
			printf(" %s │", tabuleiro[i][j].imagem);

		// Menu ao lado do tabuleiro, vez do participante atual e Peca selecionada
		switch (k)
		{
		case 6:
			putchar('\t');	putchar('\t');
			printf("| Vez das peças %s |", (vez == 'B') ? "Brancas" : "Pretas" );
			break;
		case 4:
			putchar('\t');	putchar('\t');
			printf("| Peça selecionada: %s |", peca);
		default:
			break;
		}
		
		//Pula de linha
		putchar('\n');
		
		//Imprime a separação de cada linha
		if(i < TAM - 1)
			printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}
	
	//Imprime a parte de baixo do tabuleiro
	printf("  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "    a   b   c   d   e   f   g   h\n");
}

/*----------Funções de Movimentação----------*/
void tratamento_entrada_usuario(char *linha, char *coluna){
	switch(*linha){
		case '8': *linha = 0; break;
		case '7': *linha = 1; break;
		case '6': *linha = 2; break;
		case '5': *linha = 3; break;
		case '4': *linha = 4; break;
		case '3': *linha = 5; break;
		case '2': *linha = 6; break;
		case '1': *linha = 7; break;
	}
	
	*coluna -= 'a';
}

void recebe_entrada_usuario(char *linha, char *coluna){
	//Recebe a coluna
	*coluna = getchar();

	//Recebe a linha
	*linha = getchar();
	
	//Limpa o lixo no buffer
	limpa_buffer();

	//Valida casa
	while(*coluna < 'a' || *coluna > 'h' || *linha < '1' || *linha > '8'){
		printf("\nVocê digitou uma coordenada inválida! Digite novamente: ");
	
		*coluna = getchar();
	
		*linha = getchar();

		limpa_buffer();
	}
}

bool valida_movimento_torre(entradas usuario){
	if(usuario.linha_origem == usuario.linha_destino)
		return true;
	if(usuario.coluna_origem == usuario.coluna_destino)
		return true;
		
	return false;
}

bool valida_movimento_bispo(entradas usuario){
	int i, j;
	
	i = usuario.linha_origem;
	j = usuario.coluna_origem;
	
	while(i > 0 || j > 0){
		if(i > 0)
			i--;
		
		if(j > 0)
			j--;
			
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}
	
	i = usuario.linha_origem;
	j = usuario.coluna_origem;
	
	while(i > 0 || j < 7){
		if(i > 0)
			i--;
			
		if(j < 7)
			j++;
			
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}
	
	i = usuario.linha_origem;
	j = usuario.coluna_origem;
	
	while(i < 7 || j > 0){
		if(i < 7)
			i++;
			
		if(j > 0)
			j--;
			
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}
	
	i = usuario.linha_origem;
	j = usuario.coluna_origem;
	
	while(i < 7 || j < 7){
		if(i < 7)
			i++;
			
		if(j < 7)
			j++;
			
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}
	
	return false;
}

bool valida_movimento_cavalo(){
	

}

bool valida_movimento(entradas usuario, char peca){
	if(peca == TORRE)
		return valida_movimento_torre(usuario);
	else if(peca == BISPO)
		return valida_movimento_bispo(usuario);
	else if(peca == CAVALO)
		return valida_movimento_cavalo(usuario);
	else if(peca == DAMA){
		if(valida_movimento_torre(usuario) == true)
			return true;
		if(valida_movimento_bispo(usuario) == true)
			return true;
	}
	
	return false;
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
/*-----------------------------------*/

/*----------Funções do Menu----------*/
void jogar(){
	//Declaração da matriz do tabuleiro
	casa tabuleiro[TAM][TAM];

	//Declaração da estrutura que armazenará as entradas do usuário
	entradas usuario;
	
	bool rodando = true;
	
	// Declaração da vez atual no tabuleiro, inicia com Branco
	char vezAtual = 'B';

	// Declaração da String Peça selecionada
	char pecaSelecionada[7] = "\u25A0";

	inicializa_tabuleiro(tabuleiro);
	
	do{
		do{
			limpa_tela();
			imprime_tabuleiro(tabuleiro, vezAtual, pecaSelecionada);
		
			//Recebe as coordenadas da peça que irá se mover
			printf("\nDigite as coordenadas da peça que deseja mover: ");
			recebe_entrada_usuario(&usuario.linha_origem, &usuario.coluna_origem);
			
			//Converte os valores que o usuário digitou em valores válidos para a matriz.
			tratamento_entrada_usuario(&usuario.linha_origem, &usuario.coluna_origem);
		}while(tabuleiro[usuario.linha_origem][usuario.coluna_origem].cor != vezAtual);
		
		strcpy(pecaSelecionada, tabuleiro[usuario.linha_origem][usuario.coluna_origem].imagem);
		
		do{
			limpa_tela();
			imprime_tabuleiro(tabuleiro, vezAtual, pecaSelecionada);
		
			//Recebe as coordenadas de para onde a peça irá se mover
			printf("\nDigite para você onde deseja mover tal peça: ");
			recebe_entrada_usuario(&usuario.linha_destino, &usuario.coluna_destino);
			
			//Converte os valores que o usuário digitou em valores válidos para a matriz.
			tratamento_entrada_usuario(&usuario.linha_destino, &usuario.coluna_destino);
		}while(tabuleiro[usuario.linha_destino][usuario.coluna_destino].cor == vezAtual);
		
		//Valida o movimento
		valida_movimento(usuario, tabuleiro[usuario.linha_origem][usuario.coluna_origem].peca);
		
		//Realizará o movimento
		movimenta_peca(tabuleiro, &usuario);

		// Inversao da vez atual no tabuleiro
		vezAtual == 'B'? (vezAtual = 'P') : (vezAtual = 'B');
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
