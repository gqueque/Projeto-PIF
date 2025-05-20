
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/expressoes_logicas.h"

static LogicalExpression faceis[] = {
    {"A ∧ B",      {0, 0, 0, 1}},
    {"A ∨ B",      {0, 1, 1, 1}},
};

static LogicalExpression medias[] = {
    {"A ∧ ¬B",     {0, 0, 1, 0}},
    {"¬A ∨ B",     {1, 1, 0, 1}},
    {"(A ∧ B) ∨ A",{0, 0, 1, 1}},
};

static LogicalExpression dificieis[] = {
    {"¬(A ∨ B)",         {1, 0, 0, 0}},
    {"(A ∧ B) ∨ ¬A" ,     {1, 1, 0, 1}},
    {"¬(A ∧ B) ∧ B",     {0, 1, 0, 0}},
};


LogicalExpression get_random_expression(int nivel) {
    srand(time(NULL)); 

    int index;
    if (nivel == 1) {
        index = rand() % (sizeof(faceis)/sizeof(faceis[0]));
        return faceis[index];
    } else if (nivel == 2) {
        index = rand() % (sizeof(medias)/sizeof(medias[0]));
        return medias[index];
    } else {
        index = rand() % (sizeof(dificieis)/sizeof(dificieis[0]));
        return dificieis[index];
    }
}
