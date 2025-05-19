# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -I$(INCLUDE_DIR)

# Project name (nome do executável)
PROJ_NAME = PROJETO-PIF

# Target directories
BUILD_DIR   = build
OBJ_DIR     = $(BUILD_DIR)/obj
SRC_DIR     = src
INCLUDE_DIR = include

# Source files
SRC_FILES = $(notdir $(wildcard $(SRC_DIR)/*.c))
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(OBJ_DIR) $(OBJ_FILES)
	@echo Compilando o projeto $(PROJ_NAME)...
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(PROJ_NAME) $(OBJ_FILES)

# Criar pasta build/ e build/obj/
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Compilar arquivos .c para .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo Compilando $<...
	@$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -rf $(BUILD_DIR)

# Rodar o jogo
run: all
	@echo Executando o jogo...
	@./$(BUILD_DIR)/$(PROJ_NAME)
