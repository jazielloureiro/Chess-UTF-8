# Esboço da função de movimentação
---
Aqui eu vou apresentar um esboço básico da função de movimentação, perceba que ela é uma das bases do nosso projeto, e todas as outras funções que cuidarão dos movimentos especiais provavelmente estarão contidas dentro dela. Para deixar claro isso é só um esboço e deve ser alterado no decorrer do projeto.

```
int movimentacao(casa *casa_origem, casa *casa_destino, char vez){
	/*Explicando cada um dos parâmetros da função:
	  casa *casa_origem: é um ponteiro para a casa onde está a peça que o jogador deseja movimentar.
	  casa *casa_destino: é um ponteiro para a casa para onde o jogador deseja movimentar sua peça.
	  Observação: o tipo casa é uma estrutura declarada logo no começo do projeto, ela armazena a imagem da peça, o nome dela e a cor.
	  Muito provavelmente, a gente deverá criar uma outra estrutura que irá armazenar além das informações acima, a linha e a coluna
	  das casas em questão.

	  char vez: é uma variável que armazena quem é o jogador da vez, ela será útil para evitar que o jogador de negras movimenta uma
	  peça branca e vice-versa.*/
	  
	int valor_retorno = 0;
	/*Essa variável armazenará o valor que a função retornará, por exemplo: se a função for bem sucedida ela retornará 0,
	  se um jogador tentar mover a peça do seu oponente retornará -1, e assim por diante*/
	    
	//Esse if checará se a peça que o jogador quer mover é de sua cor.
	if(casa_origem->cor != vez)
		valor_retorno = -1; //Se a gente seguir com esse padrão, é bom a gente usar constantes para identificar os valores de retorno
	
	//Daqui por diante eu pensei em um esquema como esse para cada peça
	else if(casa_origem->peca == PEAO){
		//Aqui dentro ele checará se o lance é válido ou não, primeiramente a gente se preocupará com os movimentos básicos.
		//Depois com os movimentos especiais de cada peça, que provavelmente estarão aqui dentro.
	}
	//O padrão acima poderá ser replicado para todas as outras peças
	
	//Feito toda a checagem do lance, a gente pode chamar uma função simples que só movimentará as peças
	movimenta_pecas(casa_origem, casa_destino);
	
	return valor_retorno; //Retorno da função
}
```

Lembrando que isso é só um esboço, e está sujeito a alterações, qualquer dúvida podem perguntar.
