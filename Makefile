ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    ARCH_RAW := $(PROCESSOR_ARCHITECTURE)
    BIN_EXT := .exe
    ifeq ($(origin CC),default)
        CC := gcc
    endif
    ifeq ($(shell echo %OS%),Windows_NT)
        RM := del /Q
        RMDIR = if exist "$(call NATIVE_PATH,$(1))" rmdir /S /Q "$(call NATIVE_PATH,$(1))"
        NATIVE_PATH = $(subst /,\,$(1))
        MKDIR_P = if not exist "$(call NATIVE_PATH,$(1))" mkdir "$(call NATIVE_PATH,$(1))"
        RUN_PREFIX :=
    else
        RM := rm -f
        RMDIR = rm -rf "$(1)"
        NATIVE_PATH = $(1)
        MKDIR_P = mkdir -p "$(1)"
        RUN_PREFIX := ./
    endif
else
    DETECTED_OS := $(shell uname -s)
    RM := rm -f
    RMDIR = rm -rf "$(1)"
    ARCH_RAW := $(shell uname -m)
    BIN_EXT :=
    NATIVE_PATH = $(1)
    MKDIR_P = mkdir -p "$(1)"
    RUN_PREFIX := ./
endif

ifneq (,$(filter x86_64 amd64 AMD64 x86 i386 i486 i586 i686,$(ARCH_RAW)))
    ARCH := x86
else ifneq (,$(filter aarch64 arm64 ARM64 armv7l armv6l arm,$(ARCH_RAW)))
    ARCH := arm
else ifneq (,$(filter ppc64 ppc64le powerpc,$(ARCH_RAW)))
    ARCH := powerpc
else
    ARCH := $(ARCH_RAW)
endif

CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Iinclude

BUILD_DIR := build/$(ARCH)
BIN_DIR := $(BUILD_DIR)/bin
TEST_DIR := $(BUILD_DIR)/.tests
BIN := $(BIN_DIR)/micropascal$(BIN_EXT)
TEST_BIN := $(TEST_DIR)/micropascal-tests$(BIN_EXT)

LIB_SRCS := $(wildcard src/micropascal-lexer/*.c) $(wildcard src/micropascal-parser/*.c)
APP_SRCS := src/main.c $(LIB_SRCS)
TEST_SRCS := tests/test_main.c tests/test_lexer.c tests/test_parser.c $(LIB_SRCS)

ARGS ?= examples/testaparidade.pas

.PHONY: help all build run test clean

help:
	@echo "Micro-Pascal - alvos disponiveis"
	@echo "  make help    mostra esta ajuda"
	@echo "  make build   compila o compilador em $(BIN)"
	@echo "  make run     compila e executa em uma amostra (ARGS=arquivo)"
	@echo "  make test    compila e executa a suite de testes"
	@echo "  make clean   remove o diretorio build"

all: build

build: $(BIN)

$(BIN): $(APP_SRCS)
	@$(call MKDIR_P,$(BIN_DIR))
	$(CC) $(CFLAGS) $(APP_SRCS) -o $(BIN)

$(TEST_BIN): $(TEST_SRCS)
	@$(call MKDIR_P,$(TEST_DIR))
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_BIN)

run: build
	$(RUN_PREFIX)$(call NATIVE_PATH,$(BIN)) $(ARGS)

test: $(TEST_BIN)
	$(RUN_PREFIX)$(call NATIVE_PATH,$(TEST_BIN))

clean:
	$(call RMDIR,build)
