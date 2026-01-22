#!/bin/bash

# Script para preparar o projeto para entrega/git
# Remove arquivos temporários e de compilação

echo "=============================================="
echo "  Preparando projeto para entrega"
echo "=============================================="
echo ""

# Remove arquivos de compilação
echo "1. Removendo arquivos de compilação..."
make clean > /dev/null 2>&1
echo "   ✓ Limpeza de compilação concluída"

# Remove resultados (opcional - comente se quiser manter)
# echo "2. Removendo resultados antigos..."
# rm -f results/*.csv
# echo "   ✓ Resultados removidos"

# Remove arquivos temporários
echo "2. Removendo arquivos temporários..."
find . -name "*~" -delete
find . -name "*.swp" -delete
find . -name "*.swo" -delete
find . -name ".DS_Store" -delete
echo "   ✓ Temporários removidos"

# Verifica estrutura do projeto
echo ""
echo "3. Verificando estrutura do projeto..."
echo ""

check_file() {
    if [ -f "$1" ]; then
        echo "   ✓ $1"
    else
        echo "   ✗ $1 (FALTANDO!)"
    fi
}

check_dir() {
    if [ -d "$1" ]; then
        echo "   ✓ $1/"
    else
        echo "   ✗ $1/ (FALTANDO!)"
    fi
}

echo "   Arquivos principais:"
check_file "README.txt"
check_file "Makefile"
check_file ".gitignore"

echo ""
echo "   Código fonte:"
check_file "src/main.cpp"
check_file "src/Graph.cpp"
check_file "src/LPQColoring.cpp"
check_file "src/Utils.cpp"

echo ""
echo "   Headers:"
check_file "include/Graph.h"
check_file "include/LPQColoring.h"
check_file "include/Utils.h"

echo ""
echo "   Scripts:"
check_file "run_experiments.sh"
check_file "test.sh"
check_file "analyze_results.py"

echo ""
echo "   Documentação:"
check_file "PROBLEMA.txt"
check_file "DOWNLOAD_INSTANCIAS.txt"
check_file "TROUBLESHOOTING.txt"
check_file "relatorio_template.tex"

echo ""
echo "   Diretórios:"
check_dir "src"
check_dir "include"
check_dir "instances"
check_dir "results"

# Conta linhas de código
echo ""
echo "4. Estatísticas do projeto:"
echo ""
total_lines=$(find src include -name "*.cpp" -o -name "*.h" | xargs wc -l | tail -1 | awk '{print $1}')
echo "   Total de linhas de código: $total_lines"

num_instances=$(ls instances/*.col 2>/dev/null | wc -l)
echo "   Número de instâncias: $num_instances"

# Verifica se compila
echo ""
echo "5. Testando compilação..."
make > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "   ✓ Projeto compila sem erros!"
    
    # Testa execução básica
    if [ -f "instances/exemplo.col" ]; then
        echo ""
        echo "6. Testando execução básica..."
        ./bin/lpqcoloring -f instances/exemplo.col -p 2 -q 1 -a greedy > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo "   ✓ Programa executa corretamente!"
        else
            echo "   ✗ Erro na execução!"
        fi
    fi
else
    echo "   ✗ Erro na compilação!"
    echo "   Execute 'make' manualmente para ver os erros."
fi

# Lista arquivos grandes
echo ""
echo "7. Verificando tamanho dos arquivos..."
large_files=$(find . -type f -size +1M 2>/dev/null)
if [ -z "$large_files" ]; then
    echo "   ✓ Nenhum arquivo muito grande encontrado"
else
    echo "   ⚠ Arquivos grandes encontrados:"
    find . -type f -size +1M -exec ls -lh {} \;
fi

# Gera checklist para entrega
echo ""
echo "=============================================="
echo "  Checklist para Entrega"
echo "=============================================="
echo ""
echo "□ Código compila sem erros"
echo "□ Código compila sem warnings"
echo "□ README.txt atualizado com seu nome"
echo "□ Todos os 3 algoritmos implementados"
echo "□ Testes executados"
echo "□ Resultados gerados"
echo "□ Relatório escrito"
echo "□ Git atualizado"
echo "□ Código testado no laboratório"
echo ""

echo "=============================================="
echo "  Projeto pronto para entrega!"
echo "=============================================="
echo ""
echo "Para enviar ao git:"
echo "  git add ."
echo "  git commit -m 'Projeto L(p,q)-coloring completo'"
echo "  git push"
echo ""
echo "Para gerar arquivo compactado:"
echo "  tar -czf lpqcoloring.tar.gz src/ include/ Makefile README.txt"
echo ""
