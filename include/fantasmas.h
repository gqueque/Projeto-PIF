#ifndef FANTASMAS_H
#define FANTASMAS_H

#include <stdbool.h>

typedef struct {
    int x, y;
    int direcao;
    int origem_x, origem_y;
} Fantasma;

void inicializar_fantasmas(int largura, int altura);
void mover_fantasmas_para_jogador(int jogador_x, int jogador_y, int largura, int altura);
void desenhar_fantasmas(int offset_x, int offset_y);
void checar_colisao_fantasmas(int* jogador_x, int* jogador_y, int* vidas, bool* precisa_render, int largura, int altura, int* multiplicador);

#endif
