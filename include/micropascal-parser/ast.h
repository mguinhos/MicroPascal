#ifndef MICROPASCAL_PARSER_AST_H
#define MICROPASCAL_PARSER_AST_H

typedef enum AstKind {
    AST_PROGRAMA,
    AST_SECAO_VAR,
    AST_DECL_VAR,
    AST_TIPO,
    AST_BLOCO,
    AST_LISTA_COMANDOS,
    AST_ATRIBUICAO,
    AST_ITERACAO,
    AST_DECISAO,
    AST_ESCRITA,
    AST_EXPRESSAO_BINARIA,
    AST_EXPRESSAO_UNARIA,
    AST_EXPRESSAO_PARENTESES,
    AST_EXPRESSAO_LITERAL,
    AST_EXPRESSAO_IDENTIFICADOR
} AstKind;

typedef struct AstNo {
    AstKind kind;
    char* valor;
    struct AstNo* esquerda;
    struct AstNo* direita;
    struct AstNo* terceiro;
    struct AstNo* proximo;
} AstNo;

const char* astKindNome(AstKind kind);

#endif
