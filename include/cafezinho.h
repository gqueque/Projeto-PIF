#ifndef CAFEZINHO_H
#define CAFEZINHO_H

#include <stdbool.h>

typedef struct {
    int x, y;
    bool ativo;
} CoffeeItem;


void gerar_item_cafe(CoffeeItem* item, int maxCols, int maxLinhas, int playerX, int playerY);


void desenhar_item_cafe(CoffeeItem* item);


bool checar_colisao_cafe(CoffeeItem* item, int playerX, int playerY);

#endif 
