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

//Constantes para identificar a cor de cada peça
#define BRANCO 'B'
#define PRETO 'P'

//Quando a casa estiver vazia essa constante será usada para identificá-la
#define NULO 'N'

//Constantes para identificar o fim de jogo
#define FIM_DE_JOGO 'F'
#define CONTINUA 'C'

//Estrutura para armazenar as informações de cada casa do tabuleiro
typedef struct
{
	/*Aqui será guardado a imagem de cada peça.
	  Como os caracteres especiais ocupam mais de um byte, é necessário usar um vetor para guardar eles*/
	char imagem[7];

	//Aqui será guardado o nome de cada peça
	char peca;

	//Aqui será guardado a cor de cada peça
	char cor;
} casa;

//Estrutura para armazenar as entradas do usurário
typedef struct
{
	char linha_origem;
	char coluna_origem;
	char linha_destino;
	char coluna_destino;
} entradas;

/*----------Funções Auxiliares----------*/
//Essa função deve funcionar tanto no Linux como no Windows
void limpa_tela()
{
	system("clear || cls");
}

//Função usada para limpar o lixo que fica no buffer após alguma leitura
void limpa_buffer()
{
	char buffer;

	while ((buffer = getchar()) != '\n' && buffer != EOF);
}

void pausa_tela(){
	puts("\nPressione Enter para continuar");
	
	while(getchar() != '\n');
}
/*--------------------------------------*/

//Essa função inicia a matriz do tabuleiro colocando cada peça no seu devido lugar
void inicializa_tabuleiro(casa tabuleiro[TAM][TAM])
{
	int i, j;

	//Inicializando todas as peças pretas com a cor preto
	for (i = 0; i < 2; i++)
		for (j = 0; j < TAM; j++)
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
	for (i = 0; i < TAM; i++)
	{
		sprintf(tabuleiro[1][i].imagem, "♟");
		tabuleiro[1][i].peca = PEAO;
	}

	//Inicializando as casas vazias do meio do tabuleiro
	for (i = 2; i < 6; i++)
	{
		for (j = 0; j < TAM; j++)
		{
			sprintf(tabuleiro[i][j].imagem, " ");
			tabuleiro[i][j].peca = NULO;
			tabuleiro[i][j].cor = NULO;
		}
	}

	//Inicializando todas as peças brancas com a cor branco
	for (i = 6; i < TAM; i++)
		for (j = 0; j < TAM; j++)
			tabuleiro[i][j].cor = BRANCO;

	//Inicializando todos os peões brancos, com imagem e nome
	for (i = 0; i < TAM; i++)
	{
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
void imprime_tabuleiro(casa tabuleiro[TAM][TAM], char vez, char peca[7])
{
	//O i será usado para acessar as linhas da matriz
	//O j será usado para acessar as colunas da matriz
	//O k será usado para mostrar o número de cada linha do tabuleiro, semelhante ao a, b, c... Que ficam embaixo do tabuleiro
	int i, j, k = 8;

	//Imprime a parte de cima do tabuleiro
	printf("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

	for (i = 0; i < TAM; i++)
	{
		//Imprime o número da linha do tabuleiro
		printf("%d │", k--);

		//Imprime todas as imagens das casas da linha i do tabuleiro
		for (j = 0; j < TAM; j++)
			printf(" %s │", tabuleiro[i][j].imagem);

		// Menu ao lado do tabuleiro, vez do participante atual e Peca selecionada
		switch (k)
		{
			case 6:
				printf("\t\t| Vez das peças %s |", (vez == 'B') ? "Brancas" : "Pretas");
				break;
			case 4:
				printf("\t\t| Peça selecionada: %s |", peca);
				break;
		}

		//Pula de linha
		putchar('\n');

		//Imprime a separação de cada linha
		if (i < TAM - 1)
			printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}

	//Imprime a parte de baixo do tabuleiro
	printf("  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
		   "    a   b   c   d   e   f   g   h\n");
}

/*----------Funções de Movimentação----------*/
//Ler as coordenadas de uma casa
void ler_casa(char *linha, char *coluna){
	//Recebe a coluna
	*coluna = getchar();

	//Recebe a linha
	*linha = getchar();

	//Limpa o lixo no buffer
	limpa_buffer();
}

char realiza_acoes_usuario(char acao, char vez){
	char escolha;
	
	if(acao == 'd')
		printf("Você deseja desistir da partida? [s/n] ");
	else
		printf("O jogador de %s ofereceu um empate, o jogador de %s aceita? [s/n] ",
		       vez == BRANCO? "Brancas" : "Negras",
		       vez == BRANCO? "Negras"  : "Brancas");

	do{
		escolha = getchar();
		limpa_buffer();
	}while(escolha != 's' && escolha != 'n');
	
	if(escolha == 's'){
		if(acao == 'd')
			printf("Fim da partida! O jogador de %s desistiu da partida!\n",
			       vez == BRANCO? "Brancas" : "Negras");
		else
			printf("Fim da partida! Ambos os jogadores concordaram em um empate!\n");
			
		pausa_tela();
			       
		return FIM_DE_JOGO;
	}
	
	return CONTINUA;
}

char verifica_acoes_usuario(char linha, char coluna, char vez){
	if(coluna == ':' && (linha == 'e' || linha == 'd'))
		return realiza_acoes_usuario(linha, vez);
		
	return CONTINUA;
}

//verifica se as coordenadas lidas são válidas
void valida_casa(char *linha, char *coluna){
	while(*coluna < 'a' || *coluna > 'h' || *linha < '1' || *linha > '8'){
		printf("\nVocê digitou uma coordenada inválida! Digite novamente: ");
		ler_casa(linha, coluna);
	}
}

void tratamento_entrada_usuario(char *linha, char *coluna)
{
	switch (*linha)
	{
		case '8':
			*linha = 0;
			break;
		case '7':
			*linha = 1;
			break;
		case '6':
			*linha = 2;
			break;
		case '5':
			*linha = 3;
			break;
		case '4':
			*linha = 4;
			break;
		case '3':
			*linha = 5;
			break;
		case '2':
			*linha = 6;
			break;
		case '1':
			*linha = 7;
	}

	*coluna -= 'a';
}

/*void recebe_entrada_usuario(char *linha, char *coluna)
{
	//Recebe a coluna
	*coluna = getchar();

	//Recebe a linha
	*linha = getchar();

	//Limpa o lixo no buffer
	limpa_buffer();
	
	if(*coluna == ':' && (*linha == 'e' || *linha == 'd'))
		if(acoes_usuario(*linha, BRANCO) == FIM_DE_JOGO)
			exit(1);

	//Valida casa
	while (*coluna < 'a' || *coluna > 'h' || *linha < '1' || *linha > '8')
	{
		printf("\nVocê digitou uma coordenada inválida! Digite novamente: ");

		*coluna = getchar();

		*linha = getchar();

		limpa_buffer();
	}
}*/

bool valida_movimento_torre(entradas usuario)
{
	if (usuario.linha_origem == usuario.linha_destino)
		return true;
	if (usuario.coluna_origem == usuario.coluna_destino)
		return true;

	return false;
}

bool valida_movimento_bispo(entradas usuario)
{
	int i, j;

	i = usuario.linha_origem;
	j = usuario.coluna_origem;

	while (i > 0 || j > 0)
	{
		if (i > 0)
			i--;

		if (j > 0)
			j--;

		if (i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}

	i = usuario.linha_origem;
	j = usuario.coluna_origem;

	while (i > 0 || j < 7)
	{
		if (i > 0)
			i--;

		if (j < 7)
			j++;

		if (i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}

	i = usuario.linha_origem;
	j = usuario.coluna_origem;

	while (i < 7 || j > 0)
	{
		if (i < 7)
			i++;

		if (j > 0)
			j--;

		if (i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}

	i = usuario.linha_origem;
	j = usuario.coluna_origem;

	while (i < 7 || j < 7)
	{
		if (i < 7)
			i++;

		if (j < 7)
			j++;

		if (i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;
	}

	return false;
}

bool valida_movimento_cavalo(entradas usuario)
{
	int difLin, difCol, lin, col, linf, colf;

	lin = usuario.linha_origem;
	col = usuario.coluna_origem;
	linf = usuario.linha_destino;
	colf = usuario.coluna_destino;
	
	difLin = lin - linf;
	difCol = col - colf;

	if (difLin == 2  && difCol == 1  ||
	    difLin == 2  && difCol == -1 ||
	    difLin == -2 && difCol == 1  ||
	    difLin == -2 && difCol == -1 ||
	    difCol == 2  && difLin == 1  ||
	    difCol == 2  && difLin == -1 ||
	    difCol == -2 && difLin == 1  ||
	    difCol == -2 && difLin == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool valida_movimento_peao(entradas usuario, char vez)
{
}

bool valida_movimento(char pecaselec, entradas usuario, char vez)
{

	switch (pecaselec)
	{
		case PEAO:
			//return valida_movimento_peao(usuario, vez);
			break;
		case TORRE:
			return valida_movimento_torre(usuario);
			break;
		case DAMA:
			if(valida_movimento_torre(usuario) == true)
				return true;
			if(valida_movimento_bispo(usuario) == true)
				return true;
			break;
		case CAVALO:
			return valida_movimento_cavalo(usuario);
			break;
		case REI:

			break;
		case BISPO:
			return valida_movimento_bispo(usuario);
	}
	
	//FIXME: Quando todas as funções de validação estiverem prontas, mudar para false
	return true;
}

bool checa_colisao(char peca, casa tabuleiro[TAM][TAM], entradas usuario){
	int i = usuario.linha_origem, j = usuario.coluna_origem;
	
	if(peca != CAVALO){
		while(i != usuario.linha_destino || j != usuario.coluna_destino){
			if(i < usuario.linha_destino)
				i++;
			else if(i > usuario.linha_destino)
				i--;
			
			if(j < usuario.coluna_destino)
				j++;
			else if(j > usuario.coluna_destino)
				j--;
			
			if(tabuleiro[i][j].peca != NULO)
				return true;
		}
	}
	
	return false;
}

/*
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
}*/

void movimenta_peca(casa tabuleiro[TAM][TAM], entradas *usuario)
{
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
void jogar()
{
	//Declaração da matriz do tabuleiro
	casa tabuleiro[TAM][TAM];

	//Declaração da estrutura que armazenará as entradas do usuário
	entradas usuario;

	// Declaração da vez atual no tabuleiro, inicia com Branco
	char vezAtual = BRANCO;

	// Declaração da String Peça selecionada
	char pecaselecionadaIm[7] = "\u25A0", pecaselecionada = NULO;

	inicializa_tabuleiro(tabuleiro);

	do
	{
		do
		{
			do
			{
				limpa_tela();
				imprime_tabuleiro(tabuleiro, vezAtual, pecaselecionadaIm);

				//Recebe as coordenadas da peça que irá se mover
				printf("\nDigite as coordenadas da peça que deseja mover: ");
				ler_casa(&usuario.linha_origem, &usuario.coluna_origem);
				
				//Verifica se um dos jogadores pediu empate ou desistiu da partida
				if(verifica_acoes_usuario(usuario.linha_origem,
							   usuario.coluna_origem,
							   vezAtual) == FIM_DE_JOGO)
					return;
				
				//Verifica se a casa lida é válida
				valida_casa(&usuario.linha_origem, &usuario.coluna_origem);

				//Converte os valores que o usuário digitou em valores válidos para a matriz.
				tratamento_entrada_usuario(&usuario.linha_origem, &usuario.coluna_origem);
			} while (tabuleiro[usuario.linha_origem][usuario.coluna_origem].cor != vezAtual);

			pecaselecionada = tabuleiro[usuario.linha_origem][usuario.coluna_origem].peca;
			strcpy(pecaselecionadaIm, tabuleiro[usuario.linha_origem][usuario.coluna_origem].imagem);

			do
			{
				limpa_tela();
				imprime_tabuleiro(tabuleiro, vezAtual, pecaselecionadaIm);

				//Recebe as coordenadas de para onde a peça irá se mover
				printf("\nDigite para você onde deseja mover tal peça: ");
				ler_casa(&usuario.linha_destino, &usuario.coluna_destino);
				
				//Verifica se um dos jogadores pediu empate ou desistiu da partida
				if(verifica_acoes_usuario(usuario.linha_destino,
							   usuario.coluna_destino,
							   vezAtual) == FIM_DE_JOGO)
					return;

				//Converte os valores que o usuário digitou em valores válidos para a matriz.
				tratamento_entrada_usuario(&usuario.linha_destino, &usuario.coluna_destino);
			} while (tabuleiro[usuario.linha_destino][usuario.coluna_destino].cor == vezAtual);
			
		}while(!valida_movimento(pecaselecionada, usuario, vezAtual) || checa_colisao(pecaselecionada, tabuleiro, usuario));

		//Valida o movimento
		//valida_movimento(pecaselecionada, usuario);

		//Realizará o movimento
		movimenta_peca(tabuleiro, &usuario);

		// Inversao da vez atual no tabuleiro
		vezAtual == BRANCO ? (vezAtual = PRETO) : (vezAtual = BRANCO);
	} while (true);
}

void ajuda(){
	limpa_tela();

	puts("Para pedir um empate durante a partida digite :e\n"
	     "Para desistir da partida digite :d");
	     
	pausa_tela();
}
/*-----------------------------------*/

int main()
{
	char escolha;

	//Imprimindo o menu
	do
	{
		limpa_tela();

		printf("\tXadrez UTF-8\n1. Jogar\n2. Ajuda\n0. Sair\n\n> ");
		escolha = getchar();
		limpa_buffer();

		if (escolha == '1')
			jogar();
		else if(escolha == '2')
			ajuda();
	} while (escolha != '0');

	return 0;
}
