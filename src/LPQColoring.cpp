#include "../include/LPQColoring.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>

using namespace std;

LPQColoring::LPQColoring(Graph* g, int p_val, int q_val, unsigned int seed) 
    : graph(g), p(p_val), q(q_val), rng(seed) {
}

bool LPQColoring::isValidColor(int vertex, int color, const vector<int>& coloring) const {
    // Verifica restrição com vizinhos diretos (distância 1)
    for (auto neighbor : graph->getNeighbors(vertex)) {
        if (coloring[neighbor] != -1) {
            auto diff = abs(color - coloring[neighbor]);
            if (diff < p) {
                return false;
            }
        }
    }
    
    // Verifica restrição com vizinhos a distância 2
    for (auto neighbor2 : graph->getDistance2Neighbors(vertex)) {
        if (coloring[neighbor2] != -1) {
            auto diff = abs(color - coloring[neighbor2]);
            if (diff < q) {
                return false;
            }
        }
    }
    
    return true;
}

int LPQColoring::findSmallestValidColor(int vertex, const vector<int>& coloring) const {
    auto color = 0;
    while (!isValidColor(vertex, color, coloring)) {
        color++;
        if (color > 10000) {  // Proteção contra loop infinito
            cerr << "Erro: não foi possível encontrar cor válida para vértice " << vertex << endl;
            return color;
        }
    }
    return color;
}

vector<int> LPQColoring::getValidColors(int vertex, const vector<int>& coloring, int maxColorSoFar) const {
    vector<int> validColors;
    
    // Testa cores de 0 até um pouco acima da maior cor atual
    auto maxColorToTest = maxColorSoFar + max(p, q) + 10;
    
    for (auto color = 0; color <= maxColorToTest; color++) {
        if (isValidColor(vertex, color, coloring)) {
            validColors.push_back(color);
        }
    }
    
    return validColors;
}

Solution LPQColoring::localSearch(Solution sol, int maxIterations) {
    auto n = graph->getNumVertices();
    auto improved = true;
    auto iterations = 0;
    
    while (improved && iterations < maxIterations) {
        improved = false;
        iterations++;
        
        // Para cada vértice que usa uma cor alta, tenta recolori-lo com cor menor
        for (auto v = 0; v < n; v++) {
            auto currentColor = sol.coloring[v];
            
            // Se não é um vértice com cor alta, skip
            if (currentColor < sol.maxColor * 0.7) continue;
            
            // Tenta cores menores
            for (auto newColor = 0; newColor < currentColor; newColor++) {
                if (isValidColor(v, newColor, sol.coloring)) {
                    sol.coloring[v] = newColor;
                    
                    // Recalcula maxColor
                    auto newMaxColor = 0;
                    for (auto i = 0; i < n; i++) {
                        newMaxColor = max(newMaxColor, sol.coloring[i]);
                    }
                    
                    if (newMaxColor < sol.maxColor) {
                        sol.maxColor = newMaxColor;
                        improved = true;
                    }
                    break;
                }
            }
        }
    }
    
    return sol;
}

int LPQColoring::calculateSaturationDegree(int vertex, const vector<int>& coloring) const {
    auto satDegree = 0;
    for (auto neighbor : graph->getNeighbors(vertex)) {
        if (coloring[neighbor] != -1) satDegree++;
    }
    for (auto neighbor2 : graph->getDistance2Neighbors(vertex)) {
        if (coloring[neighbor2] != -1) satDegree++;
    }
    return satDegree;
}

double LPQColoring::calculateVertexCost(int vertex, const vector<int>& coloring) const {
    auto minColor = findSmallestValidColor(vertex, coloring);
    auto satDegree = calculateSaturationDegree(vertex, coloring);
    return minColor * 100.0 - satDegree;
}

int LPQColoring::selectVertexFromRCL(const vector<pair<double, int>>& costs, double alpha) {
    auto minCost = costs[0].first;
    auto maxCost = costs.back().first;
    auto threshold = minCost + alpha * (maxCost - minCost);
    
    vector<int> rcl;
    rcl.reserve(costs.size() / 4); // Otimização: reserva memória
    
    for (const auto& pair : costs) {
        if (pair.first <= threshold) {
            rcl.push_back(pair.second);
        } else {
            break; // Já está ordenado, pode parar
        }
    }
    
    uniform_int_distribution<int> dist(0, rcl.size() - 1);
    return rcl[dist(rng)];
}

void LPQColoring::updateProbabilities(vector<double>& probabilities,
                                      const vector<double>& blockQuality,
                                      const vector<size_t>& blockUsage) {
    auto numAlphas = probabilities.size();
    vector<double> avgQuality(numAlphas, 0.0);
    
    // Calcula qualidade média de cada alpha
    for (size_t i = 0; i < numAlphas; i++) {
        if (blockUsage[i] > 0) {
            avgQuality[i] = blockQuality[i] / blockUsage[i];
        }
    }
    
    // Atualiza probabilidades proporcionalmente à qualidade
    auto sumQuality = 0.0;
    for (size_t i = 0; i < numAlphas; i++) {
        sumQuality += avgQuality[i];
    }
    
    if (sumQuality > 0) {
        for (size_t i = 0; i < numAlphas; i++) {
            probabilities[i] = avgQuality[i] / sumQuality;
        }
    }
}

Solution LPQColoring::buildSolution(double alpha) {
    auto n = graph->getNumVertices();
    Solution sol(n);
    
    // Usa vetor de índices e swap-and-pop para remoção O(1)
    vector<int> uncolored(n);
    iota(uncolored.begin(), uncolored.end(), 0);
    
    // Array de flags para acesso rápido
    vector<bool> isColored(n, false);
    
    while (!uncolored.empty()) {
        vector<pair<double, int>> costs;
        costs.reserve(uncolored.size());
        
        // Calcula custos para todos os vértices não coloridos
        for (auto v : uncolored) {
            costs.push_back({calculateVertexCost(v, sol.coloring), v});
        }
        
        // Ordena por custo (menor custo = melhor candidato)
        sort(costs.begin(), costs.end());
        
        // Seleciona vértice da RCL
        auto chosenVertex = selectVertexFromRCL(costs, alpha);
        
        // Colore o vértice escolhido
        auto color = findSmallestValidColor(chosenVertex, sol.coloring);
        sol.coloring[chosenVertex] = color;
        sol.maxColor = max(sol.maxColor, color);
        isColored[chosenVertex] = true;
        
        // Remove vértice colorido usando swap-and-pop O(1)
        for (size_t i = 0; i < uncolored.size(); i++) {
            if (uncolored[i] == chosenVertex) {
                uncolored[i] = uncolored.back();
                uncolored.pop_back();
                break;
            }
        }
    }
    
    return sol;
}

Solution LPQColoring::buildSolutionIncremental(double alpha) {
    auto n = graph->getNumVertices();
    Solution sol(n);
    
    // Estruturas para otimização incremental
    vector<int> uncolored(n);
    iota(uncolored.begin(), uncolored.end(), 0);
    
    vector<bool> isColored(n, false);
    vector<bool> needsUpdate(n, true);  // Marca vértices que precisam recalcular custo
    vector<double> cachedCosts(n, 0.0);
    
    // Calcula custos iniciais
    for (auto v : uncolored) {
        cachedCosts[v] = calculateVertexCost(v, sol.coloring);
    }
    
    while (!uncolored.empty()) {
        vector<pair<double, int>> costs;
        costs.reserve(uncolored.size());
        
        // Usa custos em cache ou recalcula se necessário
        for (auto v : uncolored) {
            if (needsUpdate[v]) {
                cachedCosts[v] = calculateVertexCost(v, sol.coloring);
                needsUpdate[v] = false;
            }
            costs.push_back({cachedCosts[v], v});
        }
        
        sort(costs.begin(), costs.end());
        auto chosenVertex = selectVertexFromRCL(costs, alpha);
        
        // Colore o vértice
        auto color = findSmallestValidColor(chosenVertex, sol.coloring);
        sol.coloring[chosenVertex] = color;
        sol.maxColor = max(sol.maxColor, color);
        isColored[chosenVertex] = true;
        
        // Marca vizinhos para atualização (afetados pela coloração)
        for (auto neighbor : graph->getNeighbors(chosenVertex)) {
            if (!isColored[neighbor]) {
                needsUpdate[neighbor] = true;
            }
        }
        for (auto neighbor2 : graph->getDistance2Neighbors(chosenVertex)) {
            if (!isColored[neighbor2]) {
                needsUpdate[neighbor2] = true;
            }
        }
        
        // Remove usando swap-and-pop
        for (size_t i = 0; i < uncolored.size(); i++) {
            if (uncolored[i] == chosenVertex) {
                uncolored[i] = uncolored.back();
                uncolored.pop_back();
                break;
            }
        }
    }
    
    return sol;
}

Solution LPQColoring::greedy() {
    auto n = graph->getNumVertices();
    Solution sol(n);
    
    // Ordena vértices por grau + grau de saturação combinados
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 0);
    
    // Ordena inicialmente por grau decrescente
    sort(vertices.begin(), vertices.end(), [this](int a, int b) {
        auto degreeA = graph->getDegree(a) + graph->getDistance2Neighbors(a).size();
        auto degreeB = graph->getDegree(b) + graph->getDistance2Neighbors(b).size();
        return degreeA > degreeB;
    });
    
    // Colore cada vértice com a menor cor válida
    for (auto v : vertices) {
        auto color = findSmallestValidColor(v, sol.coloring);
        sol.coloring[v] = color;
        sol.maxColor = max(sol.maxColor, color);
    }
    
    return sol;
}

Solution LPQColoring::greedyRandomized(double alpha, int iterations) {
    auto n = graph->getNumVertices();
    Solution bestSol(n);
    bestSol.maxColor = INT32_MAX;
    
    for (auto iter = 0; iter < iterations; iter++) {
        auto sol = buildSolution(alpha);
        
        // Atualiza melhor solução
        if (sol.maxColor < bestSol.maxColor) {
            bestSol = sol;
        }
    }
    
    // Aplica busca local na melhor solução
    bestSol = localSearch(bestSol, 50);
    
    return bestSol;
}

Solution LPQColoring::greedyRandomizedReactive(const vector<double>& alphas, int iterations, int blockSize) {
    auto n = graph->getNumVertices();
    Solution bestSol(n);
    bestSol.maxColor = INT32_MAX;
    
    auto numAlphas = alphas.size();
    vector<double> probabilities(numAlphas, 1.0 / numAlphas);
    vector<double> blockQuality(numAlphas, 0.0);
    vector<size_t> blockUsage(numAlphas, 0);
    
    auto iterationsInBlock = 0;
    
    for (auto iter = 0; iter < iterations; iter++) {
        // Seleciona alpha baseado nas probabilidades
        discrete_distribution<int> dist(probabilities.begin(), probabilities.end());
        auto alphaIndex = dist(rng);
        
        // Constrói solução usando o alpha selecionado (versão incremental)
        auto sol = buildSolutionIncremental(alphas[alphaIndex]);
        
        // Atualiza estatísticas do bloco
        auto quality = 1.0 / (1.0 + sol.maxColor);
        blockQuality[alphaIndex] += quality;
        blockUsage[alphaIndex]++;
        iterationsInBlock++;
        
        // Atualiza melhor solução
        if (sol.maxColor < bestSol.maxColor) {
            bestSol = sol;
        }
        
        // Atualiza probabilidades ao final de cada bloco
        if (iterationsInBlock >= blockSize) {
            updateProbabilities(probabilities, blockQuality, blockUsage);
            
            // Reset para próximo bloco
            fill(blockQuality.begin(), blockQuality.end(), 0.0);
            fill(blockUsage.begin(), blockUsage.end(), 0);
            iterationsInBlock = 0;
        }
    }
    
    // Aplica busca local na melhor solução
    bestSol = localSearch(bestSol, 100);
    
    return bestSol;
}

bool LPQColoring::isSolutionValid(const Solution& sol) const {
    int n = graph->getNumVertices();
    
    // Verifica se todos os vértices foram coloridos
    for (auto i = 0; i < n; i++) {
        if (sol.coloring[i] == -1) {
            cerr << "Erro: vértice " << i << " não foi colorido" << endl;
            return false;
        }
    }
    
    // Verifica restrições
    for (auto v = 0; v < n; v++) {
        // Verifica vizinhos diretos
        for (auto neighbor : graph->getNeighbors(v)) {
            auto diff = abs(sol.coloring[v] - sol.coloring[neighbor]);
            if (diff < p) {
                cerr << "Erro: vértices adjacentes " << v << " e " << neighbor 
                          << " violam restrição p" << endl;
                return false;
            }
        }
        
        // Verifica vizinhos a distância 2
        for (auto neighbor2 : graph->getDistance2Neighbors(v)) {
            auto diff = abs(sol.coloring[v] - sol.coloring[neighbor2]);
            if (diff < q) {
                cerr << "Erro: vértices a distância 2 " << v << " e " << neighbor2 
                          << " violam restrição q" << endl;
                return false;
            }
        }
    }
    
    return true;
}
