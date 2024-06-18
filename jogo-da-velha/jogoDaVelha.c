#include <stdio.h>
#include <unistd.h>
#include "telag.h"

#define CELL_SIZE 100 //tam de cada célula 100²

char ganhaLinha(int l, char m[3][3]){
	if (m[l][0] == m[l][1] && m[l][1] ==  m[l][2]){  //coordenadas linha variando a coluna
		return m[l][0];
	}
	return ' ';
}

char ganhaColuna(int c, char m[3][3]){
	if (m[0][c] == m[1][c] && m[1][c] == m[2][c]){ //coordenadas coluna variando a linha
		return m[0][c];
	}
	return ' ';
}

char ganhaDiagonal(char m[3][3]){
	if (m[0][0] == m[1][1] && m[1][1] == m[2][2]){ //coordenadas diagonal principal
		//diagonal principal
		return m[0][0];
	} else if(m[2][0] == m[1][1] && m[1][1] == m[0][2]){ //coordenadas diagonal secundaria
		//diagonal secundária
		return m[2][0];
	}
	return ' ';
}

char ganhador(char m[3][3]){
	char g = ' ';
	for (int i = 0; i < 3; i++){ //testa se algum jogador ganhou aluma linha ou coluna
		g = ganhaLinha(i,m);
		if (g != ' ') return g;
		g = ganhaColuna(i,m);
		if (g != ' ') return g;
	}
	g = ganhaDiagonal(m); //testa as diagonais
	if (g != ' ') return g;
	return ' ';
}

int matrizPreenchida(char m[3][3], char jogadorAtual){
	int cont = 0, lAux, cAux;
	for (int l = 0; l < 3; l++){
		for (int c = 0; c < 3; c++){
			if(m[l][c] == ' '){
				cont++;
				lAux = l;
				cAux = c;
			}
		}
	}

	if (cont == 1){ //se só tiver uma posição vazia
		m[lAux][cAux] = jogadorAtual;
		char g = ganhador(m);
		if(g == jogadorAtual){ //se o jogadorAtual vai ganhar nessa última rodada
			m[lAux][cAux] = ' '; //deixa o jogador finalizar a partida
			return 0;
		}else{ //se o jogador não foi ganhar, então já deu velha
			//alteração de estado independente de entrada
			return 1;
		}
	}else if (cont > 0){
		return 0; //retorna 0 se mais de uma posição estiver vazia
	} else{
		return 1; //retorna 1 se todas posições estiverem preenchidas
	}
}

void desenharTabuleiro(char t[3][3], int linAtual, int colAtual, char jogadorAtual){
	char mensagem[30];
	sprintf(mensagem, "JOGADOR ATUAL: %c", jogadorAtual);
	tela_texto(110, 330, 20, branco, mensagem);

	for(int l = 0; l < 3; l ++){
		for(int c = 0; c < 3; c++){
			int x = c * CELL_SIZE + CELL_SIZE / 2; //coordenadas, recebe a coluna e aí divide no meio pra centralizar o X ou O
			int y = l * CELL_SIZE + CELL_SIZE / 2; //coordenadas
			if(l == linAtual && c == colAtual){
                		tela_retangulo(x - CELL_SIZE / 2, y - CELL_SIZE / 2, x + CELL_SIZE / 2, y + CELL_SIZE / 2, 2, preto, vermelho); //posição selecionada vai receber um quadrado vermelho
            		} else {
                		tela_retangulo(x - CELL_SIZE / 2, y - CELL_SIZE / 2, x + CELL_SIZE / 2, y + CELL_SIZE / 2, 2, preto, branco); //restante das posições recebem um quadrado branco
			}
            		if (t[l][c] != ' '){ //garante que a posição não ta vazia
				char texto[2] = {t[l][c], '\0'}; //variável texto pra garantir que só teria um caracter(ja que tava dando erro, não entendi porque)
				tela_texto(x, y, 30, preto, texto); //na hora de passar a matriz direto, tava sendo concatenado o jogador (talvez por questão de pegar lixo da memória? nao sei)
            		}
        	}
	}
	tela_atualiza();
}

int main(){
	tela_inicio(300, 360, "Jogo da Velha"); //inicia a tela gráfica
	//inicialização das variáveis
    	char m[3][3] = {
        	{' ', ' ', ' '},
        	{' ', ' ', ' '},
        	{' ', ' ', ' '}
    	}; //cria a matriz do jogo e preenche com ' ' vazio

    	char jogadorAtual = 'X'; //jogo começa com X
    	char g; //variável para armazenar o ganhador
    	int linha = 0, coluna = 0;
    	int preenchida = matrizPreenchida(m, jogadorAtual); //variável de controle do loop


	tela_texto(150, 50, 20, branco, "Bem-vindo ao Jogo da Velha!");
	tela_atualiza();
	sleep(2);
    	//loop do jogo/principal
	while (preenchida == 0){ //enquanto a matriz não ta preenchida
		desenharTabuleiro(m, linha, coluna, jogadorAtual); //desenho da tela a partir do estado

		//entrada e processamento -  teclas para controlar
		//alteração do estado à partir das entradas
		char c = tela_tecla();
		if ((c == 's' || c == 'S') && linha < 2) linha++; //se não ta na última linha, vai pra baixo
		if ((c == 'w' || c == 'W') && linha > 0) linha--; //se não ta na primeira linha, vai pra cima
		if ((c == 'a' || c == 'A') && coluna > 0) coluna--; //se não ta na primeira coluna, vai pra esquerda
		if ((c == 'd' || c == 'D')  && coluna < 2) coluna++; //se não ta na última coluna, vai pra direita

        	//se apertar enter, é porque marcou a opção selecionada
        	if (c == '\n'){
        		if (m[linha][coluna] == ' '){   //confere se a posição ta vazia
               			m[linha][coluna] = jogadorAtual;  //insere na matriz
                		g = ganhador(m); //processamento independente de entrada
                		if (g != ' '){   //confere se tem ganhador
					//alteração do estado independente de entrada
                			desenharTabuleiro(m, linha, coluna, jogadorAtual); //desenho da tela a partir do estado
					char mensagem[30];
					sprintf(mensagem, "O jogador %c ganhou!", g);
					tela_texto(150, 50, 20, branco, mensagem);
                    			tela_atualiza();
                    			sleep(2); //pausa pro jogador ver
                    			//return 0;
					tela_fim(); //finalização do programa caso haja ganhador
                		}

                		//alterar jogadorAtual
                		if (jogadorAtual == 'X'){
                 			jogadorAtual = 'O';
                		} else if (jogadorAtual == 'O'){
					jogadorAtual = 'X';
				}

			}else{ //se posição nao ta vazia
                		tela_texto(150, 50, 20, branco, "Posição já foi marcada.");
                		tela_atualiza();
                		sleep(1); //pausa pro jogador conseguir ver a msg antes de limpar a tela
            		}
        	}
		//alteração do estado independente de entrada
        	preenchida = matrizPreenchida(m, jogadorAtual); //testa se a matriz ta preenchida, se não segue no while
	}

    	//tabuleiro preenchido, não entrou nenhuma vez no if de ganhador, então o jogo deu velha
	desenharTabuleiro(m, linha, coluna, jogadorAtual); //desenho da tela a partir do estado
	sleep(2);
	tela_texto(150, 50, 20, branco, "O jogo deu velha!");
	tela_atualiza();
	sleep(2);

	tela_fim(); //finalização do programa caso o jogo dê velha
}
