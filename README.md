# Micro-Pascal

Analisadores lexico e sintatico de um compilador para a linguagem Micro-Pascal,
uma versao simplificada de Pascal, implementados em C.

## Contexto academico

- Disciplina: 2026.2-MINF-0203-CONSTRUCAO DE COMPILADORES-MANHA
- Professor: Robinson Lins
- Universidade Catolica de Pernambuco (UNICAP)

## Autores

- Marcel Guinhos de Menezes Feitosa
- Site: https://mguinhos.github.io/
- GitHub: https://github.com/mguinhos
- RA UNICAP: 00000855041
- Email academico: marcel.00000855041@unicap.br

- Aloisio Souto Pinto Filho
- RA UNICAP: 00000849233
- Email academico: aloisio.00000849233@unicap.br

## Estrutura do projeto

```
micro-pascal/
  include/
    micropascal-lexer/    cabecalhos do lexer (token.h, lexer.h)
    micropascal-parser/   cabecalhos do parser (ast.h, parser.h)
    micropascal-tests/    cabecalho auxiliar dos testes
  src/
    main.c                ponto de entrada
    micropascal-lexer/    implementacao do lexer
    micropascal-parser/   implementacao do parser e da AST
  tests/                  codigo dos testes automatizados
  examples/               programas de exemplo em Micro-Pascal
  Makefile                build para Linux e Windows
```

Todos os arquivos de cabecalho residem em include/.

## Requisitos de compilacao

- Compilador C99 (gcc/cc ou clang).
- GNU Make.
- Alvos suportados: Linux e Windows.

## Uso

```
make help    mostra a ajuda
make build   compila o compilador
make run     compila e executa em examples/testaparidade.pas
make test    compila e executa a suite de testes
make clean   remove o diretorio build
```

Os binarios ficam em build/<arquitetura>/bin e os artefatos de teste em
build/<arquitetura>/.tests.

Para usar outro programa de entrada:

```
make run ARGS=examples/somaimpares.pas
```

Para imprimir apenas a sequencia de tokens:

```
build/x86/bin/micropascal examples/testaparidade.pas --tokens
```

## Escopo

- Analise lexica completa, com todos os tokens da especificacao.
- Analise sintatica por descida recursiva, com a precedencia e a
  associatividade definidas na especificacao.

Mensagens de erro:

- Léxico: Erro léxico no caracter [x]
- Sintático: Erro de sintaxe no token [lexema]

## Licenca

MIT. Veja LICENSE.md.
