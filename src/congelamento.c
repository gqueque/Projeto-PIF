#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "congelamento.h"
#include "screen.h"

bool fantasmasCongelados = false;
time_t tempoCongelamento = 0;

void gerar_item_congelamento(ItemCongelante *item) {
    srand(time(NULL));
    item->x = 3 + rand() % 30;
    item->y = 3 + rand() % 15;
    item->ativo = true;
}


void desenhar_item_congelamento(const ItemCongelante *item) {
    if (item->ativo) {
        screenSetColor(LIGHTBLUE, BLACK);
        screenGotoxy(SCRSTARTX + 1 + item->x, SCRSTARTY + 1 + item->y);
        printf("🥶"); 
    }
}


void checar_congelamento(ItemCongelante *item, int jogadorX, int jogadorY) {
    if (item->ativo && jogadorX == item->x && jogadorY == item->y) {
        fantasmasCongelados = true;
        tempoCongelamento = time(NULL);
        item->ativo = false;

        screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
        screenSetColor(CYAN, BLACK);
        printf("🥶 Fantasmas congelados por 5s!");
    }
}


void atualizar_estado_congelamento() {
    if (fantasmasCongelados) {
        time_t agora = time(NULL);
        if (difftime(agora, tempoCongelamento) >= 5.0) {
            fantasmasCongelados = false;

            screenGotoxy(SCRSTARTX + 2, SCRENDY + 2);
            screenSetColor(WHITE, BLACK);
            printf("Fantasmas descongelados!      ");
        }
    }
}
