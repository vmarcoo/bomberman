/**
    ##################################
    #                                #
    #      AUTORES DO PROJETO:       #
    #                                #
    # MARCO VINICIUS DA COSTA BUSATO #
    #      FELYPE CESAR MOLINARI     #
    #     LAURA FERRARI HECKMANN     #
    #                                #
    ##################################
*/

#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <limits>
#include <cmath>
#include <chrono>
#include <thread>
#include <iomanip>
#include <fstream>
#include <vector>
#include "funcoes_menu.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

///Coordenadas do personagem e da bomba
int xPersonagem1 = 2, yPersonagem1 = 2;
int xPersonagem2, yPersonagem2;
int xBomba1, yBomba1;
int xBomba2, yBomba2;
int xPortal = 8, yPortal = 20;

///Controle de movimento aleatório dos inimigos
int deslocamentos;
int direcao;

///Controle de tempo entre movimentação de inimigos e explosão de bomba.
int timerInimigo = 0;
int timerBomba1 = 0;
int timerBomba2 = 0;
int timerSobreviveBomba1 = 0;
int timerSobreviveBomba2 = 0;
bool timerBombaRelogio1 = false;
bool explodeBombaRelogio = false;
bool resetTimerVidaExtra = false;
int contaPassos1 = 0;
int contaPassos2 = 0;
int contaBombas1 = 0;
int contaBombas2 = 0;

///Controla quanto tempo o caractere de explosão fica em tela
int explosaoBomba1 = 0;
int explosaoBomba2 = 0;

///Controles gerais de execução do jogo
bool personagem1Vivo = true;
bool personagem2Vivo = true;
bool gameLoop = true;
bool bomba1EmJogo = false;
bool bomba2EmJogo = false;
bool tocaMusica = true;
bool repeteMenuVidaExtra = false;

bool morte1 = false;
bool morte2 = false;
int inimigosMortos;

///Variavel para tecla pressionada
char tecla;

///Matriz que controla a impressão do mapa
///0: Caminho livre
///1: Parede sólida
///2: Parede frágil
///3 e 6: Caracteres da bomba (pois ela pisca)
///4: Caractere de explosão da bomba
///5: Inimigos

int m[10][22] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

int mReserva[10][22] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                       0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                       0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                       0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                       0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                       0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                       0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
                       0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                       0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

///Estrutura do inimigo
struct Inimigo{
    int xInimigo;
    int yInimigo;
};

struct Configuracoes{
    int numeroDeJogadores;
    int numeroDeInimigos;
    int dificuldade;
    int faseLoop = 1;
    string nomeJogador1;
    string nomeJogador2;
};

struct Poderes{
    int x;
    int y;
    bool emUso = false;
    int poderPersonagem1 = 0;
    int poderPersonagem2 = 0;
};

Poderes vidaExtra;
Poderes bombaExtra;
Poderes raioExplosao;
Poderes bombaRelogio;
Poderes sobreviveBomba;
Poderes atravessaCx;
Poderes explodeMais;

struct Pontuacao {
    string data;
    string nome;
    int dificuldade;
    double passos;
    double bombas;
    double pontuacao;
    double ranking;
};

///Configuração do jogo
Configuracoes configuracaoAtual;

///Ponteiro para armazenar o inimigo escolhido em questão
Inimigo* inimigoEscolhido;

///As 4 próximas funções são booleanos que controlam a possibilidade de movimentação do personagem
bool cima(int x, int y, bool personagemVivo){

    if(atravessaCx.poderPersonagem1 == 1 && (x == xPersonagem1) && (y == yPersonagem1)){
        if(m[x-1][y] == 2){return true;}
    }

    if(atravessaCx.poderPersonagem2 == 1 && (x == xPersonagem2) && (y == yPersonagem2)){
        if(m[x-1][y] == 2){return true;}
    }

    if(m[x-1][y] == 1 || m[x-1][y] == 2 || m[x-1][y] == 3 || m[x-1][y] == 6 || (x == xPersonagem1 && x-1 == xPersonagem2 && yPersonagem1 == yPersonagem2)||(x == xPersonagem2 && x-1 == xPersonagem1 && yPersonagem1 == yPersonagem2) || personagemVivo == false){
        return false;
    }
    else{
        return true;
    }
}

bool baixo(int x, int y, bool personagemVivo){

    if(atravessaCx.poderPersonagem1 == 1 && (x == xPersonagem1) && (y == yPersonagem1)){
        if(m[x+1][y] == 2){return true;}
    }

    if(atravessaCx.poderPersonagem2 == 1 && (x == xPersonagem2) && (y == yPersonagem2)){
        if(m[x+1][y] == 2){return true;}
    }

    if(m[x+1][y] == 1 || m[x+1][y] == 2 || m[x+1][y] == 3 || m[x+1][y] == 6 || (x == xPersonagem1 && x+1 == xPersonagem2 && yPersonagem1 == yPersonagem2)||(x == xPersonagem2 && x+1 == xPersonagem1 && yPersonagem1 == yPersonagem2) || personagemVivo == false){
        return false;
    }
    else{
        return true;
    }
}

bool esquerda(int x, int y, bool personagemVivo){

    if(atravessaCx.poderPersonagem1 == 1 && (x == xPersonagem1) && (y == yPersonagem1)){
        if(m[x][y-1] == 2){return true;}
    }

    if(atravessaCx.poderPersonagem2 == 1 && (x == xPersonagem2) && (y == yPersonagem2)){
        if(m[x][y-1] == 2){return true;}
    }

    if(m[x][y-1] == 1 || m[x][y-1] == 2 || m[x][y-1] == 3 || m[x][y-1] == 6 || (y == yPersonagem1 && y-1 == yPersonagem2 && xPersonagem1 == xPersonagem2)||(y == yPersonagem2 && y-1 == yPersonagem1 && xPersonagem1 == xPersonagem2) || personagemVivo == false){
        return false;
    }
    else{
        return true;
    }
}

bool direita(int x, int y, bool personagemVivo){

    if(atravessaCx.poderPersonagem1 == 1 && (x == xPersonagem1) && (y == yPersonagem1)){
        if(m[x][y+1] == 2){return true;}
    }

    if(atravessaCx.poderPersonagem2 == 1 && (x == xPersonagem2) && (y == yPersonagem2)){
        if(m[x][y+1] == 2){return true;}
    }

    if(m[x][y+1] == 1 || m[x][y+1] == 2 || m[x][y+1] == 3 || m[x][y+1] == 6 || (y == yPersonagem1 && y+1 == yPersonagem2 && xPersonagem1 == xPersonagem2)||(y == yPersonagem2 && y+1 == yPersonagem1 && xPersonagem1 == xPersonagem2) || personagemVivo == false){
        return false;
    }
    else{
        return true;
    }
}

///As 4 próximas funções são booleanos que controlam a possibilidade de movimentação dos inimigos
bool cima(Inimigo &inimigoEscolhido){

   if(m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 1 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 2 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 3 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 4 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 5 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 6 ||
      ///As coordenadas -1 indicam que o inimigo já morreu, uma vez que não existe essa coordenada na matriz.
      (inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1)){
        return false;
    }
    else{
        return true;
    }
}

bool baixo(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 1 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 2 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 3 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 4 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 5 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 6 ||
       (inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1)){
        return false;
    }
    else{
        return true;
    }
}

bool esquerda(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 1 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 2 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 3 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 4 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 5 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 6 ||
       (inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1)){
        return false;
    }
    else{
        return true;
    }
}

bool direita(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 1 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 2 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 3 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 4 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 5 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 6 ||
       (inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1)){
        return false;
    }
    else{
        return true;
    }
}

///As 4 próximas funções controlam o deslocamento do inimigo em si
void inimigoDeslocaCima(Inimigo &inimigoEscolhido){

    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 0;
    inimigoEscolhido.xInimigo--;
    ///Caso a nova coordenada seja um local de explosão de bomba, o inimigo morre, recebendo então coordenadas de -1 para x e y.
    if (m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] == 4){inimigoEscolhido.xInimigo = -1; inimigoEscolhido.yInimigo = -1;}
    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 5;
}

void inimigoDeslocaBaixo(Inimigo &inimigoEscolhido){

    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 0;
    inimigoEscolhido.xInimigo++;
    if (m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] == 4){inimigoEscolhido.xInimigo = -1; inimigoEscolhido.yInimigo = -1;}
    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 5;
}

void inimigoDeslocaEsquerda(Inimigo &inimigoEscolhido){

    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 0;
    inimigoEscolhido.yInimigo--;
    if (m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] == 4){inimigoEscolhido.xInimigo = -1; inimigoEscolhido.yInimigo = -1;}
    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 5;
}

void inimigoDeslocaDireita(Inimigo &inimigoEscolhido){

    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 0;
    inimigoEscolhido.yInimigo++;
    if (m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] == 4){inimigoEscolhido.xInimigo = -1; inimigoEscolhido.yInimigo = -1;}
    m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo] = 5;
}

///Função booleana que controla a possibilidade de plantar a bomba
bool plantaBomba(int x, int y){

    if (x == xBomba1 && y == yBomba1){

        if (bombaRelogio.poderPersonagem1 == 1){
            timerBombaRelogio1 = true;
            bombaRelogio.poderPersonagem1 = 0;
            explodeBombaRelogio = true;
        }

        if (bomba1EmJogo == true){
            if (timerBombaRelogio1 == true){
                timerBombaRelogio1 = false;
            }
            return false;
        }
    }

    if (x == xBomba2 && y == yBomba2){

        if (bomba2EmJogo == true){
            return false;
        }
    }

    return true;
}

///Função que controla a explosão da bomba em si
void explodeBomba(int x, int y, Inimigo inimigos[]){

    if (raioExplosao.poderPersonagem1 == 1 && (x == xBomba1 && y == yBomba1)){

        if (explodeMais.poderPersonagem1 == 1){
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem1--;
        }

        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    if (raioExplosao.poderPersonagem1 == 2 && (x == xBomba1 && y == yBomba1)){

        if (explodeMais.poderPersonagem1 == 1){
            if ((m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-3][y] = 4;}
            if ((m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+3][y] = 4;}
            if ((m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-3] = 4;}
            if ((m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+3] = 4;}
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem1--;
        }

        if ((m[x-3][y] != 1) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-3][y] = 4;}
        if ((m[x+3][y] != 1) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+3][y] = 4;}
        if ((m[x][y-3] != 1) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-3] = 4;}
        if ((m[x][y+3] != 1) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+3] = 4;}
        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2) ||
               (inimigos[i].xInimigo == x-3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-3) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+3))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    if (raioExplosao.poderPersonagem1 == 3 && (x == xBomba1 && y == yBomba1)){

        if (explodeMais.poderPersonagem1 == 1){
            if ((m[x-4][y] != 1) && (m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-4][y] = 4;}
            if ((m[x+4][y] != 1) && (m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+4][y] = 4;}
            if ((m[x][y-4] != 1) && (m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-4] = 4;}
            if ((m[x][y+4] != 1) && (m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+4] = 4;}
            if ((m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-3][y] = 4;}
            if ((m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+3][y] = 4;}
            if ((m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-3] = 4;}
            if ((m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+3] = 4;}
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem1--;
        }

        if ((m[x-4][y] != 1) && (m[x-3][y] == 0) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-4][y] = 4;}
        if ((m[x+4][y] != 1) && (m[x+3][y] == 0) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+4][y] = 4;}
        if ((m[x][y-4] != 1) && (m[x][y-3] == 0) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-4] = 4;}
        if ((m[x][y+4] != 1) && (m[x][y+3] == 0) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+4] = 4;}
        if ((m[x-3][y] != 1) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-3][y] = 4;}
        if ((m[x+3][y] != 1) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+3][y] = 4;}
        if ((m[x][y-3] != 1) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-3] = 4;}
        if ((m[x][y+3] != 1) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+3] = 4;}
        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2) ||
               (inimigos[i].xInimigo == x-3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-3) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+3) ||
               (inimigos[i].xInimigo == x-4 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+4 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-4) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+4))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    if (raioExplosao.poderPersonagem2 == 1 && (x == xBomba2 && y == yBomba2)){

        if (explodeMais.poderPersonagem2 == 1){
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem2--;
        }

        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    if (raioExplosao.poderPersonagem2 == 2 && (x == xBomba2 && y == yBomba2)){

        if (explodeMais.poderPersonagem2 == 1){
            if ((m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-3][y] = 4;}
            if ((m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+3][y] = 4;}
            if ((m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-3] = 4;}
            if ((m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+3] = 4;}
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem2--;
        }

        if ((m[x-3][y] != 1) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-3][y] = 4;}
        if ((m[x+3][y] != 1) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+3][y] = 4;}
        if ((m[x][y-3] != 1) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-3] = 4;}
        if ((m[x][y+3] != 1) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+3] = 4;}
        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2) ||
               (inimigos[i].xInimigo == x-3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-3) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+3))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    if (raioExplosao.poderPersonagem2 == 3 && (x == xBomba2 && y == yBomba2)){

        if (explodeMais.poderPersonagem1 == 1){
            if ((m[x-4][y] != 1) && (m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-4][y] = 4;}
            if ((m[x+4][y] != 1) && (m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+4][y] = 4;}
            if ((m[x][y-4] != 1) && (m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-4] = 4;}
            if ((m[x][y+4] != 1) && (m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+4] = 4;}
            if ((m[x-3][y] != 1) && (m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-3][y] = 4;}
            if ((m[x+3][y] != 1) && (m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+3][y] = 4;}
            if ((m[x][y-3] != 1) && (m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-3] = 4;}
            if ((m[x][y+3] != 1) && (m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+3] = 4;}
            if ((m[x-2][y] != 1) && (m[x-1][y] != 1)){m[x-2][y] = 4;}
            if ((m[x+2][y] != 1) && (m[x+1][y] != 1)){m[x+2][y] = 4;}
            if ((m[x][y-2] != 1) && (m[x][y-1] != 1)){m[x][y-2] = 4;}
            if ((m[x][y+2] != 1) && (m[x][y+1] != 1)){m[x][y+2] = 4;}
            explodeMais.poderPersonagem1--;
        }

        if ((m[x-4][y] != 1) && (m[x-3][y] == 0) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-4][y] = 4;}
        if ((m[x+4][y] != 1) && (m[x+3][y] == 0) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+4][y] = 4;}
        if ((m[x][y-4] != 1) && (m[x][y-3] == 0) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-4] = 4;}
        if ((m[x][y+4] != 1) && (m[x][y+3] == 0) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+4] = 4;}
        if ((m[x-3][y] != 1) && (m[x-2][y] == 0) && (m[x-1][y] == 0)){m[x-3][y] = 4;}
        if ((m[x+3][y] != 1) && (m[x+2][y] == 0) && (m[x+1][y] == 0)){m[x+3][y] = 4;}
        if ((m[x][y-3] != 1) && (m[x][y-2] == 0) && (m[x][y-1] == 0)){m[x][y-3] = 4;}
        if ((m[x][y+3] != 1) && (m[x][y+2] == 0) && (m[x][y+1] == 0)){m[x][y+3] = 4;}
        if ((m[x-2][y] != 1) && (m[x-1][y] == 0)){m[x-2][y] = 4;}
        if ((m[x+2][y] != 1) && (m[x+1][y] == 0)){m[x+2][y] = 4;}
        if ((m[x][y-2] != 1) && (m[x][y-1] == 0)){m[x][y-2] = 4;}
        if ((m[x][y+2] != 1) && (m[x][y+1] == 0)){m[x][y+2] = 4;}

        for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
            if((inimigos[i].xInimigo == x-2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+2 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-2) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+2) ||
               (inimigos[i].xInimigo == x-3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+3 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-3) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+3) ||
               (inimigos[i].xInimigo == x-4 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x+4 && inimigos[i].yInimigo == y) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-4) ||
               (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+4))
               {
                  m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
                  inimigos[i].xInimigo = -1;
                  inimigos[i].yInimigo = -1;
                }
        }
    }

    ///Explosão das posições adjacentes à bomba, destruindo paredes frágeis
    if (m[x-1][y] == 2 || m[x-1][y] == 0){m[x-1][y] = 4;}
    if (m[x+1][y] == 2 || m[x+1][y] == 0){m[x+1][y] = 4;}
    if (m[x][y-1] == 2 || m[x][y-1] == 0){m[x][y-1] = 4;}
    if (m[x][y+1] == 2 || m[x][y+1] == 0){m[x][y+1] = 4;}

    ///Algum inimigo em posição adjacente à bomba (morte do inimigo em questão)
    for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
        if((inimigos[i].xInimigo == x-1 && inimigos[i].yInimigo == y) ||
           (inimigos[i].xInimigo == x+1 && inimigos[i].yInimigo == y) ||
           (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y-1) ||
           (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y+1) ||
           (inimigos[i].xInimigo == x && inimigos[i].yInimigo == y))
           {
              m[inimigos[i].xInimigo][inimigos[i].yInimigo] = 4;
              inimigos[i].xInimigo = -1;
              inimigos[i].yInimigo = -1;
           }
    }

    if (x == xBomba1 && y == yBomba1){
        m[xBomba1][yBomba1] = 4;
        ///Coordenadas da bomba recebem -1, para que não seja impressa no mapa
        xBomba1 = -1;
        yBomba1 = -1;
        bomba1EmJogo = false;
        explosaoBomba1 = 1;
    }

    if (x == xBomba2 && y == yBomba2){
        m[xBomba2][yBomba2] = 4;
        ///Coordenadas da bomba recebem -1, para que não seja impressa no mapa
        xBomba2 = -1;
        yBomba2 = -1;
        bomba2EmJogo = false;
        explosaoBomba2 = 1;
    }
}

bool imprimeInimigo(int linha, int coluna, int posicaoValida = 9){

    if (linha == 2 && coluna == 2){posicaoValida--;}
    else if (linha == 2 && coluna == 3){posicaoValida--;}
    else if (linha == 3 && coluna == 2){posicaoValida--;}
    else if (linha == 8 && coluna == 20){posicaoValida--;}
    else if (linha == 8 && coluna == 19){posicaoValida--;}
    else if (linha == 7 && coluna == 20){posicaoValida--;}
    else if (m[linha][coluna] != 0){posicaoValida--;}
    else if (m[linha-1][coluna] != 0 && m[linha+1][coluna] != 0 && m[linha][coluna-1] != 0 && m[linha][coluna+1] != 0){posicaoValida--;}
    else if (m[linha-1][coluna] == 5 || m[linha+1][coluna] == 5 || m[linha][coluna-1] == 5 || m[linha][coluna+1] == 5){posicaoValida--;}

    if (posicaoValida == 9){return true;}

    return false;

}

Inimigo selecionaInimigo(Inimigo maisProximoLinha, Inimigo maisProximoColuna, Inimigo maisProximo, Inimigo inimigos[], int xPersonagem1, int iteracoes){

    for (int i = 0; i < iteracoes; i++){
        if (inimigos[i].xInimigo == -1 || inimigos[i].yInimigo == -1){continue;}

        maisProximoLinha = inimigos[i];
        maisProximoColuna = inimigos[i];
        maisProximo = inimigos[i];
        break;
    }

    for (int i = 0; i < iteracoes; i++){
        if (inimigos[i].xInimigo == -1 || inimigos[i].yInimigo == -1){continue;}

        if (abs(inimigos[i].xInimigo - xPersonagem1) < abs(maisProximoLinha.xInimigo - xPersonagem1)){
            maisProximoLinha = inimigos[i];
        }
    }

    for (int i = 0; i < iteracoes; i++){
        if (inimigos[i].xInimigo == -1 || inimigos[i].yInimigo == -1){continue;}

        if (abs(inimigos[i].yInimigo - yPersonagem1) < abs(maisProximoColuna.yInimigo - yPersonagem1)){
            maisProximoColuna = inimigos[i];
        }
    }

    for (int i = 0; i < iteracoes; i++){
        if (inimigos[i].xInimigo == -1 || inimigos[i].yInimigo == -1){continue;}

        if (abs(maisProximoLinha.yInimigo - yPersonagem1) < abs(maisProximoColuna.xInimigo - xPersonagem1)){
            maisProximo = maisProximoLinha;
        }
    }

    for (int i = 0; i < iteracoes; i++){
        if (inimigos[i].xInimigo == -1 || inimigos[i].yInimigo == -1){continue;}

        if (abs(maisProximoLinha.yInimigo - yPersonagem1) > abs(maisProximoColuna.xInimigo - xPersonagem1)){
            maisProximo = maisProximoColuna;
        }
    }

    return (maisProximo);
}

///Impressão do menu inicial
void imprimeMenu(){

    char x = char(205);
    char y = char(206);
    int opcao;
    bool opcaoValida;

    do{
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
        cout<<"  Bem-vindo(a) ao Bomberman! Escolha a opcao desejada:\n\n";
        cout<<"                     1 - Iniciar\n\n";
        cout<<"                     2 - Como jogar\n\n";
        cout<<"                     3 - Estatisticas\n\n";
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
        cout << "                   Opcao escolhida: ";

        if (!(cin >> opcao) || cin.peek() != '\n' || (opcao != 1 && opcao != 2 && opcao != 3)){
            verificaCin();
            opcaoValida = false;
        }

        else{
            opcaoValida = true;
            clearScreen();
        }

    } while (!opcaoValida);

    if (opcao == 1){

        int opcaoJogadores;
        int dificuldade;
        bool opcaoJogadoresValida;
        bool dificuldadeValida;

        ///Escolha do numero de jogadores
        do{
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"           Escolha o numero de jogadores (1 ou 2):\n\n";
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                    Opcao escolhida: ";

            if (!(cin >> opcaoJogadores) || cin.peek() != '\n' || (opcaoJogadores != 1 && opcaoJogadores != 2)){
                verificaCin();
                opcaoJogadoresValida = false;
            }

            else{
                opcaoJogadoresValida = true;
                clearScreen();
            }

        } while (!opcaoJogadoresValida);

        string nomeJogadores[opcaoJogadores];
        for (int i = 0; i < opcaoJogadores; i++){
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"              Digite o nome do jogador "<< i+1 <<":\n\n";
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"              Nome: ";
            if(i == 0){cin.ignore();}
            getline(cin, nomeJogadores[i]);
            clearScreen();
        }

        ///Escolha da dificuldade
        do{
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                  Escolha a dificuldade: \n\n";
            cout<<"                       1 - Facil \n";
            cout<<"                       2 - Medio \n";
            cout<<"                       3 - Dificil \n\n";
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                     Opcao escolhida: ";

            if (!(cin >> dificuldade) || cin.peek() != '\n' || (dificuldade != 1 && dificuldade != 2 && dificuldade != 3)){
                verificaCin();
                dificuldadeValida = false;
            }

            else{
                dificuldadeValida = true;
                clearScreen();
            }

        } while (!dificuldadeValida);

        configuracaoAtual.numeroDeJogadores = opcaoJogadores;
        configuracaoAtual.dificuldade = dificuldade;

        if (dificuldade == 1){configuracaoAtual.numeroDeInimigos = 3;}
        else if (dificuldade == 2){configuracaoAtual.numeroDeInimigos = 5;}
        else if (dificuldade == 3){configuracaoAtual.numeroDeInimigos = 7;}

        if(opcaoJogadores == 1){
            configuracaoAtual.nomeJogador1 = nomeJogadores[0];
        }
        else{
            configuracaoAtual.nomeJogador1 = nomeJogadores[0];
            configuracaoAtual.nomeJogador2 = nomeJogadores[1];
        }

        if (opcaoJogadores == 1){configuracaoAtual.nomeJogador2 = "---";}
    }

    if (opcao == 2){
        menuInstrucoes();
        posicionaCursorNoTopo();
        getch();
        clearScreen();
        imprimeMenu();
    }

    if (opcao == 3){
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
        cout<<"                      ESTATISTICAS:\n" << endl;
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
        ifstream file("estatisticas.txt");
        if (!file) {cout << " " << endl;}
        else{
            ifstream estatisticas;
            estatisticas.open("estatisticas.txt");
            string frase;
            while (estatisticas.eof() == false){
                getline(estatisticas, frase);
                cout << frase;
                cout << endl;
            }
            estatisticas.close();
        }
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
        cout << "   Pressione qualquer tecla para retornar ao menu...\n\n";
        cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x;
        posicionaCursorNoTopo();
        getch();
        clearScreen();
        imprimeMenu();
    }
}

void geracaoInimigos(Inimigo inimigos[], Configuracoes configuracaoAtual)
{
	if (configuracaoAtual.dificuldade == 1){
		for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
			bool inimigoGerado = false;
			do{
				int linhaAleatoria = (rand() % 8) + 2;
				int colunaAleatoria = (rand() % 20) + 2;

				if(imprimeInimigo(linhaAleatoria, colunaAleatoria) == true){
					m[linhaAleatoria][colunaAleatoria] = 5;
					inimigos[i] = {linhaAleatoria, colunaAleatoria};
					inimigoGerado = true;
				}

				else {inimigoGerado = false;}

			}while(inimigoGerado == false);
		}
	}

	else if(configuracaoAtual.dificuldade == 2){
		for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
			bool inimigoGerado = false;
			do{
				int linhaAleatoria = (rand() % 8) + 2;
				int colunaAleatoria = (rand() % 20) + 2;

				if(imprimeInimigo(linhaAleatoria, colunaAleatoria) == true){
					m[linhaAleatoria][colunaAleatoria] = 5;
					inimigos[i] = {linhaAleatoria, colunaAleatoria};
					inimigoGerado = true;
				}

				else {inimigoGerado = false;}

			}while(inimigoGerado == false);
		}
	}

	else if(configuracaoAtual.dificuldade == 3){
		for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
			bool inimigoGerado = false;
			do{
				int linhaAleatoria = (rand() % 8) + 2;
				int colunaAleatoria = (rand() % 20) + 2;

				if(imprimeInimigo(linhaAleatoria, colunaAleatoria) == true){
					m[linhaAleatoria][colunaAleatoria] = 5;
					inimigos[i] = {linhaAleatoria, colunaAleatoria};
					inimigoGerado = true;
				}

				else {inimigoGerado = false;}

			}while(inimigoGerado == false);
		}
	}
}

void telaCausaDaMorte()
{
    if (configuracaoAtual.numeroDeJogadores == 1){
        if (morte1 == true){
            clearScreen();
            cout << "\n\n\n\n\n\n";
            cout << "                      GAME OVER!" << endl;
            cout << "\n\n\n";
            posicionaCursorNoTopo();
            getch();
            clearScreen();
        }

        if (morte1 == false) {
            clearScreen();
            cout << "\n\n\n\n\n\n";
            cout << "                       VITORIA!" << endl;
            cout << "\n\n\n";
            posicionaCursorNoTopo();
            getch();
            clearScreen();
        }
    }

    if (configuracaoAtual.numeroDeJogadores == 2){
        if (morte1 == true && morte2 == true){
            clearScreen();
            cout << "\n\n\n\n\n\n";
            cout << "                      GAME OVER!" << endl;
            cout << "\n\n\n";
            posicionaCursorNoTopo();
            getch();
            clearScreen();
        }

        if (morte1 == false && morte2 == false) {
            clearScreen();
            cout << "\n\n\n\n\n\n";
            cout << "                       VITORIA!" << endl;
            cout << "\n\n\n";
            posicionaCursorNoTopo();
            getch();
            clearScreen();
        }
    }
}

void geracaoCaixotes()
{
	int caixotes = 0;
	srand(time(0));

	do {
		int linhaAleatoria = (rand() % 8) + 2;
		int colunaAleatoria = (rand() % 20) + 2;

		if ((linhaAleatoria == 2 && colunaAleatoria == 2) || (linhaAleatoria == 8 && colunaAleatoria == 20) ||
				(linhaAleatoria == 2 && colunaAleatoria == 3) || (linhaAleatoria == 7 && colunaAleatoria == 20) ||
				(linhaAleatoria == 3 && colunaAleatoria == 2) || (linhaAleatoria == 8 && colunaAleatoria == 19) ||
				m[linhaAleatoria][colunaAleatoria] == 1 || m[linhaAleatoria][colunaAleatoria] == 2){}

		else {
			m[linhaAleatoria][colunaAleatoria] = 2;
			if (caixotes == 0){
				vidaExtra.x = linhaAleatoria;
				vidaExtra.y = colunaAleatoria;
			}
			if (caixotes == 5){
				bombaExtra.x = linhaAleatoria;
				bombaExtra.y = colunaAleatoria;
			}
			if (caixotes == 10){
				raioExplosao.x = linhaAleatoria;
				raioExplosao.y = colunaAleatoria;
			}
			if (caixotes == 15){
				bombaRelogio.x = linhaAleatoria;
				bombaRelogio.y = colunaAleatoria;
			}
			if (caixotes == 20){
				sobreviveBomba.x = linhaAleatoria;
				sobreviveBomba.y = colunaAleatoria;
			}
			if (caixotes == 25){
				atravessaCx.x = linhaAleatoria;
				atravessaCx.y = colunaAleatoria;
			}
			if (caixotes == 30){
				explodeMais.x = linhaAleatoria;
				explodeMais.y = colunaAleatoria;
			}
			caixotes++;
		}

	}while(caixotes < 40);
}

void telaFinalJogo()
{
    PlaySound(NULL, 0, 0);
    tocaMusica = true;
    char x = char(205);
        char y = char(206);
        int respEscolha;
        int respEscolhaValida;

        do{
            clearScreen();
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                Escolha a opcao desejada: \n\n";
            cout<<"               1 - Ir para o menu inicial \n";
            cout<<"               2 - Fechar jogo \n\n";
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                    Opcao escolhida: ";

            if (!(cin >> respEscolha) || cin.peek() != '\n' || (respEscolha != 1 && respEscolha != 2)){
                verificaCin();
                respEscolhaValida = false;
            }

            else {
                respEscolhaValida = true;
                clearScreen();
            }

            if(respEscolha == 1){
                for (int i = 0; i < 10; i++){
                    for (int j = 0; j < 22; j++){
                        m[i][j] = mReserva[i][j];
                    }
                }

                personagem1Vivo = true;
                personagem2Vivo = true;
                vidaExtra.poderPersonagem1 = 0;
                vidaExtra.poderPersonagem2 = 0;
                bombaExtra.poderPersonagem1 = 0;
                bombaExtra.poderPersonagem2 = 0;
                raioExplosao.poderPersonagem1 = 0;
                raioExplosao.poderPersonagem2 = 0;
                bombaRelogio.poderPersonagem1 = 0;
                bombaRelogio.poderPersonagem2 = 0;
                explodeMais.poderPersonagem1 = 0;
                explodeMais.poderPersonagem2 = 0;
                atravessaCx.poderPersonagem1 = 0;
                atravessaCx.poderPersonagem2 = 0;
                sobreviveBomba.poderPersonagem1 = 0;
                sobreviveBomba.poderPersonagem2 = 0;
                configuracaoAtual.faseLoop = 1;
                contaPassos1 = 0;
                contaPassos2 = 0;
                contaBombas1 = 0;
                contaBombas2 = 0;
                morte1 = false;
                morte2 = false;
                gameLoop = false;
            }

            ///Sai do jogo
            if(respEscolha == 2){
                exit(0);
            }

        } while(!respEscolhaValida);
}

void telaFinalFase()
{
    char x = char(205);
    char y = char(206);
    int respEscolha;
    int respEscolhaValida;

    do{
            clearScreen();
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"    Voce possui vida extra. Deseja continuar jogando? \n\n";
            cout<<"                       1 - Sim \n";
            cout<<"                       2 - Nao \n\n";
            cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
            cout<<"                    Opcao escolhida: ";

            if (!(cin >> respEscolha) || cin.peek() != '\n' || (respEscolha != 1 && respEscolha != 2)){
                verificaCin();
                respEscolhaValida = false;
            }

            else {
                respEscolhaValida = true;
                clearScreen();
            }

            if(respEscolha == 1){
                resetTimerVidaExtra = true;
            }

            if(respEscolha == 2){
                repeteMenuVidaExtra = true;
                telaFinalJogo();
            }

        } while(!respEscolhaValida);
}

template <typename T>
void criarArquivo(T dificuldade, T passos, T bombas) {

    ifstream infile("estatisticas.txt");
    infile.close();

    ofstream estatisticas("estatisticas.txt", ios::app);

    if (estatisticas.is_open()) {
        time_t currentTime = time(nullptr);
        tm* localTime = localtime(&currentTime);

        estatisticas
                     << " Data: "
                     << setw(2) << setfill('0') << localTime->tm_mday << "/"
                     << setw(2) << setfill('0') << localTime->tm_mon + 1 << "/"
                     << setw(2) << setfill('0') << localTime->tm_year + 1900
                     << " - "
                     << setw(2) << setfill('0') << localTime->tm_hour << ":"
                     << setw(2) << setfill('0') << localTime->tm_min << ":"
                     << setw(2) << setfill('0') << localTime->tm_sec << endl;
                     if (configuracaoAtual.numeroDeJogadores == 1){
        estatisticas << " Nome: " << configuracaoAtual.nomeJogador1 << endl;
                     }
                     if (configuracaoAtual.numeroDeJogadores == 2){
        estatisticas << " Nome: " << configuracaoAtual.nomeJogador1 + " e " + configuracaoAtual.nomeJogador2 << endl;
                     }
        estatisticas << " Passos Andados: " << passos << endl
                     << " Bombas Plantadas: " << bombas << endl
                     << " Pontuacao: " << (passos + bombas) << endl
                     << endl;
        estatisticas << "-------------------------------------------------------" << endl;
        estatisticas.close();
    }
}

int main()
{
    tocaMusica = true;
    while(true){
        gameLoop = true;
        repeteMenuVidaExtra = false;
        ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO A SEGUIR:
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        //INICIO: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
        ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO ACIMA.

        imprimeMenu();

        ///Matriz que controla a impressão do mapa
        ///0: Caminho livre
        ///1: Parede sólida
        ///2: Parede frágil
        ///3 e 6: Caracteres da bomba (pois ela pisca)
        ///4: Caractere de explosão da bomba
        ///5: Inimigos

        ///Geração de caixotes aleatórios

        geracaoCaixotes();

        ///Array de inimigos
        Inimigo inimigos[configuracaoAtual.numeroDeInimigos];

        if(configuracaoAtual.faseLoop == 1){
            geracaoInimigos(inimigos, configuracaoAtual);
        }

        if(configuracaoAtual.numeroDeJogadores == 2){
            xPersonagem2 = 8;
            yPersonagem2 = 20;
        }

        using clock = chrono::steady_clock;
        auto start = clock::now();
        auto end = start + chrono::minutes(3) + chrono::seconds(1);
        bool resetTimer = false;
        int faseAtual = configuracaoAtual.faseLoop;
        int previous_seconds = -1;

        while(gameLoop == true){

            if (tocaMusica == true){
                PlaySound(TEXT("musica.wav"), NULL, SND_FILENAME | SND_ASYNC);
                tocaMusica = false;
            }
            auto now = clock::now();

            if (resetTimer) {
                start = now;
                end = start + chrono::minutes(3) + chrono::seconds(1);
                resetTimer = false;
            }

            auto remaining_time = chrono::duration_cast<chrono::seconds>(end - now).count();

            auto inicioMusica = now;
            auto fimMusica = inicioMusica + chrono::minutes(4) + chrono::seconds(27);
            auto repeteMusica = chrono::duration_cast<chrono::seconds>(fimMusica - now).count();

            if (repeteMusica == 0){
                tocaMusica = true;
            }

            if (remaining_time == 0) {
                personagem1Vivo = false;
                personagem2Vivo = false;
                gameLoop = false;
            };

            int minutes = remaining_time / 60;
            int seconds = remaining_time % 60;

            if (configuracaoAtual.faseLoop == 4){
                gameLoop = false;
            }

            ///Posiciona a escrita no início do console
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "         Fase " << configuracaoAtual.faseLoop;
            ///Impressão do jogo no console
            for(int i=0;i<10;i++){
                for(int j=0;j<22;j++){

                    if(i==xPersonagem1 && j==yPersonagem1){
                        cout<<char(2); ///Personagem 1
                    }

                    else if(i==xPersonagem2 && j==yPersonagem2 && configuracaoAtual.numeroDeJogadores == 2){
                        cout<<char(1);
                    }

                    else {
                        ///O caractere de explosão da bomba se mantém em tela por apenas 80 iterações do while principal
                        if(explosaoBomba1 == 80 && m[i][j] == 4){m[i][j] = 0;}
                        if(explosaoBomba2 == 80 && m[i][j] == 4){m[i][j] = 0;}

                        if (m[vidaExtra.x][vidaExtra.y] == 0 && vidaExtra.emUso == false){m[vidaExtra.x][vidaExtra.y] = 10;}
                        if (m[bombaExtra.x][bombaExtra.y] == 0 && bombaExtra.emUso == false){m[bombaExtra.x][bombaExtra.y] = 11;}
                        if (m[raioExplosao.x][raioExplosao.y] == 0 && raioExplosao.emUso == false){m[raioExplosao.x][raioExplosao.y] = 12;}
                        if (m[bombaRelogio.x][bombaRelogio.y] == 0 && bombaRelogio.emUso == false){m[bombaRelogio.x][bombaRelogio.y] = 13;}
                        if (m[sobreviveBomba.x][sobreviveBomba.y] == 0 && sobreviveBomba.emUso == false){m[sobreviveBomba.x][sobreviveBomba.y] = 14;}
                        if (m[atravessaCx.x][atravessaCx.y] == 0 && atravessaCx.emUso == false){m[atravessaCx.x][atravessaCx.y] = 15;}
                        if (m[explodeMais.x][explodeMais.y] == 0 && explodeMais.emUso == false){m[explodeMais.x][explodeMais.y] = 16;}
                        if (m[xPortal][yPortal] == 0){m[xPortal][yPortal] = 17;}

                        switch (m[i][j]){
                            case 0: cout<<" "; break; ///Caminho
                            case 1: cout<<char(219); break; ///Parede sólida
                            case 2: cout<<char(177); break; ///Parede frágil
                            case 3: cout<<char(15); break; ///Bomba
                            case 4: cout<<char(158); break; ///Explosão
                            case 5: cout<<char(6); break; ///Inimigos
                            case 6: cout<<char(207); break; ///Bomba piscando
                            case 10: cout<<char(3); break; ///Vida extra
                            case 11: cout<<char(15); break; ///Bomba extra
                            case 12: cout<<char(29); break; ///Raio explosão
                            case 13: cout<<char(169); break; ///Bomba relógio
                            case 14: cout<<char(5); break; ///Sobrevive bomba
                            case 15: cout<<char(175); break; ///Atravessa Caixotes
                            case 16: cout<<char(31); break; ///Destruição acompanha raio de explosão
                            case 17: cout<<char(127); break; ///Portal fechado
                        }
                    }
                }
                cout<<"\n";
            }

            if (seconds != previous_seconds) {
                cout << " Tempo restante: " << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds << flush;
                previous_seconds = seconds;
            }

            if (configuracaoAtual.faseLoop != faseAtual || resetTimerVidaExtra == true) {
                resetTimer = true;
                resetTimerVidaExtra = false;
                faseAtual = configuracaoAtual.faseLoop;
            }

            cout << endl;
            cout << " Passos andados - " << configuracaoAtual.nomeJogador1 << ": " << contaPassos1 << "\n";
            cout << " Bombas plantadas - " << configuracaoAtual.nomeJogador1 << ": " << contaBombas1 << "\n";
            cout << endl;
            if (configuracaoAtual.numeroDeJogadores == 2){
                cout << " Passos andados - " << configuracaoAtual.nomeJogador2 << ": " << contaPassos2 << "\n";
            }
            if (configuracaoAtual.numeroDeJogadores == 2){
                cout << " Bombas plantadas - " << configuracaoAtual.nomeJogador2 << ": " << contaBombas2 << "\n";
            }
            cout << endl;
            cout << " " << configuracaoAtual.nomeJogador1 << ": " << endl;
            cout << " " << char(3) << " = " << vidaExtra.poderPersonagem1 << " | " << char(15) << " = " << bombaExtra.poderPersonagem1 <<
            " | " << char(29) << " = " << raioExplosao.poderPersonagem1 << " | " << char(169) << " = " << bombaRelogio.poderPersonagem1 <<
            " | " << char(5) << " = " << sobreviveBomba.poderPersonagem1 << " | " << char(175) << " = " << atravessaCx.poderPersonagem1 <<
            " | " << char(31) << " = " << explodeMais.poderPersonagem1 << endl;

            if (configuracaoAtual.numeroDeJogadores == 2){
                cout << " " << configuracaoAtual.nomeJogador2 << ": " << endl;
                cout << " " << char(3) << " = " << vidaExtra.poderPersonagem2 << " | " << char(15) << " = " << bombaExtra.poderPersonagem2 <<
                " | " << char(29) << " = " << raioExplosao.poderPersonagem2 << " | " << char(169) << " = " << bombaRelogio.poderPersonagem2 <<
                " | " << char(5) << " = " << sobreviveBomba.poderPersonagem2 << " | " << char(175) << " = " << atravessaCx.poderPersonagem2 <<
                " | " << char(31) << " = " << explodeMais.poderPersonagem2 << endl;

            }

            ///Movimentação dos inimigos (a cada 100 iterações)
            if (timerInimigo == 100){

                ///Números aleatórios em intervalos específicos, para controle de deslocamento, direção e escolha de inimigos
                srand(time(0));
                deslocamentos = (rand() % 3) + 1; ///1, 2, 3
                direcao = (rand() % 4) + 1; ///1, 2, 3, 4

                Inimigo maisProximoLinha;
                Inimigo maisProximoColuna;
                Inimigo maisProximo;

                for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
                    for (int k = 0; k < configuracaoAtual.numeroDeInimigos; k++){
                        selecionaInimigo(maisProximoLinha, maisProximoColuna, maisProximo, inimigos, xPersonagem1, configuracaoAtual.numeroDeInimigos);
                        if (selecionaInimigo(maisProximoLinha, maisProximoColuna, maisProximo, inimigos, xPersonagem1, configuracaoAtual.numeroDeInimigos).xInimigo == inimigos[k].xInimigo &&
                            selecionaInimigo(maisProximoLinha, maisProximoColuna, maisProximo, inimigos, xPersonagem1, configuracaoAtual.numeroDeInimigos).yInimigo == inimigos[k].yInimigo)
                            {inimigoEscolhido = &inimigos[k]; break;}
                    }
                }


                ///Condicionais de controle de direção e deslocamento dos inimigos (cima, baixo, esquerda e direita)
                if (configuracaoAtual.dificuldade == 2){
                    int prob = (rand() % 2) + 1;
                    if (prob == 1 || faseAtual == 3){
                        if (inimigoEscolhido->xInimigo == xPersonagem1){
                            if (inimigoEscolhido->yInimigo > yPersonagem1){direcao = 3;}
                            if (inimigoEscolhido->yInimigo < yPersonagem1){direcao = 4;}
                        }
                        if (inimigoEscolhido->yInimigo == yPersonagem1){
                            if (inimigoEscolhido->xInimigo > xPersonagem1){direcao = 1;}
                            if (inimigoEscolhido->xInimigo < xPersonagem1){direcao = 2;}
                        }
                    }
                }

                if (configuracaoAtual.dificuldade == 3 || faseAtual == 3){
                    int prob = (rand() % 4) + 1;
                    if (prob == 1 || prob == 2 || prob == 3){
                        if (inimigoEscolhido->xInimigo == xPersonagem1){
                            if (inimigoEscolhido->yInimigo > yPersonagem1){direcao = 3;}
                            if (inimigoEscolhido->yInimigo < yPersonagem1){direcao = 4;}
                        }
                        if (inimigoEscolhido->yInimigo == yPersonagem1){
                            if (inimigoEscolhido->xInimigo > xPersonagem1){direcao = 1;}
                            if (inimigoEscolhido->xInimigo < xPersonagem1){direcao = 2;}
                        }
                    }
                }

                if(direcao == 1){
                    for (int i = 0; i < deslocamentos; i++){
                        cima(*inimigoEscolhido);
                        if (cima(*inimigoEscolhido) == true){
                            inimigoDeslocaCima(*inimigoEscolhido);
                        break;
                        }
                    }
                }

                if(direcao == 2){
                    for (int i = 0; i < deslocamentos; i++){
                        baixo(*inimigoEscolhido);
                        if (baixo(*inimigoEscolhido) == true){
                            inimigoDeslocaBaixo(*inimigoEscolhido);
                        break;
                        }
                    }
                }

                if(direcao == 3){
                    for (int i = 0; i < deslocamentos; i++){
                        esquerda(*inimigoEscolhido);
                        if (esquerda(*inimigoEscolhido) == true){
                            inimigoDeslocaEsquerda(*inimigoEscolhido);
                        break;
                        }
                    }
                }

                if(direcao == 4){
                    for (int i = 0; i < deslocamentos; i++){
                        direita(*inimigoEscolhido);
                        if (direita(*inimigoEscolhido) == true){
                            inimigoDeslocaDireita(*inimigoEscolhido);
                        break;
                        }
                    }
                }
            }

            ///Execução dos movimentos
            if ( _kbhit() ){
                tecla = getch();
                switch(tecla){

                    case 'w': case 'W':///Andar para cima
                        cima(xPersonagem1, yPersonagem1, personagem1Vivo);
                        if (cima(xPersonagem1, yPersonagem1, personagem1Vivo) == true){xPersonagem1--; contaPassos1++;}
                    break;

                    case 72:
                        cima(xPersonagem2, yPersonagem2, personagem2Vivo);
                        if (cima(xPersonagem2, yPersonagem2, personagem2Vivo) == true){xPersonagem2--; contaPassos2++;}
                    break;

                    case 's': case 'S': ///Andar para baixo
                        baixo(xPersonagem1, yPersonagem1, personagem1Vivo);
                        if (baixo(xPersonagem1, yPersonagem1, personagem1Vivo) == true){xPersonagem1++; contaPassos1++;}
                    break;

                    case 80:
                        baixo(xPersonagem2, yPersonagem2, personagem2Vivo);
                        if (baixo(xPersonagem2, yPersonagem2, personagem2Vivo) == true){xPersonagem2++; contaPassos2++;}
                    break;

                    case 'a': case 'A': ///Andar para a esquerda
                        esquerda(xPersonagem1, yPersonagem1, personagem1Vivo);
                        if (esquerda(xPersonagem1, yPersonagem1, personagem1Vivo) == true){yPersonagem1--; contaPassos1++;}
                    break;

                    case 75:
                        esquerda(xPersonagem2, yPersonagem2, personagem2Vivo);
                        if (esquerda(xPersonagem2, yPersonagem2, personagem2Vivo) == true){yPersonagem2--; contaPassos2++;}
                    break;

                    case 'd': case 'D': ///Andar para a direita
                        direita(xPersonagem1, yPersonagem1, personagem1Vivo);
                        if (direita(xPersonagem1, yPersonagem1, personagem1Vivo) == true){yPersonagem1++; contaPassos1++;}
                    break;

                    case 77:
                        direita(xPersonagem2, yPersonagem2, personagem2Vivo);
                        if (direita(xPersonagem2, yPersonagem2, personagem2Vivo) == true){yPersonagem2++; contaPassos2++;}
                    break;

                    case 32: ///Plantar a bomba (barra de espaço)
                        plantaBomba(xBomba1, yBomba1);///A função plantaBomba recebe as coordendas do personagem, pois ele planta na posi  o em que est
                        if (plantaBomba(xBomba1, yBomba1) == true && personagem1Vivo == true){
                            xBomba1 = xPersonagem1;
                            yBomba1 = yPersonagem1;
                            m[xBomba1][yBomba1] = 3;
                            contaBombas1++;
                            bomba1EmJogo = true; ///Indica que há uma bomba no mapa, impedindo que outra seja plantada antes da explos o
                        }
                    break;

                    case 13:
                        if (configuracaoAtual.numeroDeJogadores == 2){
                            plantaBomba(xBomba2, yBomba2);///A função plantaBomba recebe as coordendas do personagem, pois ele planta na posi  o em que est
                            if (plantaBomba(xBomba2, yBomba2) == true && personagem2Vivo == true){
                                xBomba2 = xPersonagem2;
                                yBomba2 = yPersonagem2;
                                m[xBomba2][yBomba2] = 3;
                                contaBombas2++;
                                bomba2EmJogo = true; ///Indica que há uma bomba no mapa, impedindo que outra seja plantada antes da explos o
                            }
                        }
                    break;
                }
            }

            ///A cada 100 iterações no while principal, um inimigo aleatório irá deslocar em direção aleatória
            if (timerInimigo == 100){timerInimigo = 0;}///A variável sempre zera de 100 em 100 iterações
            timerInimigo++;

            if (bomba1EmJogo == true){timerBomba1++;}
            if (bomba2EmJogo == true){timerBomba2++;}

            if (timerBombaRelogio1 == true){
                if (timerBomba1 == 100){timerBomba1 = 1;}
            }

            if (timerBombaRelogio1 == false && bomba1EmJogo == true && explodeBombaRelogio == true){
                explodeBombaRelogio = false;
                explodeBomba(xBomba1, yBomba1, inimigos);
            }

            if (timerBomba1 == 200){timerBomba1 = 0; explodeBomba(xBomba1, yBomba1, inimigos);}///A bomba sempre explode a cada 200 iterações no while principal, para dar tempo de "piscar"
            if (timerBomba2 == 200){timerBomba2 = 0; explodeBomba(xBomba2, yBomba2, inimigos);}

            ///Aqui controlamos o "piscar" da bomba, mudando o caractere impresso entre múltiplos de 10 e 30
            if (timerBomba1 >= 1 && timerBomba1 % 10 == 0){m[xBomba1][yBomba1] = 6;}
            if (timerBomba1 >= 1 && timerBomba1 % 30 == 0){m[xBomba1][yBomba1] = 3;}

            if (timerBomba2 >= 1 && timerBomba2 % 10 == 0){m[xBomba2][yBomba2] = 6;}
            if (timerBomba2 >= 1 && timerBomba2 % 30 == 0){m[xBomba2][yBomba2] = 3;}

            ///Aqui mantemos o caractere de explosão da bomba em tela durante 80 iterações no while principal. Depois vira 0 (caminho)
            if (explosaoBomba1 == 80){explosaoBomba1 = 0;}
            if (explosaoBomba1 >= 1 && explosaoBomba1 < 80){explosaoBomba1++;}

            if (explosaoBomba2 == 80){explosaoBomba2 = 0;}
            if (explosaoBomba2 >= 1 && explosaoBomba2 < 80){explosaoBomba2++;}

            if (xPersonagem1 == vidaExtra.x && yPersonagem1 == vidaExtra.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    vidaExtra.emUso = true;
                    vidaExtra.poderPersonagem1 += 1;
                    m[vidaExtra.x][vidaExtra.y] = 0;
                    vidaExtra.x = -1;
                    vidaExtra.y = -1;
                }
            }

            if (xPersonagem1 == bombaExtra.x && yPersonagem1 == bombaExtra.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    bombaExtra.emUso = true;
                    bombaExtra.poderPersonagem1 += 1;
                    m[bombaExtra.x][bombaExtra.y] = 0;
                    bombaExtra.x = -1;
                    bombaExtra.y = -1;
                }
            }

            if (xPersonagem1 == raioExplosao.x && yPersonagem1 == raioExplosao.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                	raioExplosao.emUso = true;
                    raioExplosao.poderPersonagem1 += 1;
                    m[raioExplosao.x][raioExplosao.y] = 0;
                    raioExplosao.x = -1;
                    raioExplosao.y = -1;
                }
            }

            if (xPersonagem1 == bombaRelogio.x && yPersonagem1 == bombaRelogio.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    bombaRelogio.emUso = true;
                    bombaRelogio.poderPersonagem1 += 1;
                    m[bombaRelogio.x][bombaRelogio.y] = 0;
                    bombaRelogio.x = -1;
                    bombaRelogio.y = -1;
                }
            }

            if (xPersonagem1 == sobreviveBomba.x && yPersonagem1 == sobreviveBomba.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    sobreviveBomba.emUso = true;
                    sobreviveBomba.poderPersonagem1 += 1;
                    m[sobreviveBomba.x][sobreviveBomba.y] = 0;
                    sobreviveBomba.x = -1;
                    sobreviveBomba.y = -1;
                }
            }

            if (xPersonagem1 == atravessaCx.x && yPersonagem1 == atravessaCx.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    atravessaCx.emUso = true;
                    atravessaCx.poderPersonagem1 += 1;
                    m[atravessaCx.x][atravessaCx.y] = 0;
                    atravessaCx.x = -1;
                    atravessaCx.y = -1;
                }
            }

            if (xPersonagem1 == explodeMais.x && yPersonagem1 == explodeMais.y){
                if ((atravessaCx.poderPersonagem1 == 0) || (atravessaCx.poderPersonagem1 == 1 && m[xPersonagem1][yPersonagem1] != 2)){
                    explodeMais.emUso = true;
                    explodeMais.poderPersonagem1 += 1;
                    m[explodeMais.x][explodeMais.y] = 0;
                    explodeMais.x = -1;
                    explodeMais.y = -1;
                }
            }

            if (configuracaoAtual.numeroDeJogadores == 2){
            	if (xPersonagem2 == vidaExtra.x && yPersonagem2 == vidaExtra.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        vidaExtra.emUso = true;
                        vidaExtra.poderPersonagem2 += 1;
                        m[vidaExtra.x][vidaExtra.y] = 0;
                        vidaExtra.x = -1;
                        vidaExtra.y = -1;
                    }
            	}

            	if (xPersonagem2 == bombaExtra.x && yPersonagem2 == bombaExtra.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        bombaExtra.emUso = true;
                        bombaExtra.poderPersonagem2 += 1;
                        m[bombaExtra.x][bombaExtra.y] = 0;
                        bombaExtra.x = -1;
                        bombaExtra.y = -1;
                    }
            	}

            	if (xPersonagem2 == raioExplosao.x && yPersonagem2 == raioExplosao.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        raioExplosao.emUso = true;
                        raioExplosao.poderPersonagem2 += 1;
                        m[raioExplosao.x][raioExplosao.y] = 0;
                        raioExplosao.x = -1;
                        raioExplosao.y = -1;
                    }
            	}

            	if (xPersonagem2 == bombaRelogio.x && yPersonagem2 == bombaRelogio.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        bombaRelogio.emUso = true;
                        bombaRelogio.poderPersonagem2 += 1;
                        m[bombaRelogio.x][bombaRelogio.y] = 0;
                        bombaRelogio.x = -1;
                        bombaRelogio.y = -1;
                    }
            	}

            	if (xPersonagem2 == sobreviveBomba.x && yPersonagem2 == sobreviveBomba.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        sobreviveBomba.emUso = true;
                        sobreviveBomba.poderPersonagem2 += 1;
                        m[sobreviveBomba.x][sobreviveBomba.y] = 0;
                        sobreviveBomba.x = -1;
                        sobreviveBomba.y = -1;
                    }
            	}

            	if (xPersonagem2 == atravessaCx.x && yPersonagem2 == atravessaCx.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        atravessaCx.emUso = true;
                        atravessaCx.poderPersonagem2 += 1;
                        m[atravessaCx.x][atravessaCx.y] = 0;
                        atravessaCx.x = -1;
                        atravessaCx.y = -1;
                    }
            	}

            	if (xPersonagem2 == explodeMais.x && yPersonagem2 == explodeMais.y){
                    if ((atravessaCx.poderPersonagem2 == 0) || (atravessaCx.poderPersonagem2 == 1 && m[xPersonagem2][yPersonagem2] != 2)){
                        explodeMais.emUso = true;
                        explodeMais.poderPersonagem2 += 1;
                        m[explodeMais.x][explodeMais.y] = 0;
                        explodeMais.x = -1;
                        explodeMais.y = -1;
                    }
            	}
            }

            ///Aqui verificamos se todos os inimigos já morreram. Se sim, sai do while principal e acaba o jogo.
            inimigosMortos = 0;
            for (int i = 0; i < configuracaoAtual.numeroDeInimigos; i++){
                if (inimigos[i].xInimigo == -1 && inimigos[i].yInimigo == -1){inimigosMortos++;}
            }

            ///Entrada no portal
            if (configuracaoAtual.numeroDeJogadores == 1){
                if (inimigosMortos == configuracaoAtual.numeroDeInimigos && m[xPersonagem1][yPersonagem1] == m[xPortal][yPortal]){
                    configuracaoAtual.faseLoop++;
                    for (int i = 0; i < 10; i++){
                        for (int j = 0; j < 22; j++){
                            m[i][j] = mReserva[i][j];
                        }
                    }
                    xPersonagem1 = 2;
                    yPersonagem1 = 2;
                    personagem1Vivo = true;
                    vidaExtra.emUso = false;
                    bombaExtra.emUso = false;
                    raioExplosao.emUso = false;
                    bombaRelogio.emUso = false;
                    sobreviveBomba.emUso = false;
                    atravessaCx.emUso = false;
                    explodeMais.emUso = false;
                    bombaRelogio.poderPersonagem1 = 0;
                    sobreviveBomba.poderPersonagem1 = 0;
                    atravessaCx.poderPersonagem1 = 0;
                    explodeMais.poderPersonagem1 = 0;
                    geracaoCaixotes();
                    geracaoInimigos(inimigos, configuracaoAtual);
                }
            }

            if (configuracaoAtual.numeroDeJogadores == 2){
                if (inimigosMortos == configuracaoAtual.numeroDeInimigos && m[xPersonagem1][yPersonagem1] == m[xPortal][yPortal]){xPersonagem1 = -2; yPersonagem1 = -2;}
                if (inimigosMortos == configuracaoAtual.numeroDeInimigos && m[xPersonagem2][yPersonagem2] == m[xPortal][yPortal]){xPersonagem2 = -2; yPersonagem2 = -2;}
                if ((inimigosMortos == configuracaoAtual.numeroDeInimigos && xPersonagem1 == -2 && yPersonagem1 == -2 && xPersonagem2 == -2 && yPersonagem2 == -2)||
                    (inimigosMortos == configuracaoAtual.numeroDeInimigos && personagem1Vivo == false && xPersonagem2 == -2 && yPersonagem2 == -2)||
                    (inimigosMortos == configuracaoAtual.numeroDeInimigos && personagem2Vivo == false && xPersonagem1 == -2 && yPersonagem1 == -2)
                    ){
                    configuracaoAtual.faseLoop++;
                    for (int i = 0; i < 10; i++){
                        for (int j = 0; j < 22; j++){
                            m[i][j] = mReserva[i][j];
                        }
                    }
                    xPersonagem1 = 2;
                    yPersonagem1 = 2;
                    xPersonagem2 = 8;
                    yPersonagem2 = 20;
                    personagem1Vivo = true;
                    personagem2Vivo = true;
                    vidaExtra.emUso = false;
                    bombaExtra.emUso = false;
                    raioExplosao.emUso = false;
                    bombaRelogio.emUso = false;
                    sobreviveBomba.emUso = false;
                    atravessaCx.emUso = false;
                    explodeMais.emUso = false;
                    bombaRelogio.poderPersonagem1 = 0;
                    sobreviveBomba.poderPersonagem1 = 0;
                    atravessaCx.poderPersonagem1 = 0;
                    explodeMais.poderPersonagem1 = 0;
                    bombaRelogio.poderPersonagem2 = 0;
                    sobreviveBomba.poderPersonagem2 = 0;
                    atravessaCx.poderPersonagem2 = 0;
                    explodeMais.poderPersonagem2 = 0;
                    geracaoCaixotes();
                    geracaoInimigos(inimigos, configuracaoAtual);
                }
            }

            ///Aqui verificamos se os personagens estão em área de explosão de bomba. Se sim, morrem para a bomba.
            if (m[xPersonagem1][yPersonagem1] == 4 && sobreviveBomba.poderPersonagem1 == 1){
                timerSobreviveBomba1 = 1;
            }

            if (timerSobreviveBomba1 >= 1 && timerSobreviveBomba1 <= 3){timerSobreviveBomba1++;}
            if (timerSobreviveBomba1 == 3){
                timerSobreviveBomba1 = 0;
                sobreviveBomba.poderPersonagem1 = 0;
                for (int i = 0; i < 10; i++){
                    for (int j = 0; j < 22; j++){
                        if (m[i][j] == 4){m[i][j] = 0;}
                    }
                }
            }

            if (m[xPersonagem1][yPersonagem1] == 4 && sobreviveBomba.poderPersonagem1 == 0){
                xPersonagem1 = -1;
                yPersonagem1 = -1;
                personagem1Vivo = false;
            }

            if (configuracaoAtual.numeroDeJogadores == 2){
                if (m[xPersonagem2][yPersonagem2] == 4 && sobreviveBomba.poderPersonagem2 == 1){
                    timerSobreviveBomba2 = 1;
                }

                if (timerSobreviveBomba2 >= 1 && timerSobreviveBomba2 <= 3){timerSobreviveBomba2++;}
                if (timerSobreviveBomba2 == 3){
                    timerSobreviveBomba2 = 0;
                    sobreviveBomba.poderPersonagem2 = 0;
                    for (int i = 0; i < 10; i++){
                        for (int j = 0; j < 22; j++){
                            if (m[i][j] == 4){m[i][j] = 0;}
                        }
                    }
                }

                if (m[xPersonagem2][yPersonagem2] == 4 && sobreviveBomba.poderPersonagem2 == 0){
                    xPersonagem2 = -1;
                    yPersonagem2 = -1;
                    personagem2Vivo = false;
                }
            }

            ///Aqui verificamos se os personagens estão na mesma posição de um inimigo. Se sim, morrem para o inimigo.
            if (m[xPersonagem1][yPersonagem1] == 5){
                xPersonagem1 = -1;
                yPersonagem1 = -1;
                personagem1Vivo = false;
            }

            if (configuracaoAtual.numeroDeJogadores == 2){
                if (m[xPersonagem2][yPersonagem2] == 5){
                    xPersonagem2 = -1;
                    yPersonagem2 = -1;
                    personagem2Vivo = false;
                }
            }

            switch(configuracaoAtual.numeroDeJogadores){
                case 1:
                    if (personagem1Vivo == false && configuracaoAtual.faseLoop < 4){
                        if (vidaExtra.poderPersonagem1 > 0){
                            xPersonagem1 = 2, yPersonagem1 = 2;
                            personagem1Vivo = true;
                            m[xPersonagem1][yPersonagem1] = 0;
                            vidaExtra.poderPersonagem1--;
                            telaFinalFase();
                            break;
                        }
                        if (vidaExtra.poderPersonagem1 == 0){
                            morte1 = true;
                            gameLoop = false;
                        }
                    }
                    if (personagem1Vivo == false && configuracaoAtual.faseLoop == 4){
                        morte1 = true;
                        gameLoop = false;
                    }
                break;

                case 2:
                    if (personagem1Vivo == false && configuracaoAtual.faseLoop < 4){
                        if (vidaExtra.poderPersonagem1 > 0){
                            xPersonagem1 = 2, yPersonagem1 = 2;
                            telaFinalFase();
                            personagem1Vivo = true;
                            m[xPersonagem1][yPersonagem1] = 0;
                            vidaExtra.poderPersonagem1--;
                            break;
                        }
                        if (vidaExtra.poderPersonagem1 == 0){
                            morte1 = true;
                        }
                    }
                    if (personagem1Vivo == false && configuracaoAtual.faseLoop == 4){
                        morte1 = true;
                    }

                    if (personagem2Vivo == false && configuracaoAtual.faseLoop < 4){
                        if (vidaExtra.poderPersonagem2 > 0){
                            xPersonagem2 = 8, yPersonagem2 = 20;
                            telaFinalFase();
                            personagem2Vivo = true;
                            m[xPersonagem2][yPersonagem2] = 0;
                            vidaExtra.poderPersonagem2--;
                            break;
                        }
                        if (vidaExtra.poderPersonagem2 == 0){
                            morte2 = true;
                        }
                    }
                    if (personagem2Vivo == false && configuracaoAtual.faseLoop == 4){
                        morte2 = true;
                    }

                    if (morte1 == true && morte2 == true){
                        gameLoop = false;
                    }
                break;
            }
        }

        ///Fim do jogo, mensagens finais com causa da morte
        if (configuracaoAtual.numeroDeJogadores == 1){
            criarArquivo<int>(configuracaoAtual.dificuldade, contaPassos1, contaBombas1);
        }

        if (configuracaoAtual.numeroDeJogadores == 2){
            double contaPassos = (contaPassos1 + contaPassos2)/2;
            double contaBombas = (contaBombas1 + contaBombas2)/2;
            criarArquivo<double>(configuracaoAtual.dificuldade, contaPassos, contaBombas);
        }
        xPersonagem1 = 2, yPersonagem1 = 2;
        if (configuracaoAtual.numeroDeJogadores == 2){
            xPersonagem2 = 8, yPersonagem2 = 20;
        }

        if (!repeteMenuVidaExtra){
            telaCausaDaMorte();
            telaFinalJogo();
        }
    }
}
