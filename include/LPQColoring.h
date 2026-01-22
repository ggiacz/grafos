#ifndef LPQCOLORING_H
#define LPQCOLORING_H

#include "Graph.h"
#include <vector>
#include <random>

struct Solution {
    std::vector<int> coloring;  // coloring[v] = cor do vértice v
    int maxColor;               // Maior cor utilizada (objetivo a minimizar)
    
    Solution() : maxColor(0) {}
    Solution(int n) : coloring(n, -1), maxColor(0) {}
};

class LPQColoring {
private:
    Graph* graph;
    int p;  // Diferença mínima para vértices adjacentes
    int q;  // Diferença mínima para vértices a distância 2
    
    std::mt19937 rng;
    
    // Verifica se uma cor é válida para um vértice
    bool isValidColor(int vertex, int color, const std::vector<int>& coloring) const;
    
    // Encontra a menor cor válida para um vértice
    int findSmallestValidColor(int vertex, const std::vector<int>& coloring) const;
    
    // Encontra conjunto de cores válidas para um vértice
    std::vector<int> getValidColors(int vertex, const std::vector<int>& coloring, int maxColorSoFar) const;
    
    // Busca local para melhorar solução
    Solution localSearch(Solution sol, int maxIterations = 100);
    
    // Calcula grau de saturação de um vértice
    int calculateSaturationDegree(int vertex, const std::vector<int>& coloring) const;
    
    // Calcula custo de um vértice para seleção
    double calculateVertexCost(int vertex, const std::vector<int>& coloring) const;
    
    // Constrói solução usando algoritmo guloso randomizado (uma iteração)
    Solution buildSolution(double alpha);
    
    // Versão otimizada que recalcula apenas custos afetados
    Solution buildSolutionIncremental(double alpha);
    
    // Seleciona vértice da RCL baseado em custos e alpha
    int selectVertexFromRCL(const std::vector<std::pair<double, int>>& costs, double alpha);
    
    // Atualiza probabilidades dos alphas baseado na qualidade observada
    void updateProbabilities(std::vector<double>& probabilities, 
                            const std::vector<double>& blockQuality,
                            const std::vector<size_t>& blockUsage);

public:
    LPQColoring(Graph* g, int p_val, int q_val, unsigned int seed);
    
    // Algoritmo Guloso
    Solution greedy();
    
    // Algoritmo Guloso Randomizado
    Solution greedyRandomized(double alpha, int iterations);
    
    // Algoritmo Guloso Randomizado Reativo
    Solution greedyRandomizedReactive(const std::vector<double>& alphas, int iterations, int blockSize);
    
    // Verifica se uma solução é válida
    bool isSolutionValid(const Solution& sol) const;
};

#endif
