#include "micropascal-lexer/lexer.h"
#include "micropascal-parser/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* lerArquivo(const char* caminho, long* tamanho)
{
    FILE* arquivo = fopen(caminho, "rb");
    char* conteudo;

    if (arquivo == NULL) {
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    *tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    conteudo = (char*)malloc((size_t)(*tamanho) + 1);

    if (conteudo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    if (fread(conteudo, 1, (size_t)(*tamanho), arquivo) != (size_t)(*tamanho)) {
        free(conteudo);
        fclose(arquivo);
        return NULL;
    }

    conteudo[*tamanho] = '\0';
    fclose(arquivo);

    return conteudo;
}

void imprimirTokens(const char* fonte)
{
    Lexer lexer;
    Token token;

    lexerInicializar(&lexer, fonte);

    do {
        token = lexerProximoToken(&lexer);

        if (token.kind == TOKEN_ERRO) {
            tokenLiberar(&token);
            break;
        }

        printf("%s [%s]\n", tokenKindNome(token.kind), token.lexema);

        if (token.kind == TOKEN_EOF) {
            tokenLiberar(&token);
            break;
        }

        tokenLiberar(&token);
    } while (1);
}

int main(int argc, char** argv)
{
    const char* caminho;
    const char* modo;
    char* fonte;
    long tamanho = 0;

    if (argc < 2) {
        printf("Uso: micropascal <arquivo> [--tokens|--ast]\n");
        return 1;
    }

    caminho = argv[1];
    modo = "--ast";

    if (argc >= 3) {
        modo = argv[2];
    }

    fonte = lerArquivo(caminho, &tamanho);

    if (fonte == NULL) {
        printf("Nao foi possivel abrir o arquivo [%s]\n", caminho);
        return 1;
    }

    if (strcmp(modo, "--tokens") == 0) {
        imprimirTokens(fonte);
        free(fonte);
        return 0;
    }

    {
        Lexer lexer;
        Parser parser;
        AstNo* programa;

        lexerInicializar(&lexer, fonte);
        parserInicializar(&parser, &lexer);
        programa = parserAnalisarPrograma(&parser);
        tokenLiberar(&parser.atual);

        if (parser.houve_erro) {
            astLiberar(programa);
            free(fonte);
            return 1;
        }

        printf("Analise sintatica concluida com sucesso.\n");
        astImprimir(programa, 0);
        astLiberar(programa);
    }

    free(fonte);

    return 0;
}
