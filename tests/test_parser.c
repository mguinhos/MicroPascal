#include "micropascal-tests/testes.h"
#include "micropascal-lexer/lexer.h"
#include "micropascal-parser/parser.h"

#include <string.h>

int programaValido(const char* fonte)
{
    Lexer lexer;
    Parser parser;
    AstNo* programa;
    int valido;
    lexerInicializar(&lexer, fonte);
    parserInicializar(&parser, &lexer);
    parserSilenciarErros(&parser, 1);
    programa = parserAnalisarPrograma(&parser);
    valido = !parser.houve_erro;
    tokenLiberar(&parser.atual);
    astLiberar(programa);
    return valido;
}

const char* PROGRAMA_SIMPLES =
    "program P;\n"
    "var\n"
    "  x : integer;\n"
    "begin\n"
    "  x := 1;\n"
    "end.\n";

const char* PROGRAMA_PARIDADE =
    "program TestaParidade;\n"
    "var\n"
    "  n : integer;\n"
    "begin\n"
    "  n := 2 * 13 + 5;\n"
    "  if n = (n div 2)*2 then\n"
    "    write('p')\n"
    "  ;\n"
    "  else\n"
    "    write('i')\n"
    "  ;\n"
    "end.\n";

const char* PROGRAMA_IMPARES =
    "program SomaImpares;\n"
    "var\n"
    "  n : integer;\n"
    "  i, proximoImpar, soma : integer;\n"
    "begin\n"
    "  n := 4;\n"
    "  i := 0;\n"
    "  soma := 0;\n"
    "  while i < n do\n"
    "  begin\n"
    "    proximoImpar := 2*i + 1;\n"
    "    soma := soma + proximoImpar;\n"
    "    i := i + 1;\n"
    "  end;\n"
    "  write(soma);\n"
    "  write('\\n');\n"
    "end.\n";

const char* PROGRAMA_REPEAT =
    "program Contador;\n"
    "var\n"
    "  i : integer;\n"
    "begin\n"
    "  i := 0;\n"
    "  repeat\n"
    "    i := i + 1;\n"
    "  until i >= 10;\n"
    "end.\n";

const char* PROGRAMA_TIPOS =
    "program Tipos;\n"
    "var\n"
    "  a, b : integer;\n"
    "  c : real;\n"
    "  d : char;\n"
    "begin\n"
    "  a := 1;\n"
    "  b := 2;\n"
    "  c := 1.5 + 2.5;\n"
    "  d := 'x';\n"
    "end.\n";

void runParserTests(ResultadoTeste* resultado_testes)
{
    TESTE_ASSERT(resultado_testes, programaValido(PROGRAMA_SIMPLES), "programa simples valido");
    TESTE_ASSERT(resultado_testes, programaValido(PROGRAMA_PARIDADE), "programa TestaParidade valido");
    TESTE_ASSERT(resultado_testes, programaValido(PROGRAMA_IMPARES), "programa SomaImpares valido");
    TESTE_ASSERT(resultado_testes, programaValido(PROGRAMA_REPEAT), "programa com repeat until valido");
    TESTE_ASSERT(resultado_testes, programaValido(PROGRAMA_TIPOS), "programa com varios tipos valido");

    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := 1 + 2 * 3;\nend.\n"), "precedencia multiplicacao sobre soma");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := (1 + 2) * 3;\nend.\n"), "parenteses");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := not a and b or c;\nend.\n"), "operadores not and or");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := a = b;\nend.\n"), "igualdade");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := a <> b;\nend.\n"), "diferente");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  x := a <= b;\nend.\n"), "menor ou igual");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  if a then b := 1;\nend.\n"), "decisao sem else");
    TESTE_ASSERT(resultado_testes, programaValido("program P;\nbegin\n  while a do b := 1;\nend.\n"), "iteracao while");

    TESTE_ASSERT(resultado_testes, !programaValido("program P\nbegin end.\n"), "falta ponto e virgula apos nome");
    TESTE_ASSERT(resultado_testes, !programaValido("program P;\nbegin\n  x := ;\nend.\n"), "expressao ausente");
    TESTE_ASSERT(resultado_testes, !programaValido("program P;\nbegin\n  x := 1 +\nend.\n"), "operando ausente");
    TESTE_ASSERT(resultado_testes, !programaValido("program P;\nbegin\n  if a then\nend.\n"), "comando ausente apos then");
    TESTE_ASSERT(resultado_testes, !programaValido("program P;\nvar x integer;\nbegin end.\n"), "falta dois pontos na declaracao");
    TESTE_ASSERT(resultado_testes, !programaValido("program P;\nbegin\n  x := 1\nend.\n"), "falta ponto e virgula no comando");
}
