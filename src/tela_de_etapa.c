#include <stdio.h>
#include <unistd.h>
#include "../include/screen.h"
#include "../include/tela_de_etapa.h"



void mostrar_tela_fase(int fase) {
    screenClear();
    screenSetColor(WHITE, BLACK);
    screenDrawBox(SCRSTARTX, SCRSTARTY, SCRENDX, SCRENDY);
    screenSetColor(LIGHTCYAN, BLACK);
    screenGotoxy(30, 10);

    switch (fase) {
        case 2:
            printf("🌟 FASE 2  🌟");
            break;
        case 3:
            printf("🔥 FASE FINAL 🔥");
            break;
        default:
            printf("🌱 FASE 1 🌱");
    }

    screenGotoxy(30, 12);
    screenSetColor(YELLOW, BLACK);
    printf("Prepare-se...");
    screenUpdate();
    sleep(2);
}
