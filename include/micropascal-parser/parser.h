#ifndef MICROPASCAL_PARSER_PARSER_H
#define MICROPASCAL_PARSER_PARSER_H

#include "micropascal-lexer/lexer.h"
#include "micropascal-parser/ast.h"

typedef struct Parser {
    Lexer* lexer;
    Token atual;
    int houve_erro;
    int total_erros;
    int silencioso;
} Parser;

void parserInicializar(Parser* parser, Lexer* lexer);
void parserSilenciarErros(Parser* parser, int silenciar);
AstNo* parserAnalisarPrograma(Parser* parser);
void astLiberar(AstNo* no);
void astImprimir(const AstNo* no, int profundidade);

AstNo* parserCriarNo(AstKind kind, const char* valor);
void parserAvancar(Parser* parser);
void parserErro(Parser* parser);
int parserVerificar(Parser* parser, TokenKind kind);
int parserConsumir(Parser* parser, TokenKind kind);
int parserEsperar(Parser* parser, TokenKind kind);
int parserEmInicioDeComando(Parser* parser);
AstNo* parserAnalisarTipo(Parser* parser);
AstNo* parserAnalisarDeclVar(Parser* parser);
AstNo* parserAnalisarSecaoVar(Parser* parser);
AstNo* parserAnalisarExprBasica(Parser* parser);
AstNo* parserAnalisarExpMul(Parser* parser);
AstNo* parserAnalisarExpAd(Parser* parser);
AstNo* parserAnalisarExpRel(Parser* parser);
AstNo* parserAnalisarExpOrAnd(Parser* parser);
AstNo* parserAnalisarExpressao(Parser* parser);
AstNo* parserAnalisarAtribuicao(Parser* parser);
AstNo* parserAnalisarIteracao(Parser* parser);
AstNo* parserAnalisarDecisao(Parser* parser);
AstNo* parserAnalisarEscrita(Parser* parser);
AstNo* parserAnalisarComando(Parser* parser);
AstNo* parserAnalisarBloco(Parser* parser);

#endif
