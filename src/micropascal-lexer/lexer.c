#include "micropascal-lexer/lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lexerInicializar(Lexer* lexer, const char* fonte)
{
    lexer->fonte = fonte;
    lexer->tamanho = strlen(fonte);
    lexer->posicao = 0;
    lexer->linha = 1;
    lexer->coluna = 1;
    lexer->houve_erro = 0;
}

int lexerAcabou(const Lexer* lexer)
{
    return lexer->posicao >= lexer->tamanho;
}

char lexerEspiar(const Lexer* lexer, size_t deslocamento)
{
    size_t indice = lexer->posicao + deslocamento;

    if (indice >= lexer->tamanho) {
        return '\0';
    }

    return lexer->fonte[indice];
}

char lexerAvancar(Lexer* lexer)
{
    char atual = lexer->fonte[lexer->posicao];

    lexer->posicao++;

    if (atual == '\n') {
        lexer->linha++;
        lexer->coluna = 1;
    } else {
        lexer->coluna++;
    }

    return atual;
}

int ehLetra(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int ehDigito(char c)
{
    return c >= '0' && c <= '9';
}

char* lexerCopiarIntervalo(const Lexer* lexer, size_t inicio, size_t fim)
{
    size_t comprimento = fim - inicio;
    char* resultado = (char*)malloc(comprimento + 1);

    if (resultado == NULL) {
        return NULL;
    }

    memcpy(resultado, lexer->fonte + inicio, comprimento);
    resultado[comprimento] = '\0';

    return resultado;
}

Token lexerCriarToken(const Lexer* lexer, TokenKind kind, size_t inicio, int linha, int coluna)
{
    Token token;

    token.kind = kind;
    token.lexema = lexerCopiarIntervalo(lexer, inicio, lexer->posicao);
    token.linha = linha;
    token.coluna = coluna;

    return token;
}

TokenKind lexerPalavraReservada(const char* lexema)
{
    if (strcmp(lexema, "program") == 0) {
        return TOKEN_PROGRAM;
    }

    if (strcmp(lexema, "if") == 0) {
        return TOKEN_IF;
    }

    if (strcmp(lexema, "then") == 0) {
        return TOKEN_THEN;
    }

    if (strcmp(lexema, "else") == 0) {
        return TOKEN_ELSE;
    }

    if (strcmp(lexema, "while") == 0) {
        return TOKEN_WHILE;
    }

    if (strcmp(lexema, "do") == 0) {
        return TOKEN_DO;
    }

    if (strcmp(lexema, "repeat") == 0) {
        return TOKEN_REPEAT;
    }

    if (strcmp(lexema, "until") == 0) {
        return TOKEN_UNTIL;
    }

    if (strcmp(lexema, "integer") == 0) {
        return TOKEN_INTEGER;
    }

    if (strcmp(lexema, "real") == 0) {
        return TOKEN_REAL;
    }

    if (strcmp(lexema, "char") == 0) {
        return TOKEN_CHAR;
    }

    if (strcmp(lexema, "begin") == 0) {
        return TOKEN_BEGIN;
    }

    if (strcmp(lexema, "end") == 0) {
        return TOKEN_END;
    }

    if (strcmp(lexema, "write") == 0) {
        return TOKEN_WRITE;
    }

    if (strcmp(lexema, "var") == 0) {
        return TOKEN_VAR;
    }

    if (strcmp(lexema, "div") == 0) {
        return TOKEN_DIV;
    }

    if (strcmp(lexema, "and") == 0) {
        return TOKEN_AND;
    }

    if (strcmp(lexema, "or") == 0) {
        return TOKEN_OR;
    }

    if (strcmp(lexema, "not") == 0) {
        return TOKEN_NOT;
    }

    return TOKEN_IDENTIFICADOR;
}

Token lexerErro(Lexer* lexer, char caractere, int linha, int coluna)
{
    Token token;

    lexer->houve_erro = 1;
    token.kind = TOKEN_ERRO;
    printf("Erro lexico no caracter [%c] (linha %d, coluna %d)\n", caractere, linha, coluna);
    token.lexema = lexerCopiarIntervalo(lexer, lexer->posicao, lexer->posicao);
    token.linha = linha;
    token.coluna = coluna;

    return token;
}

Token lexerLerIdentificador(Lexer* lexer)
{
    size_t inicio = lexer->posicao;
    int linha = lexer->linha;
    int coluna = lexer->coluna;
    Token token;

    while (!lexerAcabou(lexer) && (ehLetra(lexerEspiar(lexer, 0)) || ehDigito(lexerEspiar(lexer, 0)))) {
        lexerAvancar(lexer);
    }

    token = lexerCriarToken(lexer, TOKEN_IDENTIFICADOR, inicio, linha, coluna);
    token.kind = lexerPalavraReservada(token.lexema);

    return token;
}

Token lexerLerNumero(Lexer* lexer)
{
    size_t inicio = lexer->posicao;
    int linha = lexer->linha;
    int coluna = lexer->coluna;
    TokenKind kind = TOKEN_INTEIRO_LITERAL;

    if (lexerEspiar(lexer, 0) == '.') {
        lexerAvancar(lexer);

        while (!lexerAcabou(lexer) && ehDigito(lexerEspiar(lexer, 0))) {
            lexerAvancar(lexer);
        }

        return lexerCriarToken(lexer, TOKEN_REAL_LITERAL, inicio, linha, coluna);
    }

    while (!lexerAcabou(lexer) && ehDigito(lexerEspiar(lexer, 0))) {
        lexerAvancar(lexer);
    }

    if (lexerEspiar(lexer, 0) == '.' && ehDigito(lexerEspiar(lexer, 1))) {
        kind = TOKEN_REAL_LITERAL;
        lexerAvancar(lexer);

        while (!lexerAcabou(lexer) && ehDigito(lexerEspiar(lexer, 0))) {
            lexerAvancar(lexer);
        }
    }

    return lexerCriarToken(lexer, kind, inicio, linha, coluna);
}

Token lexerLerChar(Lexer* lexer)
{
    size_t inicio = lexer->posicao;
    int linha = lexer->linha;
    int coluna = lexer->coluna;
    char conteudo;

    lexerAvancar(lexer);

    if (lexerAcabou(lexer)) {
        return lexerErro(lexer, '\'', linha, coluna);
    }

    conteudo = lexerEspiar(lexer, 0);

    if (conteudo == '\\') {
        char escapado = lexerEspiar(lexer, 1);

        if (escapado == 'n' || escapado == 't') {
            lexerAvancar(lexer);
            lexerAvancar(lexer);
        } else {
            return lexerErro(lexer, conteudo, linha, coluna);
        }
    } else if (ehLetra(conteudo) || ehDigito(conteudo)) {
        lexerAvancar(lexer);
    } else {
        return lexerErro(lexer, conteudo, linha, coluna);
    }

    if (lexerEspiar(lexer, 0) != '\'') {
        return lexerErro(lexer, lexerEspiar(lexer, 0), linha, coluna);
    }

    lexerAvancar(lexer);

    return lexerCriarToken(lexer, TOKEN_CHAR_LITERAL, inicio, linha, coluna);
}

Token lexerProximoToken(Lexer* lexer)
{
    size_t inicio;
    int linha;
    int coluna;
    char atual;
    Token token;

    while (!lexerAcabou(lexer)) {
        atual = lexerEspiar(lexer, 0);

        if (atual == ' ' || atual == '\n' || atual == '\t' || atual == '\r') {
            lexerAvancar(lexer);
            continue;
        }

        break;
    }

    if (lexerAcabou(lexer)) {
        token.kind = TOKEN_EOF;
        token.lexema = lexerCopiarIntervalo(lexer, lexer->posicao, lexer->posicao);
        token.linha = lexer->linha;
        token.coluna = lexer->coluna;

        return token;
    }

    inicio = lexer->posicao;
    linha = lexer->linha;
    coluna = lexer->coluna;
    atual = lexerEspiar(lexer, 0);

    if (ehLetra(atual)) {
        return lexerLerIdentificador(lexer);
    }

    if (ehDigito(atual)) {
        return lexerLerNumero(lexer);
    }

    if (atual == '\'') {
        return lexerLerChar(lexer);
    }

    if (atual == '.' && ehDigito(lexerEspiar(lexer, 1))) {
        return lexerLerNumero(lexer);
    }

    switch (atual) {
        case '<':
            lexerAvancar(lexer);

            if (lexerEspiar(lexer, 0) == '=') {
                lexerAvancar(lexer);
                return lexerCriarToken(lexer, TOKEN_MENOR_IGUAL, inicio, linha, coluna);
            }

            if (lexerEspiar(lexer, 0) == '>') {
                lexerAvancar(lexer);
                return lexerCriarToken(lexer, TOKEN_DIFERENTE, inicio, linha, coluna);
            }

            return lexerCriarToken(lexer, TOKEN_MENOR, inicio, linha, coluna);
        case '>':
            lexerAvancar(lexer);

            if (lexerEspiar(lexer, 0) == '=') {
                lexerAvancar(lexer);
                return lexerCriarToken(lexer, TOKEN_MAIOR_IGUAL, inicio, linha, coluna);
            }

            return lexerCriarToken(lexer, TOKEN_MAIOR, inicio, linha, coluna);
        case '=':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_IGUAL, inicio, linha, coluna);
        case '+':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_MAIS, inicio, linha, coluna);
        case '-':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_MENOS, inicio, linha, coluna);
        case '*':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_MULTIPLICACAO, inicio, linha, coluna);
        case '/':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_DIVISAO, inicio, linha, coluna);
        case ':':
            lexerAvancar(lexer);

            if (lexerEspiar(lexer, 0) == '=') {
                lexerAvancar(lexer);
                return lexerCriarToken(lexer, TOKEN_ATRIBUICAO, inicio, linha, coluna);
            }

            return lexerCriarToken(lexer, TOKEN_DOIS_PONTOS, inicio, linha, coluna);
        case '(':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_ABRE_PARENTESES, inicio, linha, coluna);
        case ')':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_FECHA_PARENTESES, inicio, linha, coluna);
        case ',':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_VIRGULA, inicio, linha, coluna);
        case ';':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_PONTO_VIRGULA, inicio, linha, coluna);
        case '.':
            lexerAvancar(lexer);

            return lexerCriarToken(lexer, TOKEN_PONTO, inicio, linha, coluna);
        default:
            lexerAvancar(lexer);

            return lexerErro(lexer, atual, linha, coluna);
    }
}

void tokenLiberar(Token* token)
{
    if (token->lexema != NULL) {
        free(token->lexema);
        token->lexema = NULL;
    }
}
