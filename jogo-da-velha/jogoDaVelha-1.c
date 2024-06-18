#include <stdio.h>
#include <unistd.h>
#include "tela.h"
#include "tecla.h"

#define LIN_MAX 24
#define COL_MAX 79

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

int matrizPreenchida(char m[3][3]){
	for (int l = 0; l < 3; l++){
		for (int c = 0; c < 3; c++){
			if(m[l][c] == ' ') return 0; //retorna 0 se alguma posição estiver vazia
		}
	}
	return 1; //retorna 1 se todas posições estiverem preenchidas
}

void tabuleiro(char t[3][3], int linAtual, int colAtual){
	for(int l = 0; l < 3; l ++){
		for(int c = 0; c < 3; c++){
			if(l == linAtual && c == colAtual){
				printf("'%c'", t[l][c]);
			} else {
				printf(" %c ", t[l][c]);
			}
			if (c < 2) printf("|");
		}
		printf("\n");
		if (l < 2) printf("---|---|---\n");
	}
}

int main(){
	char m[3][3] = {
		{' ', ' ', ' '},
		{' ', ' ', ' '},
		{' ', ' ', ' '}
	}; //preenche a matriz com ' ' vazio
	char jogadorAtual = 'X'; //jogo começa com X
	char g;
	int linha = 0, coluna = 0;
	int preenchida = matrizPreenchida(m);

	tela_ini(); //inicia tela e tecla
	tecla_ini();
	//loop do jogo
	while (preenchida == 0){ //enquanto a matriz não ta preenchida
		tela_limpa();
		tela_lincol(0,0); //posiciona o cursor no canto superior esquerdo
		tabuleiro(m, linha, coluna);
		tela_atualiza();
		tela_mostra_cursor(false);
		//teclas para controlar o cursor
		char c = tecla_le_char();
		if (c == 's') linha++;
		if (c == 'w') linha--;
		if (c == 'a') coluna--;
		if (c == 'd') coluna++;
		//se apertar enter, é porque marcou a opção selecionada
		if (c == '\n'){
			if (m[linha][coluna] == ' '){ 	//confere se a posição ta vazia
				m[linha][coluna] = jogadorAtual;  //insere na matriz
				g = ganhador(m);
				if (g != ' '){   //confere se tem ganhador
					tela_limpa();
					tabuleiro(m, linha, coluna); //atualiza tabuleiro
					printf("O jogador %c ganhou\n", g);
					tela_atualiza();
					sleep(3); //pausa pro jogador ver
					tecla_fim(); //fecha as telinha e tecla bonitinho
					tela_fim();
					return 0;
				}
				//alterar jogadorAtual
				if (jogadorAtual == 'X'){
					jogadorAtual = 'O';
				} else if (jogadorAtual == 'O'){
					 jogadorAtual = 'X';
				}
			} else{ //se posição nao ta vazia
				printf("Essa posição já foi marcada, jogue novamente! \n");
				tela_atualiza();
				sleep(1); //pausa pro jogador conseguir ver a msg antes de limpar a tela
			}
		}
		preenchida = matrizPreenchida(m); //testa se a matriz ta preenchida, se não segue no while
	}
	//tabuleiro preenchido, não entrou nenhuma vez no if de ganhador, então o jogo deu velha
	tela_limpa();
	tela_lincol(0,0);
	tabuleiro(m, linha, coluna); //atualiza tabuleiro
	//if(g = ' '){ //teste de garantia
		printf("O jogo deu velha! \n");
	//}
	tela_atualiza();
	sleep(3);
	tecla_fim();//fecha as telinha e tecla
	tela_fim();
}
