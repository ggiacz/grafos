===========================================
  L(p,q)-Coloring Solver
  DCC059 - Teoria dos Grafos - Trabalho 2
===========================================

DESCRIÇÃO
---------
Este projeto implementa três algoritmos heurísticos para resolver o problema
de L(p,q)-coloring:
  1. Algoritmo Guloso (Greedy)
  2. Algoritmo Guloso Randomizado
  3. Algoritmo Guloso Randomizado Reativo

COMPILAÇÃO
----------
Para compilar o projeto em ambiente Linux:

  make

Isso criará o executável em: bin/lpqcoloring


EXECUÇÃO
--------
O programa aceita vários parâmetros via linha de comando:

PARÂMETROS OBRIGATÓRIOS:
  -f <arquivo>     Arquivo de instância (formato DIMACS)
  -p <valor>       Valor de p (diferença mínima para vértices adjacentes)
  -q <valor>       Valor de q (diferença mínima para vértices a distância 2)
  -a <algoritmo>   Algoritmo: greedy | randomized | reactive

PARÂMETROS PARA ALGORITMO RANDOMIZED:
  -alpha <valor>   Valor de alpha (padrão: 0.3)
  -i <iterações>   Número de iterações (padrão: 30)

PARÂMETROS PARA ALGORITMO REACTIVE:
  -alphas <lista>  Lista de alphas separados por vírgula (ex: 0.1,0.3,0.5)
  -i <iterações>   Número de iterações (padrão: 300)
  -b <bloco>       Tamanho do bloco para atualização (padrão: 30)

PARÂMETROS OPCIONAIS:
  -s <seed>        Semente para randomização (padrão: baseada em tempo)
  -o <arquivo>     Salvar solução em arquivo
  -csv <arquivo>   Arquivo CSV para resultados (padrão: results/results.csv)
  -h, --help       Mostra ajuda


EXEMPLOS DE USO
---------------

1. Algoritmo Guloso:
   ./bin/lpqcoloring -f instances/grafo.col -p 2 -q 1 -a greedy

2. Algoritmo Guloso Randomizado (alpha=0.3, 50 iterações):
   ./bin/lpqcoloring -f instances/grafo.col -p 2 -q 1 -a randomized -alpha 0.3 -i 50

3. Algoritmo Reativo (3 alphas, 300 iterações, blocos de 30):
   ./bin/lpqcoloring -f instances/grafo.col -p 2 -q 1 -a reactive -alphas 0.1,0.3,0.5 -i 300 -b 30

4. Com semente específica e salvando solução:
   ./bin/lpqcoloring -f instances/grafo.col -p 2 -q 1 -a greedy -s 12345 -o solution.txt


SCRIPT DE EXPERIMENTOS
----------------------
Para facilitar a execução dos experimentos, use o script run_experiments.sh:

  ./run_experiments.sh instances/grafo.col 2 1

Este script executará automaticamente:
  - 10 execuções do algoritmo guloso
  - 10 execuções do randomizado (para cada alpha: 0.1, 0.3, 0.5)
  - 10 execuções do reativo

Os resultados serão salvos em results/results.csv


FORMATO DAS INSTÂNCIAS
----------------------
O programa lê arquivos no formato DIMACS:
  - Linhas começando com 'c' são comentários
  - Linha 'p edge <n> <m>' define número de vértices e arestas
  - Linhas 'e <u> <v>' definem arestas (índices começam em 1)


ESTRUTURA DO PROJETO
--------------------
grafos/
├── src/                  # Código fonte (.cpp)
│   ├── main.cpp
│   ├── Graph.cpp
│   ├── LPQColoring.cpp
│   └── Utils.cpp
├── include/              # Headers (.h)
│   ├── Graph.h
│   ├── LPQColoring.h
│   └── Utils.h
├── instances/            # Instâncias de teste
├── results/              # Resultados CSV
├── build/                # Arquivos objeto (gerado na compilação)
├── bin/                  # Executável (gerado na compilação)
├── Makefile              # Para compilação
├── README.txt            # Este arquivo
└── run_experiments.sh    # Script para executar experimentos


RESULTADOS
----------
Os resultados são automaticamente salvos em formato CSV com as seguintes colunas:
  - datetime: Data e hora da execução
  - instance: Nome da instância
  - p, q: Parâmetros do problema
  - algorithm: Algoritmo executado
  - alpha: Valor de alpha usado (se aplicável)
  - iterations: Número de iterações
  - blockSize: Tamanho do bloco (para reactive)
  - seed: Semente de randomização
  - executionTime: Tempo de execução em segundos
  - bestSolution: Maior cor utilizada (objetivo a minimizar)
  - avgSolution: Média das soluções
  - bestAlpha: Melhor alpha encontrado (para reactive)


LIMPEZA
-------
Para limpar arquivos de compilação:
  make clean

Para limpar tudo (compilação + resultados):
  make cleanall


DOWNLOAD DAS INSTÂNCIAS
-----------------------
As instâncias de teste para L(p,q)-coloring podem ser obtidas em:
  https://cedric.cnam.fr/~porumbed/graphs/

Baixe os arquivos .col e coloque-os no diretório instances/


OBSERVAÇÕES
-----------
- O código foi desenvolvido em C++11 e testado em ambiente Linux
- Certifique-se de que o compilador g++ está instalado
- O programa valida automaticamente se a solução encontrada é válida
- A semente de randomização é impressa para permitir reproduzir experimentos
- Para valores de p e q, consulte a literatura ou use p=2, q=1 como padrão


AUTORES
-------
[Seu nome e matrícula aqui]

DCC059 - Teoria dos Grafos
Universidade Federal de Juiz de Fora
