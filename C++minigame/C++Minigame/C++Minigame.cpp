#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
using namespace std;
int width, height, i, j, ok, p, lose, runda = 1, anulare = 0, sabie, usa1, camera, slash, oks, timp, usa2,
HPt = 1, platosa, poveste, usa2sike, space, reload, win;
long u;
char fantoma = 207, inima = 3;

enum Directie
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SLASH,
    STOP,
} dir, s3, s4, s5;


struct Obiect
{
    int x, y, HP;
};
Obiect player, obstacol, fantoma1, fantoma2, fantoma3, fantoma4,
fantoma5, fantoma6, cufarsus[4], cufarjos[6], cufar2sus[4], cufar2jos[6], p3, p4, p5,
boss[10][20], dagger[6];
bool GameOver = false;

void Intro()

{
    while (poveste == 0)
    {
        if (poveste == 0)
            cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
            << '\n' << '\n' << '\n' << "                                                  Bine ai venit!";
        cout << '\n' << '\n';
        if (space == 0)
            cout << "                                         Apasa SPACE pentru a continua";
        while (!_kbhit());
        if (_getch() != ' ')
        {
            space = 1;
            system("cls");
            cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                << '\n' << '\n' << '\n' << "                                                  Am zis SPACE";
            while (!_kbhit());
            if (_getch() != ' ')
            {
                system("cls");
                cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                    << '\n' << '\n' << '\n' <<
                    "                                SPACE e cea mai mare tasta, cum poti sa o tot ratezi...";
                while (!_kbhit());
                if (_getch() != ' ')
                {
                    system("cls");
                    cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                        << '\n' << '\n' << '\n' <<
                        "                                              Deja devine ridicol";
                    while (!_kbhit());
                    if (_getch() != ' ')
                    {
                        system("cls");
                        cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                            << '\n' << '\n' << '\n' <<
                            "                                  Jocul nu continua pana nu apesi SPACE, sa stii";
                        while (!_kbhit());
                        if (_getch() != ' ')
                        {
                            system("cls");
                            cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                                << '\n' << '\n' << '\n' <<
                                "                                                Incepi sa ma enervezi";
                            while (!_kbhit());
                            if (_getch() != ' ')
                            {
                                system("cls");
                                cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
                                    << '\n' << '\n' << '\n' <<
                                    "                               OK, MI-A AJUNS, DACA NU APESI SPACE ACUM AI PIERDUT JOCUL!!!";
                                while (!_kbhit());
                                if (_getch() != ' ')
                                {
                                    system("cls");
                                    lose = 1;
                                    GameOver = true;
                                    poveste = 2;
                                }
                            }
                        }
                    }
                }
            }
        }
        poveste++;
        system("cls");
    }
}

void SetUp()
{
    dir = STOP;
    s3 = STOP;
    s4 = STOP;
    s5 = STOP;
    camera = 1;
    boss[1][1].x = 31;
    boss[1][1].y = 2;
    boss[1][2].x = 32;
    boss[1][2].y = 2;
    boss[1][3].x = 33;
    boss[1][3].y = 2;
    boss[1][4].x = 34;
    boss[1][4].y = 2;
    boss[1][5].x = 35;
    boss[1][5].y = 2;
    boss[2][1].x = 31;
    boss[2][1].y = 3;
    boss[2][2].x = 32;
    boss[2][2].y = 3;
    boss[2][3].x = 33;
    boss[2][3].y = 3;
    boss[2][4].x = 34;
    boss[2][4].y = 3;
    boss[2][5].x = 35;
    boss[2][5].y = 3;
    boss[3][1].x = 31;
    boss[3][1].y = 4;
    boss[3][2].x = 32;
    boss[3][2].y = 4;
    boss[3][3].x = 33;
    boss[3][3].y = 4;
    boss[3][4].x = 34;
    boss[3][4].y = 4;
    boss[3][5].x = 35;
    boss[3][5].y = 4;
    boss[4][1].x = 28;
    boss[4][1].y = 5;
    boss[4][2].x = 29;
    boss[4][2].y = 5;
    boss[4][3].x = 30;
    boss[4][3].y = 5;
    boss[4][4].x = 31;
    boss[4][4].y = 5;
    boss[4][5].x = 32;
    boss[4][5].y = 5;
    boss[4][6].x = 33;
    boss[4][6].y = 5;
    boss[4][7].x = 34;
    boss[4][7].y = 5;
    boss[4][8].x = 35;
    boss[4][8].y = 5;
    boss[4][9].x = 36;
    boss[4][9].y = 5;
    boss[4][10].x = 37;
    boss[4][10].y = 5;
    boss[4][11].x = 38;
    boss[4][11].y = 5;
    boss[5][1].x = 27;
    boss[5][1].y = 6;
    boss[5][2].x = 28;
    boss[5][2].y = 6;
    boss[5][3].x = 29;
    boss[5][3].y = 6;
    boss[5][4].x = 30;
    boss[5][4].y = 6;
    boss[5][5].x = 31;
    boss[5][5].y = 6;
    boss[5][6].x = 32;
    boss[5][6].y = 6;
    boss[5][7].x = 33;
    boss[5][7].y = 6;
    boss[5][8].x = 34;
    boss[5][8].y = 6;
    boss[5][9].x = 35;
    boss[5][9].y = 6;
    boss[5][10].x = 36;
    boss[5][10].y = 6;
    boss[5][11].x = 37;
    boss[5][11].y = 6;
    boss[5][12].x = 38;
    boss[5][12].y = 6;
    boss[5][13].x = 39;
    boss[5][13].y = 6;
    boss[6][1].x = 26;
    boss[6][1].y = 7;
    boss[6][2].x = 27;
    boss[6][2].y = 7;
    boss[6][3].x = 28;
    boss[6][3].y = 7;
    boss[6][4].x = 29;
    boss[6][4].y = 7;
    boss[6][5].x = 30;
    boss[6][5].y = 7;
    boss[6][6].x = 31;
    boss[6][6].y = 7;
    boss[6][7].x = 32;
    boss[6][7].y = 7;
    boss[6][8].x = 33;
    boss[6][8].y = 7;
    boss[6][9].x = 34;
    boss[6][9].y = 7;
    boss[6][10].x = 35;
    boss[6][10].y = 7;
    boss[6][11].x = 36;
    boss[6][11].y = 7;
    boss[6][12].x = 37;
    boss[6][12].y = 7;
    boss[6][13].x = 38;
    boss[6][13].y = 7;
    boss[6][14].x = 39;
    boss[6][14].y = 7;
    boss[6][15].x = 40;
    boss[6][15].y = 7;
    boss[7][1].x = 30;
    boss[7][1].y = 8;
    boss[7][2].x = 31;
    boss[7][2].y = 8;
    boss[7][3].x = 32;
    boss[7][3].y = 8;
    boss[7][4].x = 33;
    boss[7][4].y = 8;
    boss[7][5].x = 34;
    boss[7][5].y = 8;
    boss[7][6].x = 35;
    boss[7][6].y = 8;
    boss[7][7].x = 36;
    boss[7][7].y = 8;
    boss[8][1].x = 31;
    boss[8][1].y = 9;
    boss[8][2].x = 32;
    boss[8][2].y = 9;
    boss[8][3].x = 33;
    boss[8][3].y = 9;
    boss[8][4].x = 34;
    boss[8][4].y = 9;
    boss[8][5].x = 35;
    boss[8][5].y = 9;
    fantoma1.HP = 1;
    fantoma2.HP = 1;
    fantoma3.HP = 1;
    fantoma4.HP = 1;
    fantoma5.HP = 1;
    cufarsus[1].x = 17;
    cufarsus[1].y = 5;
    cufarsus[2].x = 18;
    cufarsus[2].y = 5;
    cufarsus[3].x = 19;
    cufarsus[3].y = 5;
    cufarjos[1].x = 16;
    cufarjos[1].y = 6;
    cufarjos[2].x = 17;
    cufarjos[2].y = 6;
    cufarjos[3].x = 18;
    cufarjos[3].y = 6;
    cufarjos[4].x = 19;
    cufarjos[4].y = 6;
    cufarjos[5].x = 20;
    cufarjos[5].y = 6;
    width = 25;
    height = 11;
    player.x = 4;
    player.y = 6;
    player.HP = 1;
    fantoma1.x = 12;
    fantoma1.y = 3;
    fantoma2.x = 22;
    fantoma2.y = 8;
    fantoma3.x = 22;
    fantoma3.y = 3;
    fantoma4.x = 22;
    fantoma4.y = 6;
    fantoma5.x = 22;
    fantoma5.y = 9;
    srand(time(NULL));
    //obstacol.x = rand() % width;
    //if(obstacol.x <= 1)
        //obstacol.x = 2;
    //obstacol.y = rand() % height;
    //if(obstacol.y <= 1)
        //obstacol.y = 2;
}

void Draw()
{
    oks = 0;
    system("cls");
    //fantoma = 0;
    //for(int u = 0; u <= 100; u++)
    //{
    //    cout << ++fantoma << " ";
    //   if(u%50 == 0)
    //        cout << '\n';
    //}
    cout << '\n';
    switch (poveste)
    {
    case 1: cout << "          Bun, tu esti acel O din partea stanga a camerei." << '\n' <<
        "          Incearca sa te misti cu una din tastele w, a, s, sau d." << '\n' << '\n'; break;
    case 2: cout << "          Bravo, acum mergi pana la cufarul acela din dreapta ta," << '\n' <<
        "          Ti-am lasat acolo un cadou de care vei avea nevoie." << '\n' << '\n'; break;
    case 3: cout << "          Aceasta e vechea mea sabie de lemn.... da," << '\n' <<
        "          pai altceva n-am avut sa-ti dau deci va trebui sa te multumesti cu asta." << '\n' << '\n' <<
        "          Dupa cum probabil ai remarcat, sub harta iti poti vedea inventarul, care contine" << '\n' <<
        "          toate obiectele pe care le vei aduna pe parcursul calatoriei tale." << '\n' << '\n' <<
        "          Acum ca ai o sabie e timpul sa inveti o noua abilitate!" << '\n' <<
        "          Apasa SPACE" << '\n' << '\n'; break;
    case 4: cout << "          Acesta este atacul tau de la apropiere." << '\n' <<
        "          Vei avea nevoie de el in camera urmatoare, unde te asteapta doi inamici." << '\n' <<
        "          Trebuie sa ai grija cum iti sincronizezi atacul, sau vei fi o tinta usoara, succes!"
        << '\n' << '\n'; break;
    case 6: cout << "          Wow, chiar ai reusit..." << '\n' <<
        "          Asa, acum vino in camera urmatoare, te astept acolo" << '\n' << '\n'; break;
    case 7: cout << "          STAI, era sa uit, ca sa ai macar o sansa, ai nevoie de inca un lucru." << '\n' <<
        "          Vezi ce ti-am lasat in cufarul din stanga ta." << '\n' << '\n'; break;
    case 8: cout << "          Acum esti pregatit, sa spunem, te astept." << '\n' << '\n'; break;
    case 11: cout << "      Salut, am uitat sa iti zic ca vei da de niste arcasi, dar vad ca te-ai descurcat. Sa ma prezint, eu sunt"
        << '\n' << "       Naratorul, cel care a creat locul acesta, pentru a testa razboinici, ca tine. Poate te intrebi de ce ii"
        << '\n' << "       testez.Pai eu nu traiesc pentru totdeauna si cineva va trebui sa imi ia locul intr-o buna zi. Pentru ca ai"
        << '\n' << "       trecut de toate provocarile, iti dau sansa sa devii noul Narator, ce spui?" << '\n' << '\n';
        break;
    case 12: cout << "      Inteleapta alegere, dar ma tem ca, pentru a deveni noul Narator, va trebui sa imi" << '\n'
        << "       arati ca ai \"dedidcatia\" necesara, ca sa ii spun asa. Acesta este un razboinic, ca tine," << '\n' <<
        "       doar ca el a esuat. Daca vrei sa imi castigi puterea, va trebui sa il distrugi, ACUM!" << '\n' << '\n'; break;
    case 13: cout << "      Inteleapta alegere, dar din pacate, ca sa devii noul Narator va trebui sa imi" << '\n'
        << "       Arati ca ai \"dedicatia\" necesara, ca sa ii spun asa. Acesta este un razboinic, ca tine," << '\n' <<
        "       doar ca el a esuat. Daca vrei sa imi castigi puterea, va trebui sa il distrugi, ACUM!" << '\n' << '\n'; break;
    case 15: cout << "             Atunci MORI" << '\n' << '\n'; break;
    }
    for (i = 1; i <= width / 2 - 3; i++) cout << " ";
    switch (camera)
    {
    case 1: cout << "CAMERA 1"; break;
    case 2: cout << "CAMERA 2"; break;
    case 3: cout << "CAMERA 3"; break;
    }
    cout << '\n' << '\n';
    for (i = 1; i <= width; i++) cout << "=";
    cout << '\n';
    for (i = 2; i < height; i++)
    {
        if (camera == 2)
        {
            if (usa1 == 1 && (i == 5 || i == 7))
                cout << "=";
            else if (usa1 == 1 && i == 6)
                cout << " ";
            else cout << "|";
        }
        else if (camera == 3)
        {
            if (usa2 == 1 && (i == 5 || i == 7))
                cout << "=";
            else if (usa2 == 1 && i == 6)
                cout << " ";
            else cout << "|";
        }
        else cout << "|";
        for (j = 2; j < width; j++)
        {
            if (i == player.y && j == player.x && poveste != 14)
                cout << "O";
            else if (camera == 3 && poveste >= 12 && dagger[1].x == j && dagger[1].y == i)
                cout << "-";
            else if (camera == 3 && poveste >= 12 && dagger[2].x == j && dagger[2].y == i)
                cout << "|";
            else if (camera == 3 && poveste >= 12 && dagger[3].x == j && dagger[3].y == i)
                cout << "-";
            else if (camera == 3 && poveste >= 12 && dagger[4].x == j && dagger[4].y == i)
                cout << "-";
            else if (camera == 3 && poveste >= 12 && dagger[5].x == j && dagger[5].y == i)
                cout << "-";
            else if (cufarsus[1].x == j && cufarsus[1].y == i)
                cout << "_";
            else if (cufarsus[2].x == j && cufarsus[2].y == i)
                cout << "_";
            else if (cufarsus[3].x == j && cufarsus[3].y == i)
                cout << "_";
            else if (cufarjos[1].x == j && cufarjos[1].y == i)
                cout << "[";
            else if (cufarjos[2].x == j && cufarjos[2].y == i)
                cout << " ";
            else if (cufarjos[3].x == j && cufarjos[3].y == i)
                cout << "'";
            else if (cufarjos[4].x == j && cufarjos[4].y == i)
                cout << " ";
            else if (cufarjos[5].x == j && cufarjos[5].y == i)
                cout << "]";
            else if (cufar2sus[1].x == j && cufar2sus[1].y == i && camera == 2)
                cout << "_";
            else if (cufar2sus[2].x == j && cufar2sus[2].y == i && camera == 2)
                cout << "_";
            else if (cufar2sus[3].x == j && cufar2sus[3].y == i && camera == 2)
                cout << "_";
            else if (cufar2jos[1].x == j && cufar2jos[1].y == i && camera == 2)
                cout << "[";
            else if (cufar2jos[2].x == j && cufar2jos[2].y == i && camera == 2)
                cout << " ";
            else if (cufar2jos[3].x == j && cufar2jos[3].y == i && camera == 2)
                cout << "'";
            else if (cufar2jos[4].x == j && cufar2jos[4].y == i && camera == 2)
                cout << " ";
            else if (cufar2jos[5].x == j && cufar2jos[5].y == i && camera == 2)
                cout << "]";
            else if (slash == 1 && i == player.y + 1 && j == player.x + 1 && oks == 0)
            {
                cout << "\\";
                oks = 1;
            }
            else if (slash == 2 && i == player.y && j == player.x + 1 && oks == 0)
            {
                cout << "--";
                oks = 2;
            }
            else if (slash == 3 && i == player.y - 1 && j == player.x + 1 && oks == 0)
            {
                cout << "/";
                oks = 1;
            }
            else if (slash == 4 && i == player.y - 1 && j == player.x && oks == 0)
            {
                cout << "|";
                oks = 1;
            }
            else if (slash == 5 && i == player.y - 1 && j == player.x - 1 && oks == 0)
            {
                cout << "\\";
                oks = 1;
            }
            else if (slash == 6 && i == player.y && j == player.x - 2 && oks == 0)
            {
                cout << "--";
                oks = 2;
            }
            else if (slash == 7 && i == player.y + 1 && j == player.x - 1 && oks == 0)
            {
                cout << "/";
                oks = 1;
            }
            else if (slash == 8 && i == player.y + 1 && j == player.x && oks == 0)
            {
                cout << "|";
                oks = 1;
            }
            //          "_____"
            //          "\___/"
            //          "|X X|"
            //       "___|_o_|___"
            //      "/ /|  _  |\ \"
            //     "/_/ | '_' | \_\"
            //     " #  | '_' |"
            //     " O   \___/"
            //          "_____"
            //          "\___/"
            //          "|' '|"
            //      "____|_-_|___"
            //     "|___|  _  |\ \"
            //         "| '_' | \_\"
            //         "| '_' |"
            //          "\___/"
            else if ((poveste == 20) && camera == 3 && boss[6][2].x == j && boss[6][2].y == i - 2)
                cout << "O";
            else if (poveste >= 10 && camera == 3 && boss[1][1].x == j && boss[1][1].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[1][2].x == j && boss[1][2].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[1][3].x == j && boss[1][3].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[1][4].x == j && boss[1][4].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[1][5].x == j && boss[1][5].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[2][1].x == j && boss[2][1].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[2][2].x == j && boss[2][2].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[2][3].x == j && boss[2][3].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[2][4].x == j && boss[2][4].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[2][5].x == j && boss[2][5].y == i)
                cout << "/";
            else if (poveste >= 10 && camera == 3 && boss[3][1].x == j && boss[3][1].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[3][2].x == j && boss[3][2].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[3][3].x == j && boss[3][3].y == i)
                cout << " ";
            else if (poveste >= 16 && camera == 3 && boss[3][4].x == j && boss[3][4].y == i)
                cout << "X";
            else if (poveste >= 10 && camera == 3 && boss[3][4].x == j && boss[3][4].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[3][5].x == j && boss[3][5].y == i)
                cout << "|";
            else if (poveste >= 15 && camera == 3 && boss[4][1].x == j + 1 && boss[4][1].y == i)
                cout << "_";
            else if (poveste >= 15 && camera == 3 && boss[4][1].x == j + 2 && boss[4][1].y == i - 1)
                cout << "|";
            else if (poveste >= 15 && camera == 3 && boss[4][1].x == j + 1 && boss[4][1].y == i - 1)
                cout << "_";
            else if (poveste >= 15 && camera == 3 && boss[4][1].x == j && boss[4][1].y == i - 1)
                cout << "_";
            else if (poveste >= 15 && camera == 3 && boss[4][1].x == j - 1 && boss[4][1].y == i - 1)
                cout << "_";
            else if (poveste >= 15 && camera == 3 && boss[6][1].x == j && boss[6][1].y == i)
                cout << " ";
            else if (poveste >= 15 && camera == 3 && boss[6][2].x == j && boss[6][2].y == i)
                cout << " ";
            else if (poveste >= 15 && camera == 3 && boss[6][3].x == j && boss[6][3].y == i)
                cout << " ";
            else if (poveste >= 15 && camera == 3 && boss[6][4].x == j && boss[6][4].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[4][1].x == j && boss[4][1].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][2].x == j && boss[4][2].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][3].x == j && boss[4][3].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][4].x == j && boss[4][4].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[4][5].x == j && boss[4][5].y == i)
                cout << "_";
            else if (poveste >= 16 && camera == 3 && boss[4][6].x == j && boss[4][6].y == i)
                cout << "o";
            else if (poveste >= 10 && camera == 3 && boss[4][6].x == j && boss[4][6].y == i)
                cout << "-";
            else if (poveste >= 10 && camera == 3 && boss[4][7].x == j && boss[4][7].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][8].x == j && boss[4][8].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[4][9].x == j && boss[4][9].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][10].x == j && boss[4][10].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[4][11].x == j && boss[4][11].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[5][1].x == j && boss[5][1].y == i)
                cout << "/";
            else if (poveste >= 10 && camera == 3 && boss[5][2].x == j && boss[5][2].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][3].x == j && boss[5][3].y == i)
                cout << "/";
            else if (poveste >= 10 && camera == 3 && boss[5][4].x == j && boss[5][4].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[5][5].x == j && boss[5][5].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][6].x == j && boss[5][6].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][7].x == j && boss[5][7].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[5][8].x == j && boss[5][8].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][9].x == j && boss[5][9].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][10].x == j && boss[5][10].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[5][11].x == j && boss[5][11].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[5][12].x == j && boss[5][12].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[5][13].x == j && boss[5][13].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[6][1].x == j && boss[6][1].y == i)
                cout << "/";
            else if (poveste >= 10 && camera == 3 && boss[6][2].x == j && boss[6][2].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[6][3].x == j && boss[6][3].y == i)
                cout << "/";
            else if (poveste >= 10 && camera == 3 && boss[6][4].x == j && boss[6][4].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[6][5].x == j && boss[6][5].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[6][6].x == j && boss[6][6].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[6][7].x == j && boss[6][7].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[6][8].x == j && boss[6][8].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[6][9].x == j && boss[6][9].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[6][10].x == j && boss[6][10].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[6][11].x == j && boss[6][11].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[6][12].x == j && boss[6][12].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[6][13].x == j && boss[6][13].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[6][14].x == j && boss[6][14].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[6][15].x == j && boss[6][15].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[7][1].x == j && boss[7][1].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[7][2].x == j && boss[7][2].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[7][3].x == j && boss[7][3].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[7][4].x == j && boss[7][4].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[7][5].x == j && boss[7][5].y == i)
                cout << "'";
            else if (poveste >= 10 && camera == 3 && boss[7][6].x == j && boss[7][6].y == i)
                cout << " ";
            else if (poveste >= 10 && camera == 3 && boss[7][7].x == j && boss[7][7].y == i)
                cout << "|";
            else if (poveste >= 10 && camera == 3 && boss[8][1].x == j && boss[8][1].y == i)
                cout << "\\";
            else if (poveste >= 10 && camera == 3 && boss[8][2].x == j && boss[8][2].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[8][3].x == j && boss[8][3].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[8][4].x == j && boss[8][4].y == i)
                cout << "_";
            else if (poveste >= 10 && camera == 3 && boss[8][5].x == j && boss[8][5].y == i)
                cout << "/";
            else if ((poveste == 12 || poveste == 13 || poveste == 14) && camera == 3 && boss[6][2].x == j && boss[6][2].y == i - 1)
                cout << "#";
            else if ((poveste == 12 || poveste == 14) && camera == 3 && boss[6][2].x == j && boss[6][2].y == i - 2)
                cout << "O";
            else if (poveste == 14 && camera == 3 && boss[6][2].x == j + 1 && boss[6][2].y == i - 2)
                cout << "{";
            else if (poveste == 14 && camera == 3 && boss[6][2].x == j - 1 && boss[6][2].y == i - 2)
                cout << "}";
            else if (poveste == 13 && camera == 3 && boss[6][2].x == j && boss[6][2].y == i - 2)
                cout << "X";
            else if ((fantoma1.x == j && fantoma1.y == i && fantoma1.HP == 1 || (fantoma2.x == j && fantoma2.y == i && fantoma2.HP == 1)) &&
                (camera == 2 || camera == 3))
            {
                cout << fantoma;
            }
            else if ((fantoma3.x == j && fantoma3.y == i && fantoma3.HP == 1 || (fantoma4.x == j && fantoma4.y == i && fantoma4.HP == 1)
                || (fantoma5.x == j && fantoma5.y == i && fantoma5.HP == 1)) && camera == 3)
            {
                cout << "&";
            }
            else if (p3.x == j && p3.y == i)
            {
                if (s3 == LEFT || s3 == RIGHT)
                    cout << "-";
                else cout << "|";
            }
            else if (p4.x == j && p4.y == i && (s4 == LEFT || s4 == RIGHT))
                cout << "-";
            else if (p4.x == j && p4.y == i)
                cout << "|";
            else if (p5.x == j && p5.y == i && (s5 == LEFT || s5 == RIGHT))
            {
                cout << "-";
            }
            else if (p5.x == j && p5.y == i)
                cout << "|";
            else if (oks == 2)
                oks = 1;
            else
                cout << " ";
        }
        if (camera == 1)
        {
            if (usa1 == 1 && (i == 5 || i == 7))
                cout << "=";
            else if (usa1 == 1 && i == 6)
                cout << " ";
            else cout << "|";
        }
        else if (camera == 2)
        {
            if (usa2sike == 1 && (i == 5 || i == 7))
                cout << "=";
            else if (usa2sike == 1 && i == 6)
                cout << " ";
            else cout << "|";
        }
        else cout << "|";
        if (poveste == 11)
        {
            if (i == 3)
                cout << "Stiu ca era sa mori de cateva ori, dar asta era singura";
            else if (i == 4)
                cout << "cale prin care puteam sa te testez";
            else if (i == 6)
                cout << "Naratorul te-a pus sa faci o alegere:";
            else if (i == 8)
                cout << "Ca sa alegi nemurirea, apasa \"y\"";
            else if (i == 10)
                cout << "Ca sa alegi razbunarea, apasa \"n\"";
        }
        if (poveste == 12)
        {
            if (i == 3)
                cout << "Il poti auzi pe celalalt razboinic cum te implora sa ii cruti viata.";
            else if (i == 4)
                cout << "Acesta iti sopteste incet: \"singurul lui punct slab e capul\".";
            else if (i == 5)
                cout << "Ai trecut prin atatea probe, chiar nu vrei sa obtii nimic?";
            else if (i == 7)
                cout << "Din nou, e timpul sa alegi:";
            else if (i == 9)
                cout << "Ca sa il distrugi pe Razboinic, apasa \"y\"";
            else if (i == 10)
                cout << "Ca sa il cruti pe Razboinic, apasa \"n\"";
        }
        cout << '\n';
    }
    for (i = 1; i <= width; i++) cout << "=";
    if (poveste == 14)
        cout << '\n' << '\n' << "      Multumesc, m-ai ajutat extrem de mult in experimentul meu. Acum este timpul sa" << '\n'
        << "       ii iei locul si sa asteptam sa vedem ce va alege urmatorul razboinic. In caz ca nu" << '\n' <<
        "       ai realizat, tot testul a fost o minciuna, dar poti gasi alinare in faptul ca " << '\n' <<
        "       mi-ai confirmat ca oamenii vor actiona mereu in propriul lor interes." << '\n' << '\n' << '\n' <<
        "       Ai aflat teribilul secret al Naratorului, dar cu ce pret?" << '\n' << '\n';
    else if (poveste == 16)
        cout << '\n' << '\n' << "      Ai ucis Naratorul cu propria lui sabie, genial! Dar stai, acum cine nareaza?" << '\n'
        << "      Putin conteaza, ai reusit sa pui capat tiraniei Naratorului si sa il impiedici din a" << '\n' <<
        "      ucide mai multi razboinici nevinovati. Dar poate ca erau mai multe secrete de aflat..." << '\n' << '\n';
    else if (poveste == 20)
        cout << '\n' << '\n' << "      Impresionant, ai reusit sa il inseli pe Narator si sa salvezi razboinicul, care iti lauda" << '\n'
        << "       curajul si agilitatea cu care ti-ai doborat toti inamicii. Ii multumesti pentru" << '\n' <<
        "       sfatul dat, dar un gand iti ramane fixat in minte: daca sunt mai multi Naratori ce trebuie distrusi?" << '\n' << '\n';
    if (!GameOver)
    {
        cout << '\n' << '\n' << "     VIATA EROULUI: ";
        //cout << '\n';
        //cout << "             ";
        for (i = 1; i <= player.HP; i++)
            cout << inima << " ";
        for (i = player.HP + 1; i <= HPt; i++)
            cout << "- ";
        cout << '\n' << '\n';
        cout << "         INVENTAR" << '\n' << '\n';
        if (sabie == 1)
            cout << "         --|=====>" << '\n';
        if (platosa == 1)
        {
            cout << "        ____   ____" << '\n';
            cout << "        \\   \\_/   /" << '\n';
            cout << "         |       |" << '\n';
            cout << "         |_______|" << '\n';
        }
    }
}
void Miscarefantome()
{
    if ((timp == 0 && camera != 3) || (camera == 3 && reload % 5 == 0 && poveste < 10))
    {
        if (camera == 2 || camera == 3)
        {
            if (fantoma1.HP > 0)
            {
                p = rand();
                if (p % 2 == 0)
                {
                    if (fantoma1.x < player.x && fantoma1.x != fantoma2.x - 1)
                        fantoma1.x++;
                    else if (fantoma1.y < player.y && fantoma1.y != fantoma2.y - 1)
                        fantoma1.y++;
                    else if (fantoma1.x > player.x && fantoma2.x != fantoma1.x - 1)
                        fantoma1.x--;
                    else if (fantoma1.y > player.y && fantoma2.y != fantoma1.y - 1)
                        fantoma1.y--;
                }
                else
                {
                    if (fantoma1.y < player.y && fantoma1.y != fantoma2.y - 1)
                        fantoma1.y++;
                    else if (fantoma1.x < player.x && fantoma1.x != fantoma2.x - 1)
                        fantoma1.x++;
                    else if (fantoma1.y > player.y && fantoma2.y != fantoma1.y - 1)
                        fantoma1.y--;
                    else if (fantoma1.x > player.x && fantoma2.x != fantoma1.x - 1)
                        fantoma1.x--;
                }
            }
            if (fantoma2.HP > 0)
            {
                p = rand();
                if (p % 2 == 0)
                {
                    if (fantoma2.y < player.y && fantoma2.y != fantoma1.y - 1)
                        fantoma2.y++;
                    else if (fantoma2.x < player.x && fantoma2.x != fantoma1.x - 1)
                        fantoma2.x++;
                    else if (fantoma2.y > player.y && fantoma1.y != fantoma2.y - 1)
                        fantoma2.y--;
                    else if (fantoma2.x > player.x && fantoma1.x != fantoma2.x - 1)
                        fantoma2.x--;
                }
                else
                {
                    if (fantoma2.x < player.x && fantoma2.x != fantoma1.x - 1)
                        fantoma2.x++;
                    else if (fantoma2.y < player.y && fantoma2.y != fantoma1.y - 1)
                        fantoma2.y++;
                    else if (fantoma2.x > player.x && fantoma1.x != fantoma2.x - 1)
                        fantoma2.x--;
                    else if (fantoma2.y > player.y && fantoma1.y != fantoma2.y - 1)
                        fantoma2.y--;
                }
            }
        }
        if (camera == 3)
        {
            if (fantoma3.HP > 0)
            {
                if (fantoma3.y > player.y && fantoma4.y != fantoma3.y - 1 && fantoma5.y != fantoma3.y - 1)
                    fantoma3.y--;
                else if (fantoma3.y < player.y && fantoma3.y != fantoma4.y - 1 && fantoma3.y != fantoma5.y - 1)
                    fantoma3.y++;
                else if (fantoma3.x < player.x + 10 && fantoma3.x < 24)
                    fantoma3.x++;
            }
            if (fantoma4.HP > 0)
            {
                if (fantoma4.y > player.y && fantoma3.y != fantoma4.y - 1 && fantoma5.y != fantoma4.y - 1)
                    fantoma4.y--;
                else if (fantoma4.y < player.y && fantoma4.y != fantoma3.y - 1 && fantoma4.y != fantoma5.y - 1)
                    fantoma4.y++;
                else if (fantoma4.x < player.x + 10 && fantoma4.x < 24)
                    fantoma4.x++;
            }
            if (fantoma5.HP > 0)
            {
                if (fantoma5.y > player.y && fantoma3.y != fantoma5.y - 1 && fantoma4.y != fantoma5.y - 1)
                    fantoma5.y--;
                else if (fantoma5.y < player.y && fantoma5.y != fantoma3.y - 1 && fantoma5.y != fantoma4.y - 1)
                    fantoma5.y++;
                else if (fantoma5.x < player.x + 10 && fantoma5.x < 24)
                    fantoma5.x++;
            }
        }
    }
    if (reload >= 20)
    {
        if (fantoma3.HP > 0 && camera == 3)
        {
            p3 = fantoma3;
            if (fantoma3.x == player.x)
            {
                if (fantoma3.y < player.y)
                    s3 = DOWN;
                else s3 = UP;
            }
            else
            {
                if (fantoma3.x < player.x)
                    s3 = RIGHT;
                else s3 = LEFT;
            }
        }
        if (fantoma4.HP > 0 && camera == 3)
        {
            p4 = fantoma4;
            if (fantoma4.x == player.x)
            {
                if (fantoma4.y < player.y)
                    s4 = DOWN;
                else s4 = UP;
            }
            else
            {
                if (fantoma4.x < player.x)
                    s4 = RIGHT;
                else s4 = LEFT;
            }
        }
        if (fantoma5.HP > 0 && camera == 3)
        {
            p5 = fantoma5;
            if (fantoma5.x == player.x)
            {
                if (fantoma5.y < player.y)
                    s5 = DOWN;
                else s5 = UP;
            }
            else
            {
                if (fantoma5.x < player.x)
                    s5 = RIGHT;
                else s5 = LEFT;
            }
        }
    }
    if (fantoma1.x == player.x && fantoma1.y == player.y && fantoma1.HP == 1 && (camera == 2 || camera == 3) ||
        (fantoma2.x == player.x && fantoma2.y == player.y && fantoma2.HP == 1 && (camera == 2 || camera == 3)) ||
        (fantoma3.x == player.x && fantoma3.y == player.y && fantoma3.HP == 1 && camera == 3) ||
        (fantoma4.x == player.x && fantoma4.y == player.y && fantoma4.HP == 1 && camera == 3) ||
        (fantoma5.x == player.x && fantoma5.y == player.y && fantoma5.HP == 1 && camera == 3))
    {
        player.HP--;
        if (player.HP == 0)
        {
            GameOver = true;
            lose = 1;
        }
        if (fantoma1.x == player.x && fantoma1.y == player.y && fantoma1.HP == 1 && (camera == 2 || camera == 3))
        {
            fantoma1.HP = 0;
            fantoma1.x = 0;
            fantoma1.y = 0;
        }
        else if (fantoma2.x == player.x && fantoma2.y == player.y && fantoma2.HP == 1 && (camera == 2 || camera == 3))
        {
            fantoma2.HP = 0;
            fantoma2.x = 0;
            fantoma2.y = 0;
        }
        else if (fantoma3.x == player.x && fantoma3.y == player.y && fantoma3.HP == 1 && camera == 3)
        {
            fantoma3.HP = 0;
            fantoma3.x = 0;
            fantoma3.y = 0;
        }
        else if (fantoma4.x == player.x && fantoma4.y == player.y && fantoma4.HP == 1 && camera == 3)
        {
            fantoma4.HP = 0;
            fantoma4.x = 0;
            fantoma4.y = 0;
        }
        else
        {
            fantoma5.HP = 0;
            fantoma5.x = 0;
            fantoma5.y = 0;
        }
    }
    if (reload >= 20)
        reload = 0;
}
void Atac()
{
    {
        if (sabie == 1)
        {
            if (poveste == 3)
            {
                poveste = 4;
                usa1 = 1;
            }
            slash = 1;
            oks = 0;
            if (player.x == fantoma1.x - 1 && player.y == fantoma1.y - 1 &&
                (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if (player.x == fantoma2.x - 1 && player.y == fantoma2.y - 1 &&
                (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if (player.x == fantoma3.x - 1 && player.y == fantoma3.y - 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if (player.x == fantoma4.x - 1 && player.y == fantoma4.y - 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if (player.x == fantoma5.x - 1 && player.y == fantoma5.y - 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
			Sleep(1);
            slash = 2;
            oks = 0;
            if ((player.x == fantoma1.x - 1 || player.x == fantoma1.x - 2)
                && player.y == fantoma1.y &&
                (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x - 1 || player.x == fantoma2.x - 2)
                && player.y == fantoma2.y && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x - 1 || player.x == fantoma3.x - 2)
                && player.y == fantoma3.y && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x - 1 || player.x == fantoma4.x - 2)
                && player.y == fantoma4.y && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x - 1 || player.x == fantoma5.x - 2)
                && player.y == fantoma5.y && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
            Sleep(1);
            slash = 3;
            oks = 0;
            if ((player.x == fantoma1.x - 1)
                && player.y == fantoma1.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x - 1)
                && player.y == fantoma2.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x - 1)
                && player.y == fantoma3.y + 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x - 1)
                && player.y == fantoma4.y + 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x - 1)
                && player.y == fantoma5.y + 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
            Sleep(1);
            slash = 4;
            oks = 0;
            if ((player.x == fantoma1.x)
                && player.y == fantoma1.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x)
                && player.y == fantoma2.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x)
                && player.y == fantoma3.y + 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x)
                && player.y == fantoma4.y + 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x)
                && player.y == fantoma5.y + 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
            Sleep(1);
            slash = 5;
            oks = 0;
            if ((player.x == fantoma1.x + 1)
                && player.y == fantoma1.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x + 1)
                && player.y == fantoma2.y + 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x + 1)
                && player.y == fantoma3.y + 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x + 1)
                && player.y == fantoma4.y + 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x + 1)
                && player.y == fantoma5.y + 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
            Sleep(1);
            slash = 6;
            oks = 0;
            if ((player.x == fantoma1.x + 1 || player.x == fantoma1.x + 2)
                && player.y == fantoma1.y && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x + 1 || player.x == fantoma2.x + 2)
                && player.y == fantoma2.y && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x + 1 || player.x == fantoma3.x + 2)
                && player.y == fantoma3.y && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x + 1 || player.x == fantoma4.x + 2)
                && player.y == fantoma4.y && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x + 1 || player.x == fantoma5.x + 2)
                && player.y == fantoma5.y && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            slash = 7;
            oks = 0;
            if ((player.x == fantoma1.x + 1)
                && player.y == fantoma1.y - 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x + 1)
                && player.y == fantoma2.y - 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x + 1)
                && player.y == fantoma3.y - 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x + 1)
                && player.y == fantoma4.y - 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x + 1)
                && player.y == fantoma5.y - 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
			Sleep(1);
            slash = 8;
            oks = 0;
            if ((player.x == fantoma1.x)
                && player.y == fantoma1.y - 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma1.HP = 0;
                fantoma1.x = 0;
                fantoma1.y = 0;
            }
            if ((player.x == fantoma2.x)
                && player.y == fantoma2.y - 1 && (camera == 2 || (poveste == 9 && camera == 3)))
            {
                fantoma2.HP = 0;
                fantoma2.x = 0;
                fantoma2.y = 0;
            }
            if ((player.x == fantoma3.x)
                && player.y == fantoma3.y - 1 && camera == 3)
            {
                fantoma3.HP = 0;
                fantoma3.x = 0;
                fantoma3.y = 0;
            }
            if ((player.x == fantoma4.x)
                && player.y == fantoma4.y - 1 && camera == 3)
            {
                fantoma4.HP = 0;
                fantoma4.x = 0;
                fantoma4.y = 0;
            }
            if ((player.x == fantoma5.x)
                && player.y == fantoma5.y - 1 && camera == 3)
            {
                fantoma5.HP = 0;
                fantoma5.x = 0;
                fantoma5.y = 0;
            }
            Draw();
            slash = 0;
            oks = 0;
            timp = 0;
            Miscarefantome();
            timp = 1;
        }
    }
}
void Input()
{
    dir = STOP;
    if (camera == 3 && (poveste < 10 || poveste >= 15))
    {
        while (timp <= 1000 && !_kbhit())
        {
            timp++;
        }
        if (_kbhit)
        {
            timp++;
        }
        if (timp > 1000)
        {
            timp = 0;
            reload++;
        }
    }
    else
    {
        while (timp <= 15000 && !_kbhit())
        {
            timp++;
        }
        if (_kbhit)
        {
            timp++;
        }
        if (timp > 15000)
            timp = 0;
    }
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a': if (poveste == 1) poveste = 2; dir = LEFT; break;
        case 'w': if (poveste == 1) poveste = 2; dir = UP; break;
        case 's': if (poveste == 1) poveste = 2; dir = DOWN; break;
        case 'd': if (poveste == 1) poveste = 2; dir = RIGHT; break;
        case ' ': dir = SLASH; break;
        case 'y':
        {
            if (poveste == 11)
                poveste = 12;
            else if (poveste == 12)
            {
                while (player.y > boss[6][2].y + 2)
                {
                    player.y--;
                    Draw();
                    u = 0;
                    while (u < 300000000)
                        u++;
                }
                while (player.y < boss[6][2].y + 2)
                {
                    player.y++;
                    Draw();
                    u = 0;
                    while (u < 300000000)
                        u++;
                }
                while (player.x < boss[6][2].x - 2)
                {
                    player.x++;
                    Draw();
                    u = 0;
                    while (u < 300000000)
                        u++;
                }
                poveste = 13;
                Atac();
                u = 0;
                while (u < 300000000)
                    u++;
                poveste = 14;
                GameOver = true;
                lose = 1;
                Draw();
            }
        } break;
        case 'n':
        {
            if (poveste == 11)
            {
                poveste = 15;
                while (player.y > 6)
                {
                    player.y--;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                while (player.y < 6)
                {
                    player.y++;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                while (player.x < 14)
                {
                    player.x++;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                while (player.x > 14)
                {
                    player.x--;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                p3.x = 25;
                p3.y = player.y - 1;
                s3 = LEFT;
                p4.x = 25;
                p4.y = player.y;
                s4 = LEFT;
                p5.x = 25;
                p5.y = player.y + 1;
                s5 = LEFT;
                Draw();
                while (player.x > 5 || player.y > 4)
                {
                    u = 1;
                    while (u <= 100000000)
                    {
                        if (u % 50000000 == 0)
                        {
                            player.x--;
                            Draw();
                        }
                        if (u % 33333333 == 0)
                        {
                            p3.x--;
                            p4.x--;
                            p5.x--;
                            Draw();
                        }
                        if ((player.x == 10 && player.y == 6) || (player.x == 6 && player.y == 5))
                        {
                            player.y--;
                            Draw();
                        }
                        u++;
                    }
                }
                dagger[1].x = player.x + 1;
                dagger[2].x = player.x + 2;
                dagger[3].x = player.x + 3;
                dagger[4].x = player.x + 4;
                dagger[5].x = player.x + 5;
                dagger[1].y = 4;
                dagger[2].y = 4;
                dagger[3].y = 4;
                dagger[4].y = 4;
                dagger[5].y = 4;
                while (dagger[5].x < boss[3][2].x)
                {
                    if (p3.x > 1)
                    {
                        p3.x--;
                        p4.x--;
                        p5.x--;
                    }
                    Draw();
                    dagger[1].x++;
                    dagger[2].x++;
                    dagger[3].x++;
                    dagger[4].x++;
                    dagger[5].x++;
                }
                poveste = 16;
                GameOver = true;
                Draw();
            }
            else if (poveste == 12)
            {
                while (player.y > 9)
                {
                    player.y--;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                while (player.y < 9)
                {
                    player.y++;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                while (player.x < 25)
                {
                    player.x++;
                    Draw();
                    u = 0;
                    while (u < 200000000)
                        u++;
                }
                u = 0;
                while (u < 1000000000)
                    u++;
                while (player.x > 22)
                {
                    player.x--;
                    Draw();
                    u = 0;
                    while (u < 1000000)
                        u++;
                }
                while (player.y > 4)
                {
                    player.y--;
                    Draw();
                    u = 0;
                    while (u < 100000)
                        u++;
                }
                dagger[1].x = player.x + 1;
                dagger[2].x = player.x + 2;
                dagger[3].x = player.x + 3;
                dagger[4].x = player.x + 4;
                dagger[5].x = player.x + 5;
                dagger[1].y = 4;
                dagger[2].y = 4;
                dagger[3].y = 4;
                dagger[4].y = 4;
                dagger[5].y = 4;
                while (dagger[5].x < boss[3][2].x)
                {
                    Draw();
                    dagger[1].x++;
                    dagger[2].x++;
                    dagger[3].x++;
                    dagger[4].x++;
                    dagger[5].x++;
                    u = 0;
                    while (u < 5000000)
                        u++;
                }
                poveste = 20;
                GameOver = true;
                Draw();
            }
        }
        }
    }
}
void Logic()
{
    switch (dir)
    {
    case LEFT: if (player.x > 2) player.x--;
             else if (usa1 == 1 && camera == 2)
    {
        if (player.x == 2)
        {
            if (player.y == 6)
            {
                camera = 1;
                player.x = 24;
                width = 25;
            }
        }
        else player.x++;
    }
             else if (usa2 == 1 && camera == 3)
    {
        if (player.x == 2)
        {
            if (player.y == 6)
            {
                camera = 2;
                player.x = 24;
                width = 25;
            }
        }
        else player.x++;
    }
             break;
    case UP: if (player.y > 2) player.y--; break;
    case RIGHT: if (player.x < 24 || (poveste >= 15 && player.x < width - 1)) player.x++;
              else if (usa1 == 1 && camera == 1)
    {
        if (player.x == width - 1)
        {
            if (poveste == 4)
                poveste = 5;
            if (player.y == 6)
            {
                camera = 2;
                player.x = 2;
                width = 25;
            }
        }
        else player.x++;
    }
              else if (usa2 == 1 && camera == 2)
    {
        if (player.x == width - 1)
        {
            if (poveste == 8)
            {
                poveste = 9;
                usa2 = 0;
                fantoma1.HP = 1;
                fantoma1.x = 17;
                fantoma1.y = 5;
                fantoma2.HP = 1;
                fantoma2.x = 17;
                fantoma2.y = 7;
                reload = 30;
            }
            if (player.y == 6)
            {
                camera = 3;
                player.x = 2;
            }
        }
        else player.x++;
    }
              break;
    case DOWN: if (player.y < height - 1) player.y++; break;
    case SLASH: Atac(); break;
    }
    if (fantoma1.HP == 0 && fantoma2.HP == 0)
    {
        usa2sike = 1;
        if (poveste == 5)
            poveste = 6;
    }
    if (camera == 2 && player.y == 6 && player.x == 24 && fantoma1.HP == 0 && fantoma2.HP == 0 && platosa == 0)
    {
        cufar2sus[1].x = 12;
        cufar2sus[1].y = 5;
        cufar2sus[2].x = 13;
        cufar2sus[2].y = 5;
        cufar2sus[3].x = 14;
        cufar2sus[3].y = 5;
        cufar2jos[1].x = 11;
        cufar2jos[1].y = 6;
        cufar2jos[2].x = 12;
        cufar2jos[2].y = 6;
        cufar2jos[3].x = 13;
        cufar2jos[3].y = 6;
        cufar2jos[4].x = 14;
        cufar2jos[4].y = 6;
        cufar2jos[5].x = 15;
        cufar2jos[5].y = 6;
        if (poveste == 6)
            poveste = 7;
    }
    if (camera == 2 &&
        (player.x == cufar2sus[1].x && player.y == cufar2sus[1].y ||
            player.x == cufar2sus[2].x && player.y == cufar2sus[2].y ||
            player.x == cufar2sus[3].x && player.y == cufar2sus[3].y ||
            player.x == cufar2jos[1].x && player.y == cufar2jos[1].y ||
            player.x == cufar2jos[2].x && player.y == cufar2jos[2].y ||
            player.x == cufar2jos[3].x && player.y == cufar2jos[3].y ||
            player.x == cufar2jos[4].x && player.y == cufar2jos[4].y ||
            player.x == cufar2jos[5].x && player.y == cufar2jos[5].y))
    {
        cufar2sus[1].x = 0;
        cufar2sus[1].y = 0;
        cufar2sus[2].x = 0;
        cufar2sus[2].y = 0;
        cufar2sus[3].x = 0;
        cufar2sus[3].y = 0;
        cufar2jos[1].x = 0;
        cufar2jos[1].y = 0;
        cufar2jos[2].x = 0;
        cufar2jos[2].y = 0;
        cufar2jos[3].x = 0;
        cufar2jos[3].y = 0;
        cufar2jos[4].x = 0;
        cufar2jos[4].y = 0;
        cufar2jos[5].x = 0;
        cufar2jos[5].y = 0;
        platosa = 1;
        if (poveste == 7)
            poveste = 8;
        player.HP = player.HP + 4;
        HPt = HPt + 4;
        usa2 = 1;
    }
    if (player.x == cufarsus[1].x && player.y == cufarsus[1].y ||
        player.x == cufarsus[2].x && player.y == cufarsus[2].y ||
        player.x == cufarsus[3].x && player.y == cufarsus[3].y ||
        player.x == cufarjos[1].x && player.y == cufarjos[1].y ||
        player.x == cufarjos[2].x && player.y == cufarjos[2].y ||
        player.x == cufarjos[3].x && player.y == cufarjos[3].y ||
        player.x == cufarjos[4].x && player.y == cufarjos[4].y ||
        player.x == cufarjos[5].x && player.y == cufarjos[5].y)
    {
        cufarsus[1].x = 0;
        cufarsus[1].y = 0;
        cufarsus[2].x = 0;
        cufarsus[2].y = 0;
        cufarsus[3].x = 0;
        cufarsus[3].y = 0;
        cufarjos[1].x = 0;
        cufarjos[1].y = 0;
        cufarjos[2].x = 0;
        cufarjos[2].y = 0;
        cufarjos[3].x = 0;
        cufarjos[3].y = 0;
        cufarjos[4].x = 0;
        cufarjos[4].y = 0;
        cufarjos[5].x = 0;
        cufarjos[5].y = 0;
        sabie = 1;
        poveste = 3;
    }
    Miscarefantome();
    if (p3.x == player.x && p3.y == player.y)
    {
        if (poveste == 15)
            player.HP = 0;
        else
            player.HP--;
        if (player.HP == 0)
        {
            GameOver = true;
            lose = 1;
        }
        p3.x = 0;
        p3.y = 0;
    }
    if (p4.x == player.x && p4.y == player.y)
    {
        if (poveste == 15)
            player.HP = 0;
        else
            player.HP--;
        if (player.HP == 0)
        {
            GameOver = true;
            lose = 1;
        }
        p4.x = 0;
        p4.y = 0;
    }
    if (p5.x == player.x && p5.y == player.y)
    {
        if (poveste == 15)
            player.HP = 0;
        else
            player.HP--;
        if (player.HP == 0)
        {
            GameOver = true;
            lose = 1;
        }
        p5.x = 0;
        p5.y = 0;
    }
    switch (s3)
    {
    case UP: p3.y--; break;
    case DOWN: p3.y++; break;
    case LEFT: p3.x--; break;
    case RIGHT: p3.x++; break;
    }
    switch (s4)
    {
    case UP: p4.y--; break;
    case DOWN: p4.y++; break;
    case LEFT: p4.x--; break;
    case RIGHT: p4.x++; break;
    }
    switch (s5)
    {
    case UP: p5.y--; break;
    case DOWN: p5.y++; break;
    case LEFT: p5.x--; break;
    case RIGHT: p5.x++; break;
    }
    if (fantoma1.HP == 0 && fantoma2.HP == 0 && fantoma3.HP == 0 && fantoma4.HP == 0 && fantoma5.HP == 0
        && camera == 3 && poveste == 9)
    {
        poveste = 10;
        while (width <= 40)
        {
            width++;
            while (u < 1000)
                u++;
            u = 0;
            Draw();
        }
        poveste = 11;
    }
}
int main()
{
    Intro();
    if (GameOver)
        cout << '\n' << '\n' << '\n' << '\n' << '\n' << '\n'
        << '\n' << '\n' << '\n' << "                  Felicitari, ai reusit sa pierzi jocul inainte sa-l incepi, impresionant" << '\n';
    else
    {
        SetUp();
        Draw();
        while (!GameOver)
        {
            if (!GameOver)
                Input();
            if (!GameOver)
                Logic();
            if (!GameOver)
                Draw();
        }
        if (lose == 1)
        {
            if (poveste != 14)
                cout << '\n' << '\n' << "         Ai pierdut";
        }
        else
            cout << '\n' << '\n' << "       Ai castigat";
        cout << '\n';
    }
    return 0;
}