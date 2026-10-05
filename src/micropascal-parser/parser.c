#include "micropascal-parser/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AstNo* parserCriarNo(AstKind kind, const char* valor)
{
    AstNo* no = (AstNo*)malloc(sizeof(AstNo));

    no->kind = kind;

    if (valor != NULL) {
        no->valor = (char*)malloc(strlen(valor) + 1);
        strcpy(no->valor, valor);
    } else {
        no->valor = NULL;
    }

    no->esquerda = NULL;
    no->direita = NULL;
    no->terceiro = NULL;
    no->proximo = NULL;

    return no;
}

void parserAvancar(Parser* parser)
{
    tokenLiberar(&parser->atual);
    parser->atual = lexerProximoToken(parser->lexer);
}

void parserInicializar(Parser* parser, Lexer* lexer)
{
    parser->lexer = lexer;
    parser->atual.kind = TOKEN_EOF;
    parser->atual.lexema = NULL;
    parser->atual.linha = 0;
    parser->atual.coluna = 0;
    parser->houve_erro = 0;
    parser->total_erros = 0;
    parser->silencioso = 0;

    parserAvancar(parser);
}

void parserSilenciarErros(Parser* parser, int silenciar)
{
    parser->silencioso = silenciar;
}

void parserErro(Parser* parser)
{
    const char* lexema = parser->atual.lexema;

    if (lexema == NULL) {
        lexema = "";
    }

    parser->houve_erro = 1;
    parser->total_erros++;

    if (!parser->silencioso) {
        printf("Erro de sintaxe no token [%s] (linha %d, coluna %d)\n", lexema, parser->atual.linha, parser->atual.coluna);
    }
}

int parserVerificar(Parser* parser, TokenKind kind)
{
    return parser->atual.kind == kind;
}

int parserConsumir(Parser* parser, TokenKind kind)
{
    if (parserVerificar(parser, kind)) {
        parserAvancar(parser);
        return 1;
    }

    return 0;
}

int parserEsperar(Parser* parser, TokenKind kind)
{
    if (parserConsumir(parser, kind)) {
        return 1;
    }

    parserErro(parser);

    return 0;
}

int parserEmInicioDeComando(Parser* parser)
{
    switch (parser->atual.kind) {
        case TOKEN_BEGIN:
        case TOKEN_IDENTIFICADOR:
        case TOKEN_WHILE:
        case TOKEN_REPEAT:
        case TOKEN_IF:
        case TOKEN_WRITE:
            return 1;
        default:
            return 0;
    }
}

AstNo* parserAnalisarBloco(Parser* parser);
AstNo* parserAnalisarComando(Parser* parser);
AstNo* parserAnalisarExpressao(Parser* parser);

AstNo* parserAnalisarTipo(Parser* parser)
{
    switch (parser->atual.kind) {
        case TOKEN_INTEGER:
            parserAvancar(parser);

            return parserCriarNo(AST_TIPO, "integer");
        case TOKEN_REAL:
            parserAvancar(parser);

            return parserCriarNo(AST_TIPO, "real");
        case TOKEN_CHAR:
            parserAvancar(parser);

            return parserCriarNo(AST_TIPO, "char");
        default:
            parserErro(parser);

            return NULL;
    }
}

AstNo* parserAnalisarDeclVar(Parser* parser)
{
    AstNo* decl = parserCriarNo(AST_DECL_VAR, NULL);
    AstNo* ultimo = NULL;
    AstNo* identificador;
    AstNo* tipo;

    if (!parserEsperar(parser, TOKEN_IDENTIFICADOR)) {
        astLiberar(decl);
        return NULL;
    }

    identificador = parserCriarNo(AST_EXPRESSAO_IDENTIFICADOR, NULL);
    decl->esquerda = identificador;
    ultimo = identificador;

    while (parserVerificar(parser, TOKEN_VIRGULA)) {
        parserAvancar(parser);

        if (!parserEsperar(parser, TOKEN_IDENTIFICADOR)) {
            return decl;
        }

        ultimo->proximo = parserCriarNo(AST_EXPRESSAO_IDENTIFICADOR, NULL);
        ultimo = ultimo->proximo;
    }

    if (!parserEsperar(parser, TOKEN_DOIS_PONTOS)) {
        return decl;
    }

    tipo = parserAnalisarTipo(parser);
    decl->direita = tipo;

    if (!parserEsperar(parser, TOKEN_PONTO_VIRGULA)) {
        return decl;
    }

    return decl;
}

AstNo* parserAnalisarSecaoVar(Parser* parser)
{
    AstNo* secao = parserCriarNo(AST_SECAO_VAR, NULL);
    AstNo* ultimo = NULL;

    if (!parserEsperar(parser, TOKEN_VAR)) {
        astLiberar(secao);
        return NULL;
    }

    while (parserVerificar(parser, TOKEN_IDENTIFICADOR)) {
        AstNo* decl = parserAnalisarDeclVar(parser);

        if (decl == NULL) {
            break;
        }

        if (ultimo == NULL) {
            secao->esquerda = decl;
        } else {
            ultimo->proximo = decl;
        }

        ultimo = decl;
    }

    return secao;
}

AstNo* parserAnalisarExprBasica(Parser* parser)
{
    AstNo* no;

    switch (parser->atual.kind) {
        case TOKEN_ABRE_PARENTESES:
            parserAvancar(parser);
            no = parserCriarNo(AST_EXPRESSAO_PARENTESES, NULL);
            no->esquerda = parserAnalisarExpressao(parser);
            parserEsperar(parser, TOKEN_FECHA_PARENTESES);

            return no;
        case TOKEN_NOT:
            parserAvancar(parser);
            no = parserCriarNo(AST_EXPRESSAO_UNARIA, "not");
            no->esquerda = parserAnalisarExprBasica(parser);

            return no;
        case TOKEN_INTEIRO_LITERAL:
            no = parserCriarNo(AST_EXPRESSAO_LITERAL, parser->atual.lexema);
            parserAvancar(parser);

            return no;
        case TOKEN_REAL_LITERAL:
            no = parserCriarNo(AST_EXPRESSAO_LITERAL, parser->atual.lexema);
            parserAvancar(parser);

            return no;
        case TOKEN_CHAR_LITERAL:
            no = parserCriarNo(AST_EXPRESSAO_LITERAL, parser->atual.lexema);
            parserAvancar(parser);

            return no;
        case TOKEN_IDENTIFICADOR:
            no = parserCriarNo(AST_EXPRESSAO_IDENTIFICADOR, parser->atual.lexema);
            parserAvancar(parser);

            return no;
        default:
            parserErro(parser);

            return NULL;
    }
}

AstNo* parserAnalisarExpMul(Parser* parser)
{
    AstNo* esquerda = parserAnalisarExprBasica(parser);

    while (parser->atual.kind == TOKEN_MULTIPLICACAO || parser->atual.kind == TOKEN_DIVISAO || parser->atual.kind == TOKEN_DIV) {
        char operador[8];
        AstNo* direita;
        AstNo* novo;

        strcpy(operador, parser->atual.lexema);
        parserAvancar(parser);
        direita = parserAnalisarExprBasica(parser);
        novo = parserCriarNo(AST_EXPRESSAO_BINARIA, operador);
        novo->esquerda = esquerda;
        novo->direita = direita;
        esquerda = novo;
    }

    return esquerda;
}

AstNo* parserAnalisarExpAd(Parser* parser)
{
    AstNo* esquerda = parserAnalisarExpMul(parser);

    while (parser->atual.kind == TOKEN_MAIS || parser->atual.kind == TOKEN_MENOS) {
        char operador[8];
        AstNo* direita;
        AstNo* novo;

        strcpy(operador, parser->atual.lexema);
        parserAvancar(parser);
        direita = parserAnalisarExpMul(parser);
        novo = parserCriarNo(AST_EXPRESSAO_BINARIA, operador);
        novo->esquerda = esquerda;
        novo->direita = direita;
        esquerda = novo;
    }

    return esquerda;
}

AstNo* parserAnalisarExpRel(Parser* parser)
{
    AstNo* esquerda = parserAnalisarExpAd(parser);

    while (parser->atual.kind == TOKEN_IGUAL || parser->atual.kind == TOKEN_DIFERENTE || parser->atual.kind == TOKEN_MENOR || parser->atual.kind == TOKEN_MAIOR || parser->atual.kind == TOKEN_MENOR_IGUAL || parser->atual.kind == TOKEN_MAIOR_IGUAL) {
        char operador[8];
        AstNo* direita;
        AstNo* novo;

        strcpy(operador, parser->atual.lexema);
        parserAvancar(parser);
        direita = parserAnalisarExpAd(parser);
        novo = parserCriarNo(AST_EXPRESSAO_BINARIA, operador);
        novo->esquerda = esquerda;
        novo->direita = direita;
        esquerda = novo;
    }

    return esquerda;
}

AstNo* parserAnalisarExpOrAnd(Parser* parser)
{
    AstNo* esquerda = parserAnalisarExpRel(parser);

    while (parser->atual.kind == TOKEN_OR || parser->atual.kind == TOKEN_AND) {
        char operador[8];
        AstNo* direita;
        AstNo* novo;

        strcpy(operador, parser->atual.lexema);
        parserAvancar(parser);
        direita = parserAnalisarExpRel(parser);
        novo = parserCriarNo(AST_EXPRESSAO_BINARIA, operador);
        novo->esquerda = esquerda;
        novo->direita = direita;
        esquerda = novo;
    }

    return esquerda;
}

AstNo* parserAnalisarExpressao(Parser* parser)
{
    return parserAnalisarExpOrAnd(parser);
}

AstNo* parserAnalisarAtribuicao(Parser* parser)
{
    AstNo* no = parserCriarNo(AST_ATRIBUICAO, parser->atual.lexema);

    parserAvancar(parser);

    if (!parserEsperar(parser, TOKEN_ATRIBUICAO)) {
        return no;
    }

    no->esquerda = parserAnalisarExpressao(parser);
    parserEsperar(parser, TOKEN_PONTO_VIRGULA);

    return no;
}

AstNo* parserAnalisarIteracao(Parser* parser)
{
    AstNo* no;

    if (parser->atual.kind == TOKEN_WHILE) {
        parserAvancar(parser);
        no = parserCriarNo(AST_ITERACAO, "while");
        no->esquerda = parserAnalisarExpressao(parser);
        parserEsperar(parser, TOKEN_DO);
        no->direita = parserAnalisarComando(parser);

        return no;
    }

    parserAvancar(parser);
    no = parserCriarNo(AST_ITERACAO, "repeat");
    no->esquerda = parserAnalisarComando(parser);
    parserEsperar(parser, TOKEN_UNTIL);
    no->direita = parserAnalisarExpressao(parser);
    parserEsperar(parser, TOKEN_PONTO_VIRGULA);

    return no;
}

AstNo* parserAnalisarDecisao(Parser* parser)
{
    AstNo* no;

    parserAvancar(parser);
    no = parserCriarNo(AST_DECISAO, NULL);
    no->esquerda = parserAnalisarExpressao(parser);
    parserEsperar(parser, TOKEN_THEN);
    no->direita = parserAnalisarComando(parser);

    if (parserVerificar(parser, TOKEN_ELSE)) {
        parserAvancar(parser);
        no->terceiro = parserAnalisarComando(parser);
    }

    return no;
}

AstNo* parserAnalisarEscrita(Parser* parser)
{
    AstNo* no;

    parserAvancar(parser);
    no = parserCriarNo(AST_ESCRITA, NULL);
    parserEsperar(parser, TOKEN_ABRE_PARENTESES);
    no->esquerda = parserAnalisarExpressao(parser);
    parserEsperar(parser, TOKEN_FECHA_PARENTESES);
    parserEsperar(parser, TOKEN_PONTO_VIRGULA);

    return no;
}

AstNo* parserAnalisarComando(Parser* parser)
{
    switch (parser->atual.kind) {
        case TOKEN_BEGIN: {
            AstNo* bloco = parserAnalisarBloco(parser);

            parserEsperar(parser, TOKEN_PONTO_VIRGULA);

            return bloco;
        }
        case TOKEN_IDENTIFICADOR:
            return parserAnalisarAtribuicao(parser);
        case TOKEN_WHILE:
        case TOKEN_REPEAT:
            return parserAnalisarIteracao(parser);
        case TOKEN_IF:
            return parserAnalisarDecisao(parser);
        case TOKEN_WRITE:
            return parserAnalisarEscrita(parser);
        default:
            parserErro(parser);

            return NULL;
    }
}

AstNo* parserAnalisarBloco(Parser* parser)
{
    AstNo* bloco = parserCriarNo(AST_BLOCO, NULL);
    AstNo* lista = parserCriarNo(AST_LISTA_COMANDOS, NULL);
    AstNo* ultimo = NULL;

    if (!parserEsperar(parser, TOKEN_BEGIN)) {
        astLiberar(bloco);
        astLiberar(lista);
        return NULL;
    }

    while (parserEmInicioDeComando(parser)) {
        AstNo* comando = parserAnalisarComando(parser);

        if (comando == NULL) {
            break;
        }

        if (ultimo == NULL) {
            lista->esquerda = comando;
        } else {
            ultimo->proximo = comando;
        }

        ultimo = comando;
    }

    parserEsperar(parser, TOKEN_END);
    bloco->esquerda = lista;

    return bloco;
}

AstNo* parserAnalisarPrograma(Parser* parser)
{
    AstNo* programa = parserCriarNo(AST_PROGRAMA, NULL);

    if (!parserEsperar(parser, TOKEN_PROGRAM)) {
        astLiberar(programa);
        return NULL;
    }

    if (parserVerificar(parser, TOKEN_IDENTIFICADOR)) {
        free(programa->valor);
        programa->valor = (char*)malloc(strlen(parser->atual.lexema) + 1);
        strcpy(programa->valor, parser->atual.lexema);
        parserAvancar(parser);
    } else {
        parserErro(parser);
    }

    parserEsperar(parser, TOKEN_PONTO_VIRGULA);

    if (parserVerificar(parser, TOKEN_VAR)) {
        programa->esquerda = parserAnalisarSecaoVar(parser);
    }

    programa->direita = parserAnalisarBloco(parser);
    parserEsperar(parser, TOKEN_PONTO);

    if (!parserVerificar(parser, TOKEN_EOF)) {
        parserErro(parser);
    }

    return programa;
}
