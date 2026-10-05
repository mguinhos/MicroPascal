#include "micropascal-tests/testes.h"

#include <stdio.h>

int main(void)
{
    ResultadoTeste resultado_testes;
    resultado_testes.total = 0;
    resultado_testes.passaram = 0;
    resultado_testes.falharam = 0;

    printf("Executando testes do lexer\n");
    runLexerTests(&resultado_testes);

    printf("Executando testes do parser\n");
    runParserTests(&resultado_testes);

    printf("\nTotal: %d | Passaram: %d | Falharam: %d\n",
        resultado_testes.total,
        resultado_testes.passaram,
        resultado_testes.falharam);

    if (resultado_testes.falharam > 0) {
        return 1;
    }
    return 0;
}
