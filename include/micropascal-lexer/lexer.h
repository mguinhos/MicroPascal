#ifndef MICROPASCAL_LEXER_LEXER_H
#define MICROPASCAL_LEXER_LEXER_H

#include "micropascal-lexer/token.h"

typedef struct Lexer {
    const char* fonte;
    size_t tamanho;
    size_t posicao;
    int linha;
    int coluna;
    int houve_erro;
} Lexer;

void lexerInicializar(Lexer* lexer, const char* fonte);
Token lexerProximoToken(Lexer* lexer);
void tokenLiberar(Token* token);

int lexerAcabou(const Lexer* lexer);
char lexerEspiar(const Lexer* lexer, size_t deslocamento);
char lexerAvancar(Lexer* lexer);
int ehLetra(char c);
int ehDigito(char c);
char* lexerCopiarIntervalo(const Lexer* lexer, size_t inicio, size_t fim);
Token lexerCriarToken(const Lexer* lexer, TokenKind kind, size_t inicio, int linha, int coluna);
TokenKind lexerPalavraReservada(const char* lexema);
Token lexerErro(Lexer* lexer, char caractere, int linha, int coluna);
Token lexerLerIdentificador(Lexer* lexer);
Token lexerLerNumero(Lexer* lexer);
Token lexerLerChar(Lexer* lexer);

#endif
