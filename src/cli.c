#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include "cli.h"

void cli_setup() {
    system("cls");
}

void cli_cleanup() {
   
}

void clrscr() {
    system("cls");
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
