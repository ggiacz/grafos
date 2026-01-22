#!/bin/bash

# Script para teste rápido do projeto
# Compila e executa todos os três algoritmos

echo "=========================================="
echo "  Teste Rápido - L(p,q)-Coloring"
echo "=========================================="
echo ""

# Compila o projeto
echo "1. Compilando o projeto..."
make clean > /dev/null 2>&1
make

if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    exit 1
fi

echo "   ✓ Compilação bem-sucedida!"
echo ""

# Parâmetros de teste
INSTANCE="instances/exemplo.col"
P=2
Q=1

# Verifica se a instância existe
if [ ! -f "$INSTANCE" ]; then
    echo "Erro: Instância de exemplo não encontrada!"
    exit 1
fi

echo "2. Testando com instância: $INSTANCE (p=$P, q=$Q)"
echo ""

# Teste 1: Guloso
echo "----------------------------------------"
echo "Teste 1: Algoritmo Guloso"
echo "----------------------------------------"
./bin/lpqcoloring -f "$INSTANCE" -p $P -q $Q -a greedy
echo ""

# Teste 2: Guloso Randomizado
echo "----------------------------------------"
echo "Teste 2: Algoritmo Guloso Randomizado"
echo "----------------------------------------"
./bin/lpqcoloring -f "$INSTANCE" -p $P -q $Q -a randomized -alpha 0.3 -i 10
echo ""

# Teste 3: Guloso Randomizado Reativo
echo "----------------------------------------"
echo "Teste 3: Algoritmo Guloso Randomizado Reativo"
echo "----------------------------------------"
./bin/lpqcoloring -f "$INSTANCE" -p $P -q $Q -a reactive -alphas 0.1,0.3,0.5 -i 30 -b 10
echo ""

echo "=========================================="
echo "  Testes Concluídos!"
echo "=========================================="
echo ""
echo "Verifique os resultados em: results/results.csv"
echo ""
echo "Para executar experimentos completos:"
echo "  ./run_experiments.sh $INSTANCE $P $Q"
echo ""
