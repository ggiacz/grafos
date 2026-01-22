#include "../include/Utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sys/stat.h>

std::string Utils::getCurrentDateTime() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

void Utils::createCSVHeaderIfNeeded(const std::string& filename) {
    struct stat buffer;
    bool fileExists = (stat(filename.c_str(), &buffer) == 0);
    
    if (!fileExists) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "datetime,instance,p,q,algorithm,alpha,iterations,blockSize,seed,executionTime,bestSolution,avgSolution,bestAlpha\n";
            file.close();
        }
    }
}

void Utils::saveResultToCSV(const ExecutionResult& result, const std::string& filename) {
    createCSVHeaderIfNeeded(filename);
    
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo CSV: " << filename << std::endl;
        return;
    }
    
    file << result.datetime << ","
         << result.instance << ","
         << result.p << ","
         << result.q << ","
         << result.algorithm << ","
         << result.alpha << ","
         << result.iterations << ","
         << result.blockSize << ","
         << result.seed << ","
         << std::fixed << std::setprecision(6) << result.executionTime << ","
         << result.bestSolution << ","
         << std::fixed << std::setprecision(2) << result.avgSolution << ","
         << std::fixed << std::setprecision(3) << result.bestAlpha << "\n";
    
    file.close();
}

void Utils::printSolution(const Solution& sol, const std::string& filename) {
    if (filename.empty()) {
        // Imprime na tela
        std::cout << "\n=== Solução ===" << std::endl;
        std::cout << "Maior cor utilizada: " << sol.maxColor << std::endl;
        std::cout << "\nColoração dos vértices:" << std::endl;
        for (size_t i = 0; i < sol.coloring.size(); i++) {
            std::cout << "Vértice " << i << ": cor " << sol.coloring[i] << std::endl;
        }
    } else {
        // Salva em arquivo
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "Maior cor utilizada: " << sol.maxColor << "\n\n";
            file << "Coloração (formato para csacademy.com/app/grapheditor/):\n";
            file << "# Vértices: " << sol.coloring.size() << "\n";
            for (size_t i = 0; i < sol.coloring.size(); i++) {
                file << i << ": " << sol.coloring[i] << "\n";
            }
            file.close();
            std::cout << "Solução salva em: " << filename << std::endl;
        }
    }
}

unsigned int Utils::generateSeed() {
    return static_cast<unsigned int>(time(nullptr));
}
