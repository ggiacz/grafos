#!/bin/bash

# Script para executar experimentos do L(p,q)-coloring
# Uso: ./run_experiments.sh <arquivo_instancia> <p> <q>

if [ "$#" -ne 3 ]; then
    echo "Uso: $0 <arquivo_instancia> <p> <q>"
    echo "Exemplo: $0 instances/grafo.col 2 1"
    exit 1
fi

INSTANCE=$1
P=$2
Q=$3
EXECUTABLE="./bin/lpqcoloring"

# Verifica se o executável existe
if [ ! -f "$EXECUTABLE" ]; then
    echo "Erro: Executável não encontrado. Execute 'make' primeiro."
    exit 1
fi

# Verifica se a instância existe
if [ ! -f "$INSTANCE" ]; then
    echo "Erro: Arquivo de instância não encontrado: $INSTANCE"
    exit 1
fi

echo "=========================================="
echo "  Executando Experimentos"
echo "=========================================="
echo "Instância: $INSTANCE"
echo "p = $P, q = $Q"
echo ""

# Configurações
NUM_EXECUCOES=10
ALPHAS=(0.1 0.3 0.5)
ITER_RANDOMIZED=30
ITER_REACTIVE=300
BLOCK_SIZE=30

# Cria diretório de resultados se não existe
mkdir -p results

# Nome base para os resultados
INSTANCE_NAME=$(basename "$INSTANCE" .col)
CSV_FILE="results/results_${INSTANCE_NAME}_p${P}_q${Q}.csv"

echo "Resultados serão salvos em: $CSV_FILE"
echo ""

# ========================================
# 1. ALGORITMO GULOSO
# ========================================
echo "----------------------------------------"
echo "1. Executando Algoritmo Guloso"
echo "----------------------------------------"
for i in $(seq 1 $NUM_EXECUCOES); do
    echo "  Execução $i/$NUM_EXECUCOES..."
    $EXECUTABLE -f "$INSTANCE" -p $P -q $Q -a greedy -csv "$CSV_FILE"
done
echo "Guloso: Concluído!"
echo ""

# ========================================
# 2. ALGORITMO GULOSO RANDOMIZADO
# ========================================
echo "----------------------------------------"
echo "2. Executando Algoritmo Guloso Randomizado"
echo "----------------------------------------"
for alpha in "${ALPHAS[@]}"; do
    echo "  Alpha = $alpha"
    for i in $(seq 1 $NUM_EXECUCOES); do
        echo "    Execução $i/$NUM_EXECUCOES..."
        $EXECUTABLE -f "$INSTANCE" -p $P -q $Q -a randomized \
                    -alpha $alpha -i $ITER_RANDOMIZED -csv "$CSV_FILE"
    done
done
echo "Guloso Randomizado: Concluído!"
echo ""

# ========================================
# 3. ALGORITMO GULOSO RANDOMIZADO REATIVO
# ========================================
echo "----------------------------------------"
echo "3. Executando Algoritmo Guloso Randomizado Reativo"
echo "----------------------------------------"
ALPHAS_STR=$(IFS=,; echo "${ALPHAS[*]}")
for i in $(seq 1 $NUM_EXECUCOES); do
    echo "  Execução $i/$NUM_EXECUCOES..."
    $EXECUTABLE -f "$INSTANCE" -p $P -q $Q -a reactive \
                -alphas "$ALPHAS_STR" -i $ITER_REACTIVE -b $BLOCK_SIZE \
                -csv "$CSV_FILE"
done
echo "Guloso Randomizado Reativo: Concluído!"
echo ""

# ========================================
# RESUMO
# ========================================
echo "=========================================="
echo "  Experimentos Concluídos!"
echo "=========================================="
echo "Total de execuções:"
echo "  - Guloso: $NUM_EXECUCOES"
echo "  - Randomizado: $((NUM_EXECUCOES * ${#ALPHAS[@]}))"
echo "  - Reativo: $NUM_EXECUCOES"
echo "  TOTAL: $((NUM_EXECUCOES * (1 + ${#ALPHAS[@]} + 1)))"
echo ""
echo "Resultados salvos em: $CSV_FILE"
echo ""
echo "Para visualizar os resultados:"
echo "  cat $CSV_FILE"
echo "  ou abra o arquivo em um editor de planilhas"
echo ""
