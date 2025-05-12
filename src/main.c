#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "../include/cli.h"

#define MAX_NAME 30
#define MAX_PLAYERS 100
#define WIDTH  24
#define HEIGHT 12

typedef struct {
    char nome[MAX_NAME];
    int pontos;
} Player;

Player jogador;
int score = 0;
int tempo_restante = 20;

char map[HEIGHT][WIDTH + 1] = {
    "[][][][][][][][][][][][]",
    "[]....................[]",
    "[]...////.......////..[]",
    "[]....................[]",
    "[].////.....////......[]",
    "[].......C............[]",
    "[].....////.....////..[]",
    "[]....................[]",
    "[].////.......////....[]",
    "[]....................[]",
    "[][][][][][][][][][][][]"
};

int pacX = 9, pacY = 5;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawTitle() {
    gotoxy(0, 0);
    printf("+-----------------------------+\n");
    gotoxy(0, 1);
    printf("|     PAC-MAN DO PITUXO       |\n");
    gotoxy(0, 2);
    printf("+-----------------------------+\n");
    gotoxy(0, 3);
    printf("Use W A S D pra mover. Q pra sair.\n");
    gotoxy(0, 4);
    printf("Pontos: %d     Tempo: %ds", score, tempo_restante);
}

void drawMap() {
    clrscr();
    drawTitle();
    for (int i = 0; i < HEIGHT; i++) {
        gotoxy(0, i + 5);
        for (int j = 0; j < WIDTH; j++) {
            char tile = map[i][j];
            switch (tile) {
                case '/': printf("/"); break;
                case '_': printf("_"); break;
                case '[': printf("["); break;
                case ']': printf("]"); break;
                case '.': printf("."); break;
                case 'C': printf("C"); break;
                default: printf(" ");
            }
        }
    }
}

void movePacman(int dx, int dy) {
    int newX = pacX + dx;
    int newY = pacY + dy;
    if (map[newY][newX] == '[' || map[newY][newX] == ']' || map[newY][newX] == '/' || newX < 0 || newY < 0 || newX >= WIDTH || newY >= HEIGHT)
        return;
    if (map[newY][newX] == '.') {
        score += 10;
    }
    map[pacY][pacX] = ' ';
    pacX = newX;
    pacY = newY;
    map[pacY][pacX] = 'C';
}

int main() {
    printf("Digite seu nome: ");
    fgets(jogador.nome, MAX_NAME, stdin);
    jogador.nome[strcspn(jogador.nome, "\n")] = 0;
    time_t start_time = time(NULL);

    cli_setup();
    drawMap();

    while (1) {
        tempo_restante = 20 - (int)difftime(time(NULL), start_time);
        if (tempo_restante <= 0) break;

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
    jogador.pontos = score;
    printf("Jogo encerrado. Seu Score final foi de: %d\n", jogador.pontos);

    if (jogador.pontos > 0) {
        FILE *arq = fopen("ranking.txt", "a");
        if (arq != NULL) {
            fprintf(arq, "%s %d\n", jogador.nome, jogador.pontos);
            fclose(arq);
        }
    }

    printf("\nRanking:\n");

    Player ranking[MAX_PLAYERS];
    int count = 0;

    FILE *read = fopen("ranking.txt", "r");
    if (read != NULL) {
        char linha[100];
        while (fgets(linha, sizeof(linha), read)) {
            char *last_space = strrchr(linha, ' ');
            if (last_space != NULL) {
                *last_space = '\0';
                int pontos = atoi(last_space + 1);
                strncpy(ranking[count].nome, linha, MAX_NAME - 1);
                ranking[count].nome[MAX_NAME - 1] = '\0';
                ranking[count].pontos = pontos;
                count++;
                if (count >= MAX_PLAYERS) break;
            }
        }
        fclose(read);

        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (ranking[j].pontos > ranking[i].pontos) {
                    Player temp = ranking[i];
                    ranking[i] = ranking[j];
                    ranking[j] = temp;
                }
            }
        }

        for (int i = 0; i < count && i < 10; i++) {
            printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontos);
        }
    }

    return 0;
}
