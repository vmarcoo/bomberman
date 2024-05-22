#include "funcoes_menu.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <limits>
#include <cmath>

using namespace std;

void posicionaCursorNoTopo(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(hConsole, topLeft);
}

void verificaCin(){
    /// Verifica se o usuário inseriu um caractere inválido
    clearScreen();
    cout << "\n\n\n\n\n";
    cout << "                 Erro! Opcao invalida!\n\n";
    cout << "\n";
    cout << "       Pressione qualquer tecla para continuar...";
    getch();
    clearScreen();

    /// Limpa o cin
    cin.clear();

    /// Limpa o buffer do cin
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clearScreen(){
    COORD topLeft = {0, 0};
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(consoleHandle, &screenBufferInfo);
    DWORD consoleSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;
    DWORD charsWritten;
    FillConsoleOutputCharacter(consoleHandle, ' ', consoleSize, topLeft, &charsWritten);
    FillConsoleOutputAttribute(consoleHandle, screenBufferInfo.wAttributes, consoleSize, topLeft, &charsWritten);
    SetConsoleCursorPosition(consoleHandle, topLeft);
}

void menuInstrucoes(){
    char x = char(205);
    char y = char(206);
    cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
    cout<<"                      COMO JOGAR:\n\n";
    cout<<" * O jogo consiste em uma adaptacao de Bomberman, com\n";
    cout<<"   uma mecanica similar, adaptada para o funcionamento\n";
    cout<<"   no terminal do sistema operacional.\n";
    cout<<endl;
    cout<<" * O jogo permite ate dois jogadores simultaneamente,\n";
    cout<<"   que poderao movimentar-se livremente pelo mapa. O\n";
    cout<<"   primeiro jogador utilizara as teclas W, A, S, D\n";
    cout<<"   enquanto que o segundo jogador utilizara as setas\n";
    cout<<"   direcionais para se movimentar. Para plantar\n";
    cout<<"   a bomba, o primeiro jogador devera utilizar a\n";
    cout<<"   barra de espaco, e o segundo jogador a tecla ENTER.\n";
    cout<< endl;
    cout<<" * O objetivo do jogo e matar todos os inimigos, que\n";
    cout<<"   morrerao sempre que estiverem no raio de explosao\n";
    cout<<"   da bomba. O jogador perde caso seja morto pela\n";
    cout<<"   bomba ou cruze com algum inimigo no caminho.\n";
    cout<< endl;
    cout<<" * Ha caixotes espalhados pelo mapa, que bloqueiam a\n";
    cout<<"   a passagem dos jogadores, mas podem ser destruidos\n";
    cout<<"   com a explosao da bomba. Ha tambem itens escondidos\n";
    cout<<"   dentro de alguns caixotes, que liberam habilidades\n";
    cout<<"   especiais. O jogo possui tres fases.\n";
    cout<< endl;
    cout<<"                  ITENS E HABILIDADES:\n\n";
    cout<<"   "<<char(3)<<" - Vida extra (cumulativo)\n";
    cout<<"   "<<char(15)<<" - Bomba extra (cumulativo)\n";
    cout<<"   "<<char(29)<<" - Aumento do raio de explosao em 1 (cumulativo)\n";
    cout<<"   "<<char(169)<<" - Bomba relogio\n";
    cout<<"   "<<char(5)<<" - Sobreviver a explosao da bomba\n";
    cout<<"   "<<char(175)<<" - Atravessar caixotes\n";
    cout<<"   "<<char(31)<<" - Explosao acompanha o raio de destruicao da bomba\n";
    cout<<endl;
    cout<<"                     DIFICULDADES:\n\n";
    cout<<" * FACIL - 3 inimigos com movimentacao normal\n\n";
    cout<<" * MEDIO - 5 inimigos com 50% de chance de se mover\n";
    cout<<"   na direcao do jogador\n\n";
    cout<<" * DIFICIL - 7 inimigos com 75% de chance de se mover\n";
    cout<<"   na direcao do jogador\n";
    cout<<endl;
    cout<<"                        RANKING:\n\n";
    cout<<" * A pontuacao sera exibida conforme a performance do\n";
    cout<<"   jogador na rodada. Quanto mais rapido finalizar o\n";
    cout<<"   jogo, menos casas andar, menos bombas plantar,\n";
    cout<<"   menos caixotes destruir e mais inimigos abater,\n";
    cout<<"   maior sera a pontuacao do jogador em questao.\n";
    cout<<endl;
    cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<"\n";
    cout << "   Pressione qualquer tecla para retornar ao menu...\n\n";
    cout<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x<<y<<x;
}
