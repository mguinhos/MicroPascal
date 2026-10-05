#include "micropascal-parser/ast.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void astLiberar(AstNo* no);
void astImprimir(const AstNo* no, int profundidade);

const char* astKindNome(AstKind kind)
{
    switch (kind) {
        case AST_PROGRAMA:
            return "Programa";
        case AST_SECAO_VAR:
            return "SecaoVar";
        case AST_DECL_VAR:
            return "DeclVar";
        case AST_TIPO:
            return "Tipo";
        case AST_BLOCO:
            return "Bloco";
        case AST_LISTA_COMANDOS:
            return "ListaComandos";
        case AST_ATRIBUICAO:
            return "Atribuicao";
        case AST_ITERACAO:
            return "Iteracao";
        case AST_DECISAO:
            return "Decisao";
        case AST_ESCRITA:
            return "Escrita";
        case AST_EXPRESSAO_BINARIA:
            return "ExpressaoBinaria";
        case AST_EXPRESSAO_UNARIA:
            return "ExpressaoUnaria";
        case AST_EXPRESSAO_PARENTESES:
            return "ExpressaoParenteses";
        case AST_EXPRESSAO_LITERAL:
            return "ExpressaoLiteral";
        case AST_EXPRESSAO_IDENTIFICADOR:
            return "ExpressaoIdentificador";
        default:
            return "Desconhecido";
    }
}

void astLiberar(AstNo* no)
{
    if (no == NULL) {
        return;
    }

    astLiberar(no->esquerda);
    astLiberar(no->direita);
    astLiberar(no->terceiro);
    astLiberar(no->proximo);

    if (no->valor != NULL) {
        free(no->valor);
    }

    free(no);
}

void astImprimir(const AstNo* no, int profundidade)
{
    int i;
    const AstNo* atual;

    for (atual = no; atual != NULL; atual = atual->proximo) {
        for (i = 0; i < profundidade; i++) {
            printf("  ");
        }

        printf("%s", astKindNome(atual->kind));

        if (atual->valor != NULL) {
            printf(" (%s)", atual->valor);
        }

        printf("\n");
        astImprimir(atual->esquerda, profundidade + 1);
        astImprimir(atual->direita, profundidade + 1);
        astImprimir(atual->terceiro, profundidade + 1);
    }
}
