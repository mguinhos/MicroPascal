#include "micropascal-tests/testes.h"
#include "micropascal-lexer/lexer.h"

#include <string.h>

TokenKind tokenDe(const char* fonte)
{
    Lexer lexer;
    Token token;
    lexerInicializar(&lexer, fonte);
    token = lexerProximoToken(&lexer);
    tokenLiberar(&token);
    return token.kind;
}

int lexemaDe(const char* fonte, const char* esperado)
{
    Lexer lexer;
    Token token;
    int igual;
    lexerInicializar(&lexer, fonte);
    token = lexerProximoToken(&lexer);
    igual = token.lexema != NULL && strcmp(token.lexema, esperado) == 0;
    tokenLiberar(&token);
    return igual;
}

int contaTokens(const char* fonte)
{
    Lexer lexer;
    Token token;
    int contador = 0;
    lexerInicializar(&lexer, fonte);
    while (1) {
        token = lexerProximoToken(&lexer);
        contador++;
        if (token.kind == TOKEN_EOF || token.kind == TOKEN_ERRO) {
            tokenLiberar(&token);
            break;
        }
        tokenLiberar(&token);
    }
    return contador;
}

void runLexerTests(ResultadoTeste* resultado_testes)
{
    TESTE_ASSERT(resultado_testes, tokenDe("program") == TOKEN_PROGRAM, "reconhece program");
    TESTE_ASSERT(resultado_testes, tokenDe("if") == TOKEN_IF, "reconhece if");
    TESTE_ASSERT(resultado_testes, tokenDe("then") == TOKEN_THEN, "reconhece then");
    TESTE_ASSERT(resultado_testes, tokenDe("else") == TOKEN_ELSE, "reconhece else");
    TESTE_ASSERT(resultado_testes, tokenDe("while") == TOKEN_WHILE, "reconhece while");
    TESTE_ASSERT(resultado_testes, tokenDe("do") == TOKEN_DO, "reconhece do");
    TESTE_ASSERT(resultado_testes, tokenDe("repeat") == TOKEN_REPEAT, "reconhece repeat");
    TESTE_ASSERT(resultado_testes, tokenDe("until") == TOKEN_UNTIL, "reconhece until");
    TESTE_ASSERT(resultado_testes, tokenDe("integer") == TOKEN_INTEGER, "reconhece integer");
    TESTE_ASSERT(resultado_testes, tokenDe("real") == TOKEN_REAL, "reconhece real");
    TESTE_ASSERT(resultado_testes, tokenDe("char") == TOKEN_CHAR, "reconhece char");
    TESTE_ASSERT(resultado_testes, tokenDe("begin") == TOKEN_BEGIN, "reconhece begin");
    TESTE_ASSERT(resultado_testes, tokenDe("end") == TOKEN_END, "reconhece end");
    TESTE_ASSERT(resultado_testes, tokenDe("write") == TOKEN_WRITE, "reconhece write");
    TESTE_ASSERT(resultado_testes, tokenDe("var") == TOKEN_VAR, "reconhece var");
    TESTE_ASSERT(resultado_testes, tokenDe("div") == TOKEN_DIV, "reconhece div");
    TESTE_ASSERT(resultado_testes, tokenDe("and") == TOKEN_AND, "reconhece and");
    TESTE_ASSERT(resultado_testes, tokenDe("or") == TOKEN_OR, "reconhece or");
    TESTE_ASSERT(resultado_testes, tokenDe("not") == TOKEN_NOT, "reconhece not");

    TESTE_ASSERT(resultado_testes, tokenDe("Begin") == TOKEN_IDENTIFICADOR, "Begin nao e palavra reservada");
    TESTE_ASSERT(resultado_testes, tokenDe("PROGRAM") == TOKEN_IDENTIFICADOR, "PROGRAM nao e palavra reservada");
    TESTE_ASSERT(resultado_testes, tokenDe("_contador") == TOKEN_IDENTIFICADOR, "identificador com underscore");
    TESTE_ASSERT(resultado_testes, tokenDe("abc123") == TOKEN_IDENTIFICADOR, "identificador alfanumerico");
    TESTE_ASSERT(resultado_testes, lexemaDe("xyz", "xyz"), "lexema do identificador preservado");

    TESTE_ASSERT(resultado_testes, tokenDe("123") == TOKEN_INTEIRO_LITERAL, "inteiro literal");
    TESTE_ASSERT(resultado_testes, tokenDe("0") == TOKEN_INTEIRO_LITERAL, "inteiro literal zero");
    TESTE_ASSERT(resultado_testes, tokenDe("3.14") == TOKEN_REAL_LITERAL, "real literal");
    TESTE_ASSERT(resultado_testes, tokenDe("0.5") == TOKEN_REAL_LITERAL, "real literal com zero a esquerda");
    TESTE_ASSERT(resultado_testes, tokenDe(".5") == TOKEN_REAL_LITERAL, "real literal sem parte inteira");
    TESTE_ASSERT(resultado_testes, tokenDe("7.0") == TOKEN_REAL_LITERAL, "real literal inteiro");

    TESTE_ASSERT(resultado_testes, tokenDe("'a'") == TOKEN_CHAR_LITERAL, "char literal letra");
    TESTE_ASSERT(resultado_testes, tokenDe("'0'") == TOKEN_CHAR_LITERAL, "char literal digito");
    TESTE_ASSERT(resultado_testes, tokenDe("'\\n'") == TOKEN_CHAR_LITERAL, "char literal quebra de linha");
    TESTE_ASSERT(resultado_testes, tokenDe("'\\t'") == TOKEN_CHAR_LITERAL, "char literal tabulacao");

    TESTE_ASSERT(resultado_testes, tokenDe("<") == TOKEN_MENOR, "menor");
    TESTE_ASSERT(resultado_testes, tokenDe(">") == TOKEN_MAIOR, "maior");
    TESTE_ASSERT(resultado_testes, tokenDe("<=") == TOKEN_MENOR_IGUAL, "menor ou igual");
    TESTE_ASSERT(resultado_testes, tokenDe(">=") == TOKEN_MAIOR_IGUAL, "maior ou igual");
    TESTE_ASSERT(resultado_testes, tokenDe("=") == TOKEN_IGUAL, "igual");
    TESTE_ASSERT(resultado_testes, tokenDe("<>") == TOKEN_DIFERENTE, "diferente");
    TESTE_ASSERT(resultado_testes, tokenDe("+") == TOKEN_MAIS, "mais");
    TESTE_ASSERT(resultado_testes, tokenDe("-") == TOKEN_MENOS, "menos");
    TESTE_ASSERT(resultado_testes, tokenDe("*") == TOKEN_MULTIPLICACAO, "multiplicacao");
    TESTE_ASSERT(resultado_testes, tokenDe("/") == TOKEN_DIVISAO, "divisao de reais");
    TESTE_ASSERT(resultado_testes, tokenDe(":=") == TOKEN_ATRIBUICAO, "atribuicao");
    TESTE_ASSERT(resultado_testes, tokenDe("(") == TOKEN_ABRE_PARENTESES, "abre parenteses");
    TESTE_ASSERT(resultado_testes, tokenDe(")") == TOKEN_FECHA_PARENTESES, "fecha parenteses");
    TESTE_ASSERT(resultado_testes, tokenDe(",") == TOKEN_VIRGULA, "virgula");
    TESTE_ASSERT(resultado_testes, tokenDe(";") == TOKEN_PONTO_VIRGULA, "ponto e virgula");
    TESTE_ASSERT(resultado_testes, tokenDe(".") == TOKEN_PONTO, "ponto");
    TESTE_ASSERT(resultado_testes, tokenDe(":") == TOKEN_DOIS_PONTOS, "dois pontos");

    TESTE_ASSERT(resultado_testes, contaTokens("   \n\t\r  ") == 1, "brancos ignorados resultam em EOF");
    TESTE_ASSERT(resultado_testes, contaTokens("a b") == 3, "conta tokens simples");
    TESTE_ASSERT(resultado_testes, tokenDe("$") == TOKEN_ERRO, "caractere invalido gera erro");
    TESTE_ASSERT(resultado_testes, tokenDe("@") == TOKEN_ERRO, "arroba gera erro");
}
