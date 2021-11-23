#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <mmsystem.h>
//*********************
#define GRID 10
#define VSL5 5
#define VSL4 4
#define VSL3 3
#define VSL2 2
#define VSL1 1
#define CORNER
//*********************

using namespace std;
bool music;

char grille[GRID][GRID];                                                                                    //On crée la grille de jeu

bool Played = PlaySound("Music_fond.wav", NULL, SND_ASYNC );

int main() {
    //************************ Creation and cout of the grid

    cout << Played;

    for (int y = 0; y < GRID; y++) {                                                                        //Avec cette boucle for j'initialise toutes les cases du jeu avec un point en commencant par les y
        for (int x = 0; x < GRID; x++) {                                                                    //Avec cette boucle for j'initialise toutes les cases x du jeu avec un point
            grille[x][y] = '.';

        }
    }

    for (int y = 0; y < GRID; y++) {                                                                        //Avec cette boucle for j'affiche toutes les cases du jeu
        for (int x = 0; x < GRID; x++) {                                                                    //Avec cette boucle for j'affiche toutes les cases x du jeu
            cout << grille[x][y] << " ";
            Beep(12000, 15);
            Sleep(5);
        }
        cout << endl;
    }
}