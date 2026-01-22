#ifndef UTILS_H
#define UTILS_H

#include "LPQColoring.h"
#include <string>
#include <chrono>

// Estrutura para armazenar resultados de execução
struct ExecutionResult {
    std::string datetime;
    std::string instance;
    int p;
    int q;
    std::string algorithm;
    std::string alpha;  // Mudado para string para suportar múltiplos alphas
    int iterations;
    int blockSize;
    unsigned int seed;
    double executionTime;
    int bestSolution;
    double avgSolution;
    double bestAlpha;
    
    ExecutionResult() : p(0), q(0), alpha(""), iterations(0), blockSize(0), 
                        seed(0), executionTime(0.0), bestSolution(0), 
                        avgSolution(0.0), bestAlpha(0.0) {}
};

class Utils {
public:
    // Gera timestamp atual
    static std::string getCurrentDateTime();
    
    // Salva resultado em CSV
    static void saveResultToCSV(const ExecutionResult& result, const std::string& filename);
    
    // Cria header do CSV se necessário
    static void createCSVHeaderIfNeeded(const std::string& filename);
    
    // Imprime solução em formato visualizável
    static void printSolution(const Solution& sol, const std::string& filename = "");
    
    // Gera semente baseada em data/hora
    static unsigned int generateSeed();
    
    // Mede tempo de execução de uma função
    template<typename Func>
    static double measureExecutionTime(Func func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        return diff.count();
    }
};

#endif
