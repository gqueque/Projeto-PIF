#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "../include/logica_do_jogo.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/fantasmas.h"
#include "../include/expressoes_logicas.h"

#define MAP_LINHAS (SCRENDY - SCRSTARTY - 1)
#define MAP_COLUNAS (SCRENDX - SCRSTARTX - 1)

typedef struct {
    int x, y;
    int pontos;
} Player;

typedef struct {
    int x, y;
    int expected_value;
    bool ativo;
} LogicItem;

static Player jogador;
static LogicItem logic_items[4];
static int current_step = 0;
static int vidas = 3;
static int nivel = 1;
static int multiplicador = 1;
static LogicalExpression expressao;

void salvar_ranking(const char* nome, int pontos) {
    FILE* f = fopen("ranking.txt", "a");
    if (f) {
        fprintf(f, "%s %d\n", nome, pontos);
        fclose(f);
    }
}

void mostrar_ranking() {
    FILE* f = fopen("ranking.txt", "r");
    if (!f) {
        screenGotoxy(5, 22);
        printf("Sem ranking salvo ainda.");
        return;
    }

    // Limpa a área do ranking antes de imprimir
    for (int linha = 22; linha < MAXY - 1; linha++) {
        screenGotoxy(5, linha);
        printf("                                        ");
    }

    screenGotoxy(5, 22);
    printf("=== RANKING ===");
    char nome[50];
    int pontos;
    int linha = 23;

    while (fscanf(f, "%s %d", nome, &pontos) == 2 && linha < MAXY - 1) {
        screenGotoxy(5, linha++);
        printf("%s - %d pontos", nome, pontos);
    }

    fclose(f);
}


static char** criarMapa() {
    char** mapa = malloc(MAP_LINHAS * sizeof(char*));
    for (int i = 0; i < MAP_LINHAS; i++) {
        mapa[i] = malloc(MAP_COLUNAS * sizeof(char));
        for (int j = 0; j < MAP_COLUNAS; j++) {
            mapa[i][j] = ' ';
        }
    }
    return mapa;
}

static void destruirMapa(char** mapa) {
    for (int i = 0; i < MAP_LINHAS; i++)
        free(mapa[i]);
    free(mapa);
}

static bool ocupado(int x, int y) {
    for (int i = 0; i < 4; i++) {
        if (logic_items[i].ativo && logic_items[i].x == x && logic_items[i].y == y) {
            return true;
        }
    }
    return false;
}

static void place_logic_items(LogicalExpression expr) {
    for (int i = 0; i < 4; i++) {
        int x, y;
        do {
            x = rand() % (MAP_COLUNAS - 2) + 1;
            y = rand() % (MAP_LINHAS - 2) + 1;
        } while (ocupado(x, y) || (x == jogador.x && y == jogador.y));

        logic_items[i].x = x;
        logic_items[i].y = y;
        logic_items[i].expected_value = expr.truth_table[i];
        logic_items[i].ativo = true;
    }
    current_step = 0;
}

static void draw_expression(LogicalExpression expr) {
    screenSetColor(LIGHTCYAN, BLACK);
    screenGotoxy(SCRSTARTX + 2, SCRSTARTY);
    printf("Resolvam: %s", expr.expr_str);
}

static void draw_logic_items() {
    screenSetColor(WHITE, BLACK);
    for (int i = 0; i < 4; i++) {
        if (logic_items[i].ativo) {
            screenGotoxy(SCRSTARTX + 1 + logic_items[i].x, SCRSTARTY + 1 + logic_items[i].y);
            printf("%c", logic_items[i].expected_value ? 'V' : 'F');
        }
    }
}

static void renderizarCena(char** mapa, Player* jogador, bool atualiza_msg) {
    for (int i = 0; i < MAP_LINHAS; i++) {
        for (int j = 0; j < MAP_COLUNAS; j++) {
            screenGotoxy(SCRSTARTX + 1 + j, SCRSTARTY + 1 + i);
            printf(" ");
        }
    }

    draw_logic_items();
    desenhar_fantasmas(SCRSTARTX, SCRSTARTY);

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(SCRSTARTX + 1 + jogador->x, SCRSTARTY + 1 + jogador->y);
    printf("🥠");

    screenGotoxy(SCRSTARTX + 2, SCRENDY);
    screenSetColor(CYAN, BLACK);
    printf("Pontuação: %d  x%d", jogador->pontos, multiplicador);

    screenGotoxy(SCRSTARTX + 25, SCRENDY);
    screenSetColor(LIGHTRED, BLACK);
    printf("Vidas: %d", vidas);

    if (atualiza_msg) {
        screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
        printf("                              ");
        screenGotoxy(SCRSTARTX + 2, SCRENDY + 2);
        printf("                              ");
    }

    screenUpdate();
}

static void moverJogador(int tecla, Player* jogador) {
    int novoX = jogador->x;
    int novoY = jogador->y;

    switch (tecla) {
        case 'w': novoY--; break;
        case 's': novoY++; break;
        case 'a': novoX--; break;
        case 'd': novoX++; break;
    }

    if (novoX >= 0 && novoX < MAP_COLUNAS && novoY >= 0 && novoY < MAP_LINHAS) {
        jogador->x = novoX;
        jogador->y = novoY;
    }
}

static void check_logic_collision(Player* jogador, LogicalExpression expr, bool* precisa_render) {
    for (int i = 0; i < 4; i++) {
        if (logic_items[i].ativo &&
            logic_items[i].x == jogador->x &&
            logic_items[i].y == jogador->y) {

            *precisa_render = true;

            if (logic_items[i].expected_value == expr.truth_table[current_step]) {
                logic_items[i].ativo = false;
                current_step++;
                jogador->pontos += 100 * multiplicador;
                multiplicador++;

                screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
                screenSetColor(GREEN, BLACK);
                printf("✔ Correto!");

                if (current_step == 4) {
                    screenGotoxy(SCRSTARTX + 2, SCRENDY + 2);
                    printf("Expressão resolvida com sucesso!");

                    nivel++;
                    if (nivel > 3) {
                        screenGotoxy(SCRSTARTX + 2, SCRENDY + 3);
                        printf("Parabéns, Guga! Você zerou o jogo!");
                        sleep(3);
                        return;
                    } else {
                        expressao = get_random_expression(nivel);
                        draw_expression(expressao);
                        place_logic_items(expressao);
                        current_step = 0;
                    }
                }
            } else {
                multiplicador = 1;
                screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
                screenSetColor(RED, BLACK);
                printf("✘ Ordem errada! Reiniciando...");

                place_logic_items(expr);
                current_step = 0;
                vidas--;
            }
            break;
        }
    }
}

void iniciar_jogo() {
    char nome[50];
    screenInit(1);
    screenHideCursor();
    screenGotoxy(10, 10);
    screenSetColor(WHITE, BLACK);
    printf("Digite seu nome: ");
    scanf("%49s", nome);
    keyboardInit();
    timerInit(100);

    jogador.x = MAP_COLUNAS / 2;
    jogador.y = MAP_LINHAS / 2;
    jogador.pontos = 0;
    vidas = 3;
    nivel = 1;
    multiplicador = 1;

    char** mapa = criarMapa();

    expressao = get_random_expression(nivel);
    draw_expression(expressao);
    place_logic_items(expressao);
    inicializar_fantasmas(MAP_COLUNAS, MAP_LINHAS);

    int tecla = 0;
    long tempo = 0;
    bool precisa_render = true;

    while (tecla != 'q' && tempo <= 1000 && vidas > 0) {
        if (precisa_render) {
            renderizarCena(mapa, &jogador, true);
            precisa_render = false;
        }

        if (keyhit()) {
            tecla = readch();
            moverJogador(tecla, &jogador);
            precisa_render = true;
            check_logic_collision(&jogador, expressao, &precisa_render);
        }

        if (timerTimeOver()) {
            tempo++;
            mover_fantasmas_para_jogador(jogador.x, jogador.y, MAP_COLUNAS, MAP_LINHAS);
            checar_colisao_fantasmas(&jogador.x, &jogador.y, &vidas, &precisa_render, MAP_COLUNAS, MAP_LINHAS, &multiplicador);
            precisa_render = true;
        }
    }

    salvar_ranking(nome, jogador.pontos);
    mostrar_ranking();
    screenUpdate();
    sleep(5); 

    destruirMapa(mapa);
    screenShowCursor();
    keyboardDestroy();
    timerDestroy();
    screenDestroy();
}
