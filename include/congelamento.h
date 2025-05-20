#ifndef CONGELAMENTO_H
#define CONGELAMENTO_H

#include <stdbool.h>
#include <time.h>


typedef struct {
    int x, y;
    bool ativo;
} ItemCongelante;


extern bool fantasmasCongelados;
extern time_t tempoCongelamento;


void gerar_item_congelamento(ItemCongelante *item);
void desenhar_item_congelamento(const ItemCongelante *item);
void checar_congelamento(ItemCongelante *item, int jogadorX, int jogadorY);
void atualizar_estado_congelamento();

#endif
