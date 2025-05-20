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
#include "../include/tela_de_etapa.h"
#include "../include/congelamento.h"
#include "cafezinho.h"


#define MAP_LINHAS (SCRENDY - SCRSTARTY - 1)
#define MAP_COLUNAS (SCRENDX - SCRSTARTX - 1)
#define MAX_ENTRADAS 100

typedef struct {
    char texto1[100];
    char texto2[100];
    time_t expiracao;
    bool ativa;
} MensagemTemporaria;

typedef struct {
    char nome[50];
    int pontos;
} EntradaRanking;

typedef struct {
    int x, y;
    int pontos;
} Player;

typedef struct {
    int x, y;
    int expected_value;
    bool ativo;
} LogicItem;

// Variáveis globais
static MensagemTemporaria msgTemp;
static Player jogador;
static LogicItem logic_items[4];
static int current_step = 0;
static int vidas = 3;
static int nivel = 1;
static int multiplicador = 1;
static LogicalExpression expressao;
static ItemCongelante itemCongelante;
static CoffeeItem cafezinho;



void exibir_mensagem_temporaria(const char* linha1, const char* linha2, int duracao_segundos) {
    strncpy(msgTemp.texto1, linha1, sizeof(msgTemp.texto1));
    strncpy(msgTemp.texto2, linha2, sizeof(msgTemp.texto2));
    msgTemp.expiracao = time(NULL) + duracao_segundos;
    msgTemp.ativa = true;
}

void salvar_ranking(const char* nome, int pontos) {
    FILE* f = fopen("ranking.txt", "a");
    if (f) {
        fprintf(f, "%s %d\n", nome, pontos);
        fclose(f);
    }
}

int comparar(const void* a, const void* b) {
    EntradaRanking* r1 = (EntradaRanking*)a;
    EntradaRanking* r2 = (EntradaRanking*)b;
    return r2->pontos - r1->pontos; 
}

void mostrar_ranking() {
    EntradaRanking entradas[MAX_ENTRADAS];
    int count = 0;

    FILE* f = fopen("ranking.txt", "r");
    if (!f) {
        screenGotoxy(5, 4);
        printf("Sem ranking salvo ainda.");
        return;
    }

    while (fscanf(f, "%s %d", entradas[count].nome, &entradas[count].pontos) == 2 && count < MAX_ENTRADAS)
        count++;
    fclose(f);

    qsort(entradas, count, sizeof(EntradaRanking), comparar);

    screenGotoxy(5, 4);
    printf("=== RANKING ===");

    for (int i = 0; i < count && i < 10; i++) {
        screenGotoxy(5, 5 + i);
        printf("%d° %s - %d pontos", i + 1, entradas[i].nome, entradas[i].pontos);
    }
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
    for (int i = 0; i < 4; i++)
        if (logic_items[i].ativo && logic_items[i].x == x && logic_items[i].y == y)
            return true;
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
    printf("Resolva: %s", expr.expr_str);
}

static void draw_logic_items() {
    screenSetColor(WHITE, BLACK);
    for (int i = 0; i < 4; i++)
        if (logic_items[i].ativo) {
            screenGotoxy(SCRSTARTX + 1 + logic_items[i].x, SCRSTARTY + 1 + logic_items[i].y);
            printf("%c", logic_items[i].expected_value ? 'V' : 'F');
        }
}

static void renderizarCena(char** mapa, Player* jogador, bool atualiza_msg) {
    for (int i = 0; i < MAP_LINHAS; i++)
        for (int j = 0; j < MAP_COLUNAS; j++) {
            screenGotoxy(SCRSTARTX + 1 + j, SCRSTARTY + 1 + i);
            printf(" ");
        }

    draw_logic_items();
    desenhar_item_congelamento(&itemCongelante);
    desenhar_item_cafe(&cafezinho);
    desenhar_fantasmas(SCRSTARTX, SCRSTARTY);

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(SCRSTARTX + 1 + jogador->x, SCRSTARTY + 1 + jogador->y);
    printf("🥐");

    screenSetColor(CYAN, BLACK);
    screenGotoxy(SCRSTARTX + 2, SCRENDY);
    printf("Pontuação: %d  x%d", jogador->pontos, multiplicador);

    screenSetColor(LIGHTRED, BLACK);
    screenGotoxy(SCRSTARTX + 25, SCRENDY);
    printf("Vidas: %d", vidas);

    if (msgTemp.ativa) {
        time_t agora = time(NULL);
        if (agora <= msgTemp.expiracao) {
            screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
            screenSetColor(GREEN, BLACK);
            printf("%-30s", msgTemp.texto1);

            screenGotoxy(SCRSTARTX + 2, SCRENDY + 2);
            printf("%-30s", msgTemp.texto2);
        } else {
            msgTemp.ativa = false;
            screenGotoxy(SCRSTARTX + 2, SCRENDY + 1);
            printf("                              ");
            screenGotoxy(SCRSTARTX + 2, SCRENDY + 2);
            printf("                              ");
        }
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

static void check_logic_collision(Player* jogador,LogicalExpression expr,bool* precisa_render) {
    if (checar_colisao_cafe(&cafezinho, jogador->x, jogador->y)) {
        vidas++;
        exibir_mensagem_temporaria("☕ Vida extra!", "", 2);
        *precisa_render = true;
        return;  // sai cedo, sem processar tabela lógica
    }
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

                exibir_mensagem_temporaria("✔ Correto!", "", 2);

                if (current_step == 4) {
                    exibir_mensagem_temporaria("Expressão resolvida!", "Avançando fase...", 2);
                    nivel++;
                    if (nivel > 3) {
                        screenGotoxy(SCRSTARTX + 2, SCRENDY + 3);
                        printf("Parabéns! Você zerou o jogo!");
                        sleep(2);
                        vidas = 0;
                        return;
                    } else {
                        mostrar_tela_fase(nivel);
                        expressao = get_random_expression(nivel);
                        draw_expression(expressao);
                        place_logic_items(expressao);
                        gerar_item_congelamento(&itemCongelante);
                        gerar_item_cafe(&cafezinho,MAP_COLUNAS,MAP_LINHAS,jogador->x,jogador->y);
                    }
                }
            } else {
                multiplicador = 1;
                exibir_mensagem_temporaria("✘ Ordem errada!", "Reiniciando...", 3);
                place_logic_items(expr);
                gerar_item_congelamento(&itemCongelante);
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
    gerar_item_congelamento(&itemCongelante);
    gerar_item_cafe(&cafezinho, MAP_COLUNAS, MAP_LINHAS, jogador.x, jogador.y);
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
            checar_congelamento(&itemCongelante, jogador.x, jogador.y);
            precisa_render = true;
            check_logic_collision(&jogador, expressao, &precisa_render);
        }

        if (timerTimeOver()) {
            tempo++;
            atualizar_estado_congelamento();
            if (!fantasmasCongelados) {
                mover_fantasmas_para_jogador(jogador.x, jogador.y, MAP_COLUNAS, MAP_LINHAS);
            }
            checar_colisao_fantasmas(&jogador.x, &jogador.y, &vidas, &precisa_render, MAP_COLUNAS, MAP_LINHAS, &multiplicador);
            precisa_render = true;
        }
    }
        


    salvar_ranking(nome, jogador.pontos);

    screenClear();
    screenSetColor(CYAN, BLACK);
    screenGotoxy(5, 2);
    printf("🎮 Fim de jogo, %s! Sua pontuação final: %d", nome, jogador.pontos);

    mostrar_ranking();

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(5, MAXY - 2);
    printf("Pressione ENTER para sair...");
    screenShowCursor();
    
    screenUpdate();

    
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);

    
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);


    destruirMapa(mapa);
    keyboardDestroy();
    timerDestroy();
    screenDestroy();
}
