#include <iostream>

using namespace std;

struct Horario {
    int hora, minuto, segundo;
};

Horario a, b, c;

Horario somaHoras(Horario a, Horario b){
    c.segundo = (a.segundo + b.segundo)%60;
    c.minuto = (a.minuto + b.minuto)%60 + (a.segundo + b.segundo)/60;
    c.hora = (a.hora + b.hora) + (a.minuto + b.minuto)/60;
    return c;
}

int main()
{
    cout << "Digite as horas, minutos e segundos de dois horarios que voce deseja somar: \n" << endl;

    cout << "Horario 1: " << endl;
    cout << "Hora: "; cin >> a.hora;
    cout << "Minuto: "; cin >> a.minuto;
    cout << "Segundo: "; cin >> a.segundo;

    cout << endl;

    cout << "Horario 2: " << endl;
    cout << "Hora: "; cin >> b.hora;
    cout << "Minuto: "; cin >> b.minuto;
    cout << "Segundo: "; cin >> b.segundo;
    cout << endl;

    somaHoras(a, b);

    cout << "A soma das horas eh: " << c.hora << "h " << c.minuto << "m " << c.segundo << "s" << endl;
    return 0;
}
