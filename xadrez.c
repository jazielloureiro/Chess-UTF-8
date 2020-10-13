/*
 * Jaziel Loureiro de Sousa         - Matrícula: 496044
 * Iago de Aquino Oliveira          - Matrícula: 494017
 * Pedro Willian de Oliveira Vieira - Matrícula: 496360
 */

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
	
typedef struct{
	casa origem;
	casa destino;
} ultimo_estado;

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

void imprime_menu_superior(char vez, char *peca, bool xeque){
	if (xeque)
	{
		putchar('\n');
		printf("\t┌───────────────────────┐\t┌───────────────────────┐\t┌─────────────┐\n");
		printf("\t│ Vez das peças %s\t│\t│ Peça selecionada: %s\t│\t│ Rei em xeque│\n", (vez == 'B') ? "Brancas" : "Pretas", peca);
		printf("\t└───────────────────────┘\t└───────────────────────┘\t└─────────────┘\n");
		putchar('\n');
	} else {
		putchar('\n');
		printf("\t┌───────────────────────┐\t┌───────────────────────┐\n");
		printf("\t│ Vez das peças %s\t│\t│ Peça selecionada: %s\t│\n", (vez == 'B') ? "Brancas" : "Pretas", peca);
		printf("\t└───────────────────────┘\t└───────────────────────┘\n");
		putchar('\n');
	}
}

//Essa função imprime a situação atual do tabuleiro
void imprime_tabuleiro(casa tabuleiro[TAM][TAM])
{
	//O i será usado para acessar as linhas da matriz
	//O j será usado para acessar as colunas da matriz
	//O k será usado para mostrar o número de cada linha do tabuleiro, semelhante ao a, b, c... Que ficam embaixo do tabuleiro
	int i, j, k = 8;
	
	//Imprime a parte de cima do tabuleiro
	printf("\t  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");

	for (i = 0; i < TAM; i++)
	{
		//Imprime o número da linha do tabuleiro
		printf("\t%d │", k--);

		//Imprime todas as imagens das casas da linha i do tabuleiro
		for (j = 0; j < TAM; j++)
			printf(" %s │", tabuleiro[i][j].imagem);

		//Pula de linha
		putchar('\n');

		//Imprime a separação de cada linha
		if (i < TAM - 1)
			printf("\t  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
	}

	//Imprime a parte de baixo do tabuleiro
	printf("\t  └───┴───┴───┴───┴───┴───┴───┴───┘\n"
	       "\t    a   b   c   d   e   f   g   h\n");
}

/*--------------------------------------*/

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

bool realiza_acoes_usuario(char acao, char vez){
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
			       
		return true;
	}
	
	return false;
}

bool verifica_acoes_usuario(char linha, char coluna, char vez){
	if(coluna == ':' && (linha == 'e' || linha == 'd'))
		return realiza_acoes_usuario(linha, vez);
		
	return false;
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

bool valida_movimento_peao(entradas usuario, char vez)
{
	int avanco2Casas = (vez == BRANCO? -2 : 2),
	    avanco1Casa  = (vez == BRANCO? -1 : 1);

	// Checando o avanço de duas casas
	if(usuario.linha_origem == 6 || usuario.linha_origem == 1)
		if(usuario.linha_origem + avanco2Casas == usuario.linha_destino &&
		   usuario.coluna_origem == usuario.coluna_destino)
		   	return true;
			
	// Checando o avanço de uma casa
	if(usuario.linha_origem + avanco1Casa == usuario.linha_destino){
		int j;

		for(j = usuario.coluna_origem - 1;
		    j <= usuario.coluna_origem + 1;
		    j++)
			if(j == usuario.coluna_destino)
		    		return true;
	}
	
	return false;
}

bool valida_movimento_torre(entradas usuario)
{
	if (usuario.linha_origem == usuario.linha_destino)
		return true;
	if (usuario.coluna_origem == usuario.coluna_destino)
		return true;

	return false;
}

bool valida_movimento_cavalo(entradas usuario)
{
	int difLin, difCol;
	
	difLin = usuario.linha_origem  - usuario.linha_destino;
	difCol = usuario.coluna_origem - usuario.coluna_destino;

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

bool valida_movimento_bispo(entradas usuario)
{
	int i, j;

	// Checando a diagonal superior esquerda
	for(i = usuario.linha_origem - 1, j = usuario.coluna_origem - 1;
	    i >= 0 && j >= 0;
	    i--, j--)
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;

	// Checando a diagonal superior direita
	for(i = usuario.linha_origem - 1, j = usuario.coluna_origem + 1;
	    i >= 0 && j <= 7;
	    i--, j++)
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;

	// Checando a diagonal inferior esquerda
	for(i = usuario.linha_origem + 1, j = usuario.coluna_origem - 1;
	    i <= 7 && j >= 0;
	    i++, j--)
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;

	// Checando a diagonal inferior direita
	for(i = usuario.linha_origem + 1, j = usuario.coluna_origem + 1;
	    i <= 7 && i <= 7;
	    i++, j++)
		if(i == usuario.linha_destino && j == usuario.coluna_destino)
			return true;

	return false;
}

bool valida_movimento_rei(entradas usuario){
	int i, j;

	for(i = usuario.linha_origem - 1; i <= usuario.linha_origem + 1; i++)
		for(j = usuario.coluna_origem - 1; j <= usuario.coluna_origem + 1; j++)
			if(i == usuario.linha_destino && j == usuario.coluna_destino)
				return true;

	return false;
}

bool valida_movimento(char pecaselec, entradas usuario, char vez)
{

	switch (pecaselec)
	{
		case PEAO:
			return valida_movimento_peao(usuario, vez);
		case TORRE:
			return valida_movimento_torre(usuario);
		case CAVALO:
			return valida_movimento_cavalo(usuario);
		case BISPO:
			return valida_movimento_bispo(usuario);
		case REI:
			return valida_movimento_rei(usuario);
		case DAMA:
			if(valida_movimento_torre(usuario) == true)
				return true;
			if(valida_movimento_bispo(usuario) == true)
				return true;
	}
	
	return false;
}

bool checa_colisao(char peca, casa tabuleiro[TAM][TAM], entradas usuario){
	if(peca == PEAO){
		if(usuario.linha_origem  != usuario.linha_destino  &&
		   usuario.coluna_origem != usuario.coluna_destino &&
		   tabuleiro[usuario.linha_destino][usuario.coluna_destino].peca == NULO)
		   	return true;
	}
	
	if(peca != CAVALO){
		int i = usuario.linha_origem, j = usuario.coluna_origem;
		
		do{
			if(i < usuario.linha_destino)
				i++;
			else if(i > usuario.linha_destino)
				i--;
			
			if(j < usuario.coluna_destino)
				j++;
			else if(j > usuario.coluna_destino)
				j--;
			
			if(tabuleiro[i][j].peca != NULO && (i != usuario.linha_destino || j != usuario.coluna_destino))
				return true;
		}while(i != usuario.linha_destino || j != usuario.coluna_destino);
	}
	
	return false;
}

void movimenta_peca(casa tabuleiro[TAM][TAM], entradas *usuario)
{
	//Copia a imagem da peça da casa de origem para a casa destino
	strcpy(tabuleiro[usuario->linha_destino][usuario->coluna_destino].imagem,
	       tabuleiro[usuario->linha_origem][usuario->coluna_origem].imagem);

	//Copia o nome da peça da casa de origem para a casa destino
	tabuleiro[usuario->linha_destino][usuario->coluna_destino].peca = 
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].peca;

	//Copia a cor da peça da casa de origem para a casa destino
	tabuleiro[usuario->linha_destino][usuario->coluna_destino].cor = 
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].cor;

	//Anula a imagem na casa de origem
	strcpy(tabuleiro[usuario->linha_origem][usuario->coluna_origem].imagem, " \0");

	//Anula o nome da peça na casa de origem
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].peca = NULO;

	//Anula a cor da peça na casa de origem
	tabuleiro[usuario->linha_origem][usuario->coluna_origem].cor = NULO;
}

void promocao(casa *promocao, char vez){
	char escolha;

	puts("\nPara qual peça você deseja promover?");
	
	if(vez == BRANCO)
		puts("1. ♕\n2. ♖\n3. ♗\n4. ♘\n");
	else
		puts("1. ♛\n2. ♜\n3. ♝\n4. ♞\n");
	       
	do{
		printf("> ");
		escolha = getchar();
		limpa_buffer();
	}while(escolha < '1' || escolha > '4');
	       
	switch(escolha){
		case '1':
			sprintf(promocao->imagem, vez == BRANCO? "♕" : "♛");
			promocao->peca = DAMA;
			break;
		case '2':
			sprintf(promocao->imagem, vez == BRANCO? "♖" : "♜");
			promocao->peca = TORRE;
			break;
		case '3':
			sprintf(promocao->imagem, vez == BRANCO? "♗" : "♝");
			promocao->peca = BISPO;
			break;
		case '4':
			sprintf(promocao->imagem, vez == BRANCO? "♘" : "♞");
			promocao->peca = CAVALO;
			break;
	}       
}
/*-----------------------------------*/

void salva_estado_tabuleiro(casa tabuleiro[TAM][TAM], ultimo_estado *lance, entradas usuario){
	strcpy(lance->origem.imagem, tabuleiro[usuario.linha_origem][usuario.coluna_origem].imagem);
	lance->origem.peca = tabuleiro[usuario.linha_origem][usuario.coluna_origem].peca;
	lance->origem.cor  = tabuleiro[usuario.linha_origem][usuario.coluna_origem].cor;
	
	strcpy(lance->destino.imagem, tabuleiro[usuario.linha_destino][usuario.coluna_destino].imagem);
	lance->destino.peca = tabuleiro[usuario.linha_destino][usuario.coluna_destino].peca;
	lance->destino.cor  = tabuleiro[usuario.linha_destino][usuario.coluna_destino].cor;
}

void retorna_estado_anterior(casa tabuleiro[TAM][TAM], ultimo_estado lance, entradas usuario){
	strcpy(tabuleiro[usuario.linha_origem][usuario.coluna_origem].imagem, lance.origem.imagem);
	tabuleiro[usuario.linha_origem][usuario.coluna_origem].peca = lance.origem.peca;
	tabuleiro[usuario.linha_origem][usuario.coluna_origem].cor  = lance.origem.cor;
	
	strcpy(tabuleiro[usuario.linha_destino][usuario.coluna_destino].imagem, lance.destino.imagem);
	tabuleiro[usuario.linha_destino][usuario.coluna_destino].peca = lance.destino.peca;
	tabuleiro[usuario.linha_destino][usuario.coluna_destino].cor  = lance.destino.cor;
}

bool checa_xeque(casa tabuleiro[TAM][TAM], char vez){
	entradas xeque;
	char vez_oponente = (vez == BRANCO? PRETO : BRANCO);
	int i, j;
	
	// Procura onde está o rei
	for(i = 0; i < TAM; i++){
		for(j = 0; j < TAM; j++){
			if(tabuleiro[i][j].peca == REI && tabuleiro[i][j].cor == vez){
				xeque.linha_destino = i;
				xeque.coluna_destino = j;
				break;
			}
		}
	}
	
	// Verifica se as peças do oponenete estão ameaçando o rei
	for(i = 0; i < TAM; i++){
		for(j = 0; j < TAM; j++){
			if(tabuleiro[i][j].cor == vez_oponente){
				xeque.linha_origem = i;
				xeque.coluna_origem = j;
			
				if(valida_movimento(tabuleiro[i][j].peca, xeque, vez) &&
				   !checa_colisao(tabuleiro[i][j].peca, tabuleiro, xeque))
				   	return true;
			}
		}
	}
				   	
	return false;
}

/*----------Funções do Menu----------*/
void jogar()
{
	//Declaração da matriz do tabuleiro
	casa tabuleiro[TAM][TAM];

	//Declaração da estrutura que armazenará as entradas do usuário
	entradas usuario;
	
	ultimo_estado lance;

	// Declaração da vez atual no tabuleiro, inicia com Branco
	char vezAtual = BRANCO;

	// Declaração da String Peça selecionada
	char pecaSelecionadaIm[7], pecaSelecionada = NULO;

	inicializa_tabuleiro(tabuleiro);

	do
	{
		do
		{
			do
			{
				limpa_tela();
				imprime_menu_superior(vezAtual, "■", checa_xeque(tabuleiro, vezAtual));
				imprime_tabuleiro(tabuleiro);

				//Recebe as coordenadas da peça que irá se mover
				printf("\nDigite as coordenadas da peça que deseja mover: ");
				ler_casa(&usuario.linha_origem, &usuario.coluna_origem);
				
				//Verifica se um dos jogadores pediu empate ou desistiu da partida
				if(verifica_acoes_usuario(usuario.linha_origem,
							   usuario.coluna_origem,
							   vezAtual) == true)
					return;
				
				//Verifica se a casa lida é válida
				valida_casa(&usuario.linha_origem, &usuario.coluna_origem);

				//Converte os valores que o usuário digitou em valores válidos para a matriz.
				tratamento_entrada_usuario(&usuario.linha_origem, &usuario.coluna_origem);
			} while (tabuleiro[usuario.linha_origem][usuario.coluna_origem].cor != vezAtual);

			strcpy(pecaSelecionadaIm, tabuleiro[usuario.linha_origem][usuario.coluna_origem].imagem);
			pecaSelecionada = tabuleiro[usuario.linha_origem][usuario.coluna_origem].peca;

			do
			{
				limpa_tela();
				imprime_menu_superior(vezAtual, pecaSelecionadaIm, checa_xeque(tabuleiro, vezAtual));
				imprime_tabuleiro(tabuleiro);

				//Recebe as coordenadas de para onde a peça irá se mover
				printf("\nDigite para você onde deseja mover tal peça: ");
				ler_casa(&usuario.linha_destino, &usuario.coluna_destino);
				
				//Verifica se um dos jogadores pediu empate ou desistiu da partida
				if(verifica_acoes_usuario(usuario.linha_destino,
							   usuario.coluna_destino,
							   vezAtual) == true)
					return;
				
				//Verifica se a casa lida é válida
				valida_casa(&usuario.linha_destino, &usuario.coluna_destino);

				//Converte os valores que o usuário digitou em valores válidos para a matriz.
				tratamento_entrada_usuario(&usuario.linha_destino, &usuario.coluna_destino);
			} while (tabuleiro[usuario.linha_destino][usuario.coluna_destino].cor == vezAtual);
			
		}while(!valida_movimento(pecaSelecionada, usuario, vezAtual) || checa_colisao(pecaSelecionada, tabuleiro, usuario));

		// Salva o estado do tabuleiro antes de realizar o lance
		salva_estado_tabuleiro(tabuleiro, &lance, usuario);

		//Realizará o movimento
		movimenta_peca(tabuleiro, &usuario);
		
		// Promoção do peão
		if(pecaSelecionada == PEAO && (usuario.linha_destino == 0 || usuario.linha_destino == 7))
			promocao(&tabuleiro[usuario.linha_destino][usuario.coluna_destino], vezAtual);
			
		if(checa_xeque(tabuleiro, vezAtual) == true){
			retorna_estado_anterior(tabuleiro, lance, usuario);
			continue;
		}

		// Inversao da vez atual no tabuleiro
		vezAtual == BRANCO ? (vezAtual = PRETO) : (vezAtual = BRANCO);
	} while (true);
}

void ajuda(){
	limpa_tela();
	
	puts("Para fazer um movimento ou selecionar um peça digite a letra e o número da casa, sem o uso de espaço (Exemplo: a1)\n"
	     "Aqui vai algumas regras e dicas básicas:\n\n"
	     "1. Peão\n"
	     "Se o peão chegar ao outro lado do tabuleiro poderá ser promovido a uma peça que será de sua escolha.\n"
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
	     "Aqui estão todos os conhecimentos que você vai precisar, caso tente qualquer movimento desses porém nada ocorra,\n"
	     "quer dizer que você está em xeque e o rei está em perigo.\n"
	     "Para pedir um empate durante a partida digite :e\n"
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
