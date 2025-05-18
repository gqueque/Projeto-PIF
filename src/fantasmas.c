#include <stdio.h>
#include <stdlib.h>
#include "../include/fantasmas.h"
#include "../include/screen.h"

#define NUM_FANTASMAS 2

static int contador_lentidao = 0;
static const int FANTASMA_DELAY = 6;
static int origem_x[NUM_FANTASMAS];
static int origem_y[NUM_FANTASMAS];

static Fantasma fantasmas[NUM_FANTASMAS];

void inicializar_fantasmas(int largura, int altura) {
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        fantasmas[i].y = 2 + i * 3;
        fantasmas[i].x = rand() % (largura - 2) + 1;
        fantasmas[i].direcao = (i % 2 == 0) ? 1 : -1;

        origem_x[i] = fantasmas[i].x;
        origem_y[i] = fantasmas[i].y;
    }
}
//teste
void mover_fantasmas_para_jogador(int jogador_x, int jogador_y, int largura, int altura) {
    contador_lentidao++;
    if (contador_lentidao < FANTASMA_DELAY) return;
    contador_lentidao = 0;

    for (int i = 0; i < NUM_FANTASMAS; i++) {
        if (fantasmas[i].x < jogador_x) fantasmas[i].x++;
        else if (fantasmas[i].x > jogador_x) fantasmas[i].x--;

        if (fantasmas[i].y < jogador_y) fantasmas[i].y++;
        else if (fantasmas[i].y > jogador_y) fantasmas[i].y--;

        if (fantasmas[i].x < 1) fantasmas[i].x = 1;
        if (fantasmas[i].x >= largura - 1) fantasmas[i].x = largura - 2;
        if (fantasmas[i].y < 1) fantasmas[i].y = 1;
        if (fantasmas[i].y >= altura - 1) fantasmas[i].y = altura - 2;
    }
}

void desenhar_fantasmas(int offset_x, int offset_y) {
    screenSetColor(RED, BLACK);
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        screenGotoxy(offset_x + 1 + fantasmas[i].x, offset_y + 1 + fantasmas[i].y);
        printf("👻");
    }
}

void checar_colisao_fantasmas(int* jogador_x, int* jogador_y, int* vidas, bool* precisa_render, int largura, int altura, int* multiplicador) {
    for (int i = 0; i < NUM_FANTASMAS; i++) {
        if (fantasmas[i].x == *jogador_x && fantasmas[i].y == *jogador_y) {
            (*vidas)--;
            *multiplicador = 1;
            *jogador_x = largura / 2;
            *jogador_y = altura / 2;
            *precisa_render = true;

            screenGotoxy(3, 24);
            screenSetColor(RED, BLACK);
            printf("💀 Você perdeu 1 vida! Vidas restantes: %d", *vidas);

            fantasmas[i].x = origem_x[i];
            fantasmas[i].y = origem_y[i];
        }
    }
}
