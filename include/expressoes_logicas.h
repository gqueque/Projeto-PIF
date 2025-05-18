// expressoes_logicas.h
#ifndef EXPRESOES_LOGICAS_H
#define EXPRESOES_LOGICAS_H

typedef struct {
    char expr_str[20];
    int truth_table[4];
} LogicalExpression;

LogicalExpression get_random_expression(int nivel);

#endif
