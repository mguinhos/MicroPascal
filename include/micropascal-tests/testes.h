#ifndef MICROPASCAL_TESTS_TESTES_H
#define MICROPASCAL_TESTS_TESTES_H

#include <stdio.h>

typedef struct ResultadoTeste {
    int total;
    int passaram;
    int falharam;
} ResultadoTeste;

void runLexerTests(ResultadoTeste* resultado_testes);
void runParserTests(ResultadoTeste* resultado_testes);

#define TESTE_ASSERT(resultado_testes, condicao, descricao) \
    do { \
        (resultado_testes)->total++; \
        if (condicao) { \
            (resultado_testes)->passaram++; \
        } else { \
            (resultado_testes)->falharam++; \
            printf("FALHOU: %s (linha %d)\n", descricao, __LINE__); \
        } \
    } while (0)

#endif
