# L(p,q)-Coloring Solver

## Descrição

Este projeto implementa três algoritmos heurísticos para resolver o problema de **L(p,q)-coloring**:

| # | Algoritmo | Descrição |
|---|-----------|-----------|
| 1 | **Greedy** | Algoritmo Guloso determinístico |
| 2 | **Randomized** | Algoritmo Guloso Randomizado |
| 3 | **Reactive** | Algoritmo Guloso Randomizado Reativo |

---

## Compilação

```bash
make
```

O executável será criado em: `bin/lpqcoloring`

### Comandos de limpeza

```bash
make clean      # Limpa arquivos de compilação
make cleanall   # Limpa compilação + resultados
```

---

## Execução

### Sintaxe básica

```bash
./bin/lpqcoloring -f <arquivo> -p <valor> -q <valor> -a <algoritmo> [opções]
```

### Parâmetros

#### Obrigatórios

| Parâmetro | Descrição |
|-----------|-----------|
| `-f <arquivo>` | Arquivo de instância (formato DIMACS) |
| `-p <valor>` | Diferença mínima para vértices adjacentes |
| `-q <valor>` | Diferença mínima para vértices a distância 2 |
| `-a <algoritmo>` | Algoritmo: `greedy`, `randomized` ou `reactive` |

#### Específicos por algoritmo

**Randomized:**

| Parâmetro | Descrição | Padrão |
|-----------|-----------|--------|
| `-alpha <valor>` | Valor de alpha | 0.3 |
| `-i <iterações>` | Número de iterações | 30 |

**Reactive:**

| Parâmetro | Descrição | Padrão |
|-----------|-----------|--------|
| `-alphas <lista>` | Lista de alphas (ex: `0.1,0.3,0.5`) | 0.1,0.3,0.5 |
| `-i <iterações>` | Número de iterações | 300 |
| `-b <bloco>` | Tamanho do bloco para atualização | 30 |

#### Opcionais

| Parâmetro | Descrição |
|-----------|-----------|
| `-s <seed>` | Semente para randomização (padrão: baseada em tempo) |
| `-o <arquivo>` | Salvar solução em arquivo |
| `-csv <arquivo>` | Arquivo CSV para resultados (padrão: `results/results.csv`) |
| `-h, --help` | Mostra ajuda |

---

## Exemplos de Uso

### 1. Algoritmo Guloso

```bash
./bin/lpqcoloring -f instances/exemplo.col -p 2 -q 1 -a greedy
```

### 2. Algoritmo Guloso Randomizado

```bash
./bin/lpqcoloring -f instances/exemplo.col -p 2 -q 1 -a randomized -alpha 0.3 -i 50
```

### 3. Algoritmo Reativo

```bash
./bin/lpqcoloring -f instances/r250.5.col -p 2 -q 1 -a reactive -alphas 0.1,0.3,0.5 -i 300 -b 30
```

### 4. Com semente específica e salvando solução

```bash
./bin/lpqcoloring -f instances/exemplo.col -p 2 -q 1 -a greedy -s 12345 -o solution.txt
```

---

## Script de Experimentos

Para facilitar a execução de experimentos em lote:

```bash
./run_experiments.sh instances/grafo.col 2 1
```

Este script executa automaticamente:
- 10 execuções do algoritmo **guloso**
- 10 execuções do **randomizado** (para cada alpha: 0.1, 0.3, 0.5)
- 10 execuções do **reativo**

Os resultados são salvos em `results/results.csv`

---

## Formato dos Resultados

Os resultados são salvos em CSV com as seguintes colunas:

| Coluna | Descrição |
|--------|-----------|
| `datetime` | Data e hora da execução |
| `instance` | Nome da instância |
| `p`, `q` | Parâmetros do problema |
| `algorithm` | Algoritmo executado |
| `alpha` | Valor de alpha usado |
| `iterations` | Número de iterações |
| `blockSize` | Tamanho do bloco (reactive) |
| `seed` | Semente de randomização |
| `executionTime` | Tempo de execução (segundos) |
| `bestSolution` | Maior cor utilizada (objetivo) |
| `avgSolution` | Média das soluções |
| `bestAlpha` | Melhor alpha encontrado (reactive) |

---

## Estrutura do Projeto

```
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
├── build/                # Arquivos objeto (gerado)
├── bin/                  # Executável (gerado)
├── Makefile
├── README.md
└── run_experiments.sh
```
---

*Lucas França Neves*
*Matheus Leal Costa*
*Gabriel Giacomo Paes*
*Álvaro Davi Carneiro dos Santos*
*João Victor Pereira*
