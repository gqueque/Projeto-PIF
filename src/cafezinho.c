#include <stdio.h>
#include <stdlib.h>
#include "cafezinho.h"
#include "screen.h"

void gerar_item_cafe(CoffeeItem* item, int maxCols, int maxLinhas, int playerX, int playerY) {
    int x, y;
    do {
        x = rand() % (maxCols - 2) + 1;
        y = rand() % (maxLinhas - 2) + 1;
    } while (x == playerX && y == playerY);
    item->x = x;
    item->y = y;
    item->ativo = true;
}

void desenhar_item_cafe(CoffeeItem* item) {
    if (!item->ativo) return;
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(SCRSTARTX + 1 + item->x, SCRSTARTY + 1 + item->y);
    printf("☕");
}

bool checar_colisao_cafe(CoffeeItem* item, int playerX, int playerY) {
    if (item->ativo && item->x == playerX && item->y == playerY) {
        item->ativo = false;
        return true;
    }
    return false;
}
