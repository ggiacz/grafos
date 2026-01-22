# Makefile para o projeto L(p,q)-Coloring
# Compatível com ambientes Linux

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -I./include
LDFLAGS = 

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Arquivos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/lpqcoloring

# Regra principal
all: directories $(TARGET)

# Cria diretórios necessários
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p results

# Compilação do executável
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	@$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"

# Compilação dos objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Limpando arquivos de compilação..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Limpeza concluída!"

# Limpeza completa (incluindo resultados)
cleanall: clean
	@echo "Limpando resultados..."
	@rm -rf results/*.csv
	@echo "Limpeza completa concluída!"

# Regras de execução rápida (exemplos)
run-greedy:
	@./$(TARGET) -f instances/exemplo.col -p 2 -q 1 -a greedy

run-randomized:
	@./$(TARGET) -f instances/exemplo.col -p 2 -q 1 -a randomized -alpha 0.3 -i 30

run-reactive:
	@./$(TARGET) -f instances/exemplo.col -p 2 -q 1 -a reactive -alphas 0.1,0.3,0.5 -i 300 -b 30

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make clean    - Remove arquivos de compilação"
	@echo "  make cleanall - Remove compilação e resultados"
	@echo "  make help     - Mostra esta ajuda"
	@echo ""
	@echo "Para executar:"
	@echo "  ./bin/lpqcoloring -h  - Mostra ajuda do programa"

.PHONY: all clean cleanall directories help run-greedy run-randomized run-reactive
