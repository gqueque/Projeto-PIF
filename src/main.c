#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "../include/cli.h"

#define WIDTH  20
#define HEIGHT 10

char map[HEIGHT][WIDTH + 1] = {
    "///////////////////",
    "/................./",
    "/.///./////.///././",
    "/................./",
    "/.///././//././//./",
    "/...../..C../...../",
    "/.///././//././//./",
    "/................./",
    "/.///./////.///././",
    "///////////////////"
};

int pacX = 9, pacY = 5;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawTitle() {
    gotoxy(0, 0);
    printf("+-----------------------+\n");
    gotoxy(0, 1);
    printf("|     PAC-MAN ASCII     |\n");
    gotoxy(0, 2);
    printf("+-----------------------+\n");
    gotoxy(0, 3);
    printf("Use W A S D pra mover. Pressione Q pra sair.\n");
}

void drawMap() {
    clrscr();
    drawTitle();

    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i + 5);
        for (int j = 0; j < WIDTH; j++) {
            char tile = map[i][j];
            switch (tile) {
                case '/':
                    printf("/");  // parede
                    break;
                case '.':
                    printf(".");  // ponto
                    break;
                case 'C':
                    printf("C");  // pacman
                    break;
                default:
                    printf(" ");  // espaço vazio
            }
        }
    }
}

void movePacman(int dx, int dy) {
    int newX = pacX + dx;
    int newY = pacY + dy;

    if (map[newY][newX] == '/' || newX < 0 || newY < 0 || newX >= WIDTH || newY >= HEIGHT)
        return;

    map[pacY][pacX] = ' ';
    pacX = newX;
    pacY = newY;
    map[pacY][pacX] = 'C';
}

int main() {
    cli_setup();
    drawMap();

    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'w') movePacman(0, -1);
            else if (ch == 's') movePacman(0, 1);
            else if (ch == 'a') movePacman(-1, 0);
            else if (ch == 'd') movePacman(1, 0);
            else if (ch == 'q') break;
            drawMap();
        }
    }

    cli_cleanup();
    clrscr();
    gotoxy(0, 0);
    printf("Jogo encerrado. Valeu por jogar!\n");
    return 0;
}
