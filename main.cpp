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
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

///Coordenadas do personagem e da bomba
int xPersonagem = 2, yPersonagem = 2;
int xBomba, yBomba;

///Controle de movimento aleatório dos inimigos
int deslocamentos;
int direcao;
int inimigo;

///Controle de tempo entre movimentação de inimigos e explosão de bomba.
int timerInimigo = 0;
int timerBomba = 0;

///Controla quanto tempo o caractere de explosão fica em tela
int explosaoBomba = 0;

///Controles gerais de execução do jogo
bool gameLoop = true;
bool bombaEmJogo = false;

string causaDaMorte = "Indefinida";

///Variavel para tecla pressionada
char tecla;

///Matriz que controla a impressão do mapa
///0: Caminho livre
///1: Parede sólida
///2: Parede frágil
///3 e 6: Caracteres da bomba (pois ela pisca)
///4: Caractere de explosão da bomba
///5: Inimigos
int m[10][22]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
               0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
               0,1,0,0,2,0,2,2,0,0,2,0,2,0,2,0,2,0,0,0,5,1,
               0,1,0,1,2,1,2,1,0,1,0,1,2,1,2,1,0,1,0,1,0,1,
               0,1,2,0,2,0,2,0,0,0,2,2,2,0,2,2,2,0,0,2,0,1,
               0,1,0,1,2,1,2,1,5,1,2,1,2,1,0,1,2,1,2,1,2,1,
               0,1,0,0,2,0,2,2,0,0,0,0,2,0,2,2,0,0,0,0,0,1,
               0,1,0,1,0,1,2,1,2,1,0,1,0,1,2,1,2,1,0,1,0,1,
               0,1,5,0,0,2,2,0,0,2,0,0,0,2,2,2,0,2,0,0,5,1,
               0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

///Estrutura do inimigo
struct Inimigo{
    int xInimigo;
    int yInimigo;
};

///Inimigos
Inimigo a;
Inimigo b;
Inimigo c;
Inimigo d;

///Ponteiro para armazenar o inimigo escolhido em questão
Inimigo* inimigoEscolhido;

///As 4 próximas funções são booleanos que controlam a possibilidade de movimentação do personagem
bool personagemCima(int x, int y){

    if(m[x-1][y] == 1 || m[x-1][y] == 2 || m[x-1][y] == 3 || m[x-1][y] == 6){
        return false;
    }
    else{
        return true;
    }
}

bool personagemBaixo(int x, int y){

    if(m[x+1][y] == 1 || m[x+1][y] == 2 || m[x+1][y] == 3 || m[x+1][y] == 6){
        return false;
    }
    else{
        return true;
    }
}

bool personagemEsquerda(int x, int y){

    if(m[x][y-1] == 1 || m[x][y-1] == 2 || m[x][y-1] == 3 || m[x][y-1] == 6){
        return false;
    }
    else{
        return true;
    }
}

bool personagemDireita(int x, int y){

    if(m[x][y+1] == 1 || m[x][y+1] == 2 || m[x][y+1] == 3 || m[x][y+1] == 6){
        return false;
    }
    else{
        return true;
    }
}

///As 4 próximas funções são booleanos que controlam a possibilidade de movimentação dos inimigos
bool inimigoCima(Inimigo &inimigoEscolhido){

   if(m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 1 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 2 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 3 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 5 ||
      m[inimigoEscolhido.xInimigo-1][inimigoEscolhido.yInimigo] == 6 ||
      ///As coordenadas -1 indicam que o inimigo já morreu, uma vez que não existe essa coordenada na matriz.
      inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1){
        return false;
    }
    else{
        return true;
    }
}

bool inimigoBaixo(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 1 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 2 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 3 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 5 ||
       m[inimigoEscolhido.xInimigo+1][inimigoEscolhido.yInimigo] == 6 ||
       inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1){
        return false;
    }
    else{
        return true;
    }
}

bool inimigoEsquerda(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 1 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 2 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 3 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 5 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo-1] == 6 ||
       inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1){
        return false;
    }
    else{
        return true;
    }
}

bool inimigoDireita(Inimigo &inimigoEscolhido){

    if(m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 1 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 2 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 3 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 5 ||
       m[inimigoEscolhido.xInimigo][inimigoEscolhido.yInimigo+1] == 6 ||
       inimigoEscolhido.xInimigo == -1 && inimigoEscolhido.yInimigo == -1){
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

    if (bombaEmJogo == true){
        return false; ///Uma bomba nunca pode ser plantada se a outra ainda estiver em jogo
    }
    else {
        return true;
    }
}

///Função que controla a explosão da bomba em si
void explodeBomba(int x, int y){

    ///Explosão das posições adjacentes à bomba, destruindo paredes frágeis
    if (m[x-1][y] == 2 || m[x-1][y] == 0){m[x-1][y] = 4;}
    if (m[x+1][y] == 2 || m[x+1][y] == 0){m[x+1][y] = 4;}
    if (m[x][y-1] == 2 || m[x][y-1] == 0){m[x][y-1] = 4;}
    if (m[x][y+1] == 2 || m[x][y+1] == 0){m[x][y+1] = 4;}

    ///Personagem em posição adjacente à bomba (morte)
    if (
        m[x-1][y] == m[xPersonagem][yPersonagem] ||
        m[x+1][y] == m[xPersonagem][yPersonagem] ||
        m[x][y-1] == m[xPersonagem][yPersonagem] ||
        m[x][y+1] == m[xPersonagem][yPersonagem] ||
        m[x][y] == m[xPersonagem][yPersonagem]
        ){gameLoop = false;}

    ///Algum inimigo em posição adjacente à bomba (morte do inimigo em questão)
    if (
        x-1 == a.xInimigo && y == a.yInimigo ||
        x+1 == a.xInimigo && y == a.yInimigo ||
        x == a.xInimigo && y-1 == a.yInimigo ||
        x == a.xInimigo && y+1 == a.yInimigo ||
        x == a.xInimigo && y == a.yInimigo
        ){m[a.xInimigo][a.yInimigo] = 4; a.xInimigo = -1; a.yInimigo = -1;}

    else if (
        x-1 == b.xInimigo && y == b.yInimigo ||
        x+1 == b.xInimigo && y == b.yInimigo ||
        x == b.xInimigo && y-1 == b.yInimigo ||
        x == b.xInimigo && y+1 == b.yInimigo ||
        x == b.xInimigo && y == b.yInimigo
        ){m[b.xInimigo][b.yInimigo] = 4; b.xInimigo = -1; b.yInimigo = -1;}

    else if (
        x-1 == c.xInimigo && y == c.yInimigo ||
        x+1 == c.xInimigo && y == c.yInimigo ||
        x == c.xInimigo && y-1 == c.yInimigo ||
        x == c.xInimigo && y+1 == c.yInimigo ||
        x == c.xInimigo && y == c.yInimigo
        ){m[c.xInimigo][c.yInimigo] = 4; c.xInimigo = -1; c.yInimigo = -1;}

    else if (
        x-1 == d.xInimigo && y == d.yInimigo ||
        x+1 == d.xInimigo && y == d.yInimigo ||
        x == d.xInimigo && y-1 == d.yInimigo ||
        x == d.xInimigo && y+1 == d.yInimigo ||
        x == d.xInimigo && y == d.yInimigo
        ){m[d.xInimigo][d.yInimigo] = 4; d.xInimigo = -1; d.yInimigo = -1;}

    m[xBomba][yBomba] = 4;
    ///Coordenadas da bomba recebem -1, para que não seja impressa no mapa
    xBomba = -1;
    yBomba = -1;
    bombaEmJogo = false;
    explosaoBomba = 1;
}

///Impressão do menu inicial
void imprimeMenu(){

    char x = char(205);
    char y = char(206);

    cout<<" "<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x;
    cout<<"                      ";
    cout<<"       Bem-vindo(a)     ";
    cout<<"      ao Bomberman!     ";
    cout<<"                      ";
    cout<<"                      ";
    cout<<"      Pressione ENTER   ";
    cout<<"      para iniciar!     ";
    cout<<"                      ";
    cout<<"  "<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x;

    getchar();
    cout<<endl;
}

int main()
{
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

    ///Coordenadas iniciais de todos os inimigos
    a.xInimigo = 2;
    a.yInimigo = 20;

    b.xInimigo = 8;
    b.yInimigo = 2;

    c.xInimigo = 8;
    c.yInimigo = 20;

    d.xInimigo = 5;
    d.yInimigo = 8;

    imprimeMenu();

    while(gameLoop == true){

        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        ///Impressão do jogo no console
        for(int i=0;i<10;i++){
            for(int j=0;j<22;j++){

                if(i==xPersonagem && j==yPersonagem){
                    cout<<char(2); ///Personagem
                }

                else {
                    if(explosaoBomba == 80 && m[i][j] == 4){m[i][j] = 0;}///O caractere de explosão da bomba se mantém em tela por apenas 80 iterações do while principal
                    switch (m[i][j]){
                        case 0: cout<<" "; break; ///Caminho
                        case 1: cout<<char(219); break; ///Parede sólida
                        case 2: cout<<char(177); break; ///Parede frágil
                        case 3: cout<<char(15); break; ///Bomba
                        case 4: cout<<char(158); break; ///Explosão
                        case 5: cout<<char(6); break; ///Inimigos
                        case 6: cout<<char(207); break; ///Bomba piscando
                    }
                }
            }
            cout<<"\n";
        }

        ///Movimentação dos inimigos (a cada 100 iterações)
        if (timerInimigo == 100){

            ///Números aleatórios em intervalos específicos, para controle de deslocamento, direção e escolha de inimigos
            srand(time(0));
            deslocamentos = (rand() % 3) + 1; ///1, 2, 3
            direcao = (rand() % 4) + 1; ///1, 2, 3, 4
            inimigo = (rand() % 4); ///0, 1, 2, 3

            ///Atribuição do endereço do respectivo inimigo ao ponteiro
            if(inimigo == 0){inimigoEscolhido = &a;}
            if(inimigo == 1){inimigoEscolhido = &b;}
            if(inimigo == 2){inimigoEscolhido = &c;}
            if(inimigo == 3){inimigoEscolhido = &d;}

            ///Condicionais de controle de direção e deslocamento dos inimigos (cima, baixo, esquerda e direita)
            if(direcao == 1){
                for (int i = 0; i < deslocamentos; i++){
                    inimigoCima(*inimigoEscolhido);
                    if (inimigoCima(*inimigoEscolhido) == true){
                        inimigoDeslocaCima(*inimigoEscolhido);
                    break;
                    }
                }
            }

            if(direcao == 2){
                for (int i = 0; i < deslocamentos; i++){
                    inimigoBaixo(*inimigoEscolhido);
                    if (inimigoBaixo(*inimigoEscolhido) == true){
                        inimigoDeslocaBaixo(*inimigoEscolhido);
                    break;
                    }
                }
            }

            if(direcao == 3){
                for (int i = 0; i < deslocamentos; i++){
                    inimigoEsquerda(*inimigoEscolhido);
                    if (inimigoEsquerda(*inimigoEscolhido) == true){
                        inimigoDeslocaEsquerda(*inimigoEscolhido);
                    break;
                    }
                }
            }

            if(direcao == 4){
                for (int i = 0; i < deslocamentos; i++){
                    inimigoDireita(*inimigoEscolhido);
                    if (inimigoDireita(*inimigoEscolhido) == true){
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

                case 72: case 'w': ///Andar para cima
                    personagemCima(xPersonagem, yPersonagem);
                    if (personagemCima(xPersonagem, yPersonagem) == true){xPersonagem--;}
                break;

                case 80: case 's': ///Andar para baixo
                    personagemBaixo(xPersonagem, yPersonagem);
                    if (personagemBaixo(xPersonagem, yPersonagem) == true){xPersonagem++;}
                break;

                case 75:case 'a': ///Andar para a esquerda
                    personagemEsquerda(xPersonagem, yPersonagem);
                    if (personagemEsquerda(xPersonagem, yPersonagem) == true){yPersonagem--;}
                break;

                case 77: case 'd': ///Andar para a direita
                    personagemDireita(xPersonagem, yPersonagem);
                    if (personagemDireita(xPersonagem, yPersonagem) == true){yPersonagem++;}
                break;

                case 32: ///Plantar a bomba (barra de espaço)
                    plantaBomba(xPersonagem, yPersonagem);///A função plantaBomba recebe as coordendas do personagem, pois ele planta na posição em que está
                    if (plantaBomba(xPersonagem, yPersonagem) == true){
                        xBomba = xPersonagem;
                        yBomba = yPersonagem;
                        m[xBomba][yBomba] = 3;
                        bombaEmJogo = true; ///Indica que há uma bomba no mapa, impedindo que outra seja plantada antes da explosão
                    }
                break;
            }
         }

        ///A cada 100 iterações no while principal, um inimigo aleatório irá deslocar em direção aleatória
        if (timerInimigo == 100){timerInimigo = 0;}///A variável sempre zera de 100 em 100 iterações
        timerInimigo++;

        if (bombaEmJogo == true){timerBomba++;}

        if (timerBomba == 200){timerBomba = 0; explodeBomba(xBomba, yBomba);}///A bomba sempre explode a cada 200 iterações no while principal, para dar tempo de "piscar"

        ///Aqui controlamos o "piscar" da bomba, mudando o caractere impresso entre múltiplos de 10 e 30
        if (timerBomba >= 1 && timerBomba % 10 == 0){m[xBomba][yBomba] = 6;}
        if (timerBomba >= 1 && timerBomba % 30 == 0){m[xBomba][yBomba] = 3;}

        ///Aqui mantemos o caractere de explosão da bomba em tela durante 80 iterações no while principal. Depois vira 0 (caminho)
        if (explosaoBomba == 80){explosaoBomba = 0;}
        if (explosaoBomba >= 1 && explosaoBomba < 80){explosaoBomba++;}

        ///Aqui verificamos se todos os inimigos já morreram. Se sim, sai do while principal e acaba o jogo.
        if (a.xInimigo == -1 && a.yInimigo == -1 &&
            b.xInimigo == -1 && b.yInimigo == -1 &&
            c.xInimigo == -1 && c.yInimigo == -1 &&
            d.xInimigo == -1 && d.yInimigo == -1)
            {gameLoop = false;}

        ///Aqui verificamos se o personagem está em área de explosão de bomba. Se sim, morre para a bomba, sai do while principal e acaba o jogo.
        if (m[xPersonagem][yPersonagem] == 4){
            causaDaMorte = "Bomba";
            gameLoop = false;
        }

        ///Aqui verificamos se o personagem está na mesma posição de um inimigo. Se sim, morre para o inimigo, sai do while principal e acaba o jogo.
        if (m[xPersonagem][yPersonagem] == 5){
            causaDaMorte = "Inimigo";
            gameLoop = false;
        }

    }
    ///Fim do jogo, mensagens finais com causa da morte

    if (causaDaMorte == "Inimigo"){
        system("cls");
        cout << endl;
        cout << endl;
        cout << "       GAME OVER!" << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "       Morto pelo" << endl;
        cout << "        Inimigo!" << endl;
        getchar();
    }

    if (causaDaMorte == "Bomba"){
        system("cls");
        cout << endl;
        cout << endl;
        cout << "       GAME OVER!" << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "       Morto pela" << endl;
        cout << "         Bomba!" << endl;
        getchar();
    }

    if (causaDaMorte == "Indefinida") {
        system("cls");
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "       Parabens!" << endl;
        cout << "      Voce venceu!" << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        getchar();
    }

system("cls");
return 0;
}
