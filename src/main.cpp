#include "../include/Graph.h"
#include "../include/LPQColoring.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>

void printUsage(const char* programName) {
    std::cout << "\n=== L(p,q)-Coloring Solver ===" << std::endl;
    std::cout << "\nUso: " << programName << " <opcoes>" << std::endl;
    std::cout << "\nOpções obrigatórias:" << std::endl;
    std::cout << "  -f <arquivo>     Arquivo de instância" << std::endl;
    std::cout << "  -p <valor>       Valor de p (diferença mínima para adjacentes)" << std::endl;
    std::cout << "  -q <valor>       Valor de q (diferença mínima para distância 2)" << std::endl;
    std::cout << "  -a <algoritmo>   Algoritmo: greedy | randomized | reactive" << std::endl;
    std::cout << "\nOpções para algoritmo randomized:" << std::endl;
    std::cout << "  -alpha <valor>   Valor de alpha (ex: 0.1, 0.3, 0.5)" << std::endl;
    std::cout << "  -i <valor>       Número de iterações (padrão: 30)" << std::endl;
    std::cout << "\nOpções para algoritmo reactive:" << std::endl;
    std::cout << "  -alphas <v1,v2,v3>  Lista de alphas separados por vírgula (ex: 0.1,0.3,0.5)" << std::endl;
    std::cout << "  -i <valor>          Número de iterações (padrão: 300)" << std::endl;
    std::cout << "  -b <valor>          Tamanho do bloco (padrão: 30)" << std::endl;
    std::cout << "\nOpções gerais:" << std::endl;
    std::cout << "  -s <seed>        Semente para randomização (padrão: baseada em tempo)" << std::endl;
    std::cout << "  -o <arquivo>     Arquivo de saída para solução (padrão: não salva)" << std::endl;
    std::cout << "  -csv <arquivo>   Arquivo CSV para resultados (padrão: results/results.csv)" << std::endl;
    std::cout << "\nExemplos:" << std::endl;
    std::cout << "  " << programName << " -f instances/grafo.col -p 2 -q 1 -a greedy" << std::endl;
    std::cout << "  " << programName << " -f instances/grafo.col -p 2 -q 1 -a randomized -alpha 0.3 -i 50" << std::endl;
    std::cout << "  " << programName << " -f instances/grafo.col -p 2 -q 1 -a reactive -alphas 0.1,0.3,0.5 -i 300 -b 30" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // Parâmetros
    std::string filename = "";
    int p = -1, q = -1;
    std::string algorithm = "";
    double alpha = 0.3;
    std::vector<double> alphas = {0.1, 0.3, 0.5};
    int iterations = -1;
    int blockSize = 30;
    unsigned int seed = 0;
    bool seedProvided = false;
    std::string outputFile = "";
    std::string csvFile = "results/results.csv";
    
    // Parse argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            p = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            q = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        }
        else if (strcmp(argv[i], "-alpha") == 0 && i + 1 < argc) {
            alpha = std::stod(argv[++i]);
        }
        else if (strcmp(argv[i], "-alphas") == 0 && i + 1 < argc) {
            alphas.clear();
            std::string alphasStr = argv[++i];
            size_t pos = 0;
            while ((pos = alphasStr.find(',')) != std::string::npos) {
                alphas.push_back(std::stod(alphasStr.substr(0, pos)));
                alphasStr.erase(0, pos + 1);
            }
            alphas.push_back(std::stod(alphasStr));
        }
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            iterations = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            blockSize = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = std::stoul(argv[++i]);
            seedProvided = true;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (strcmp(argv[i], "-csv") == 0 && i + 1 < argc) {
            csvFile = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        }
    }
    
    // Validação de parâmetros obrigatórios
    if (filename.empty() || p < 0 || q < 0 || algorithm.empty()) {
        std::cerr << "Erro: Parâmetros obrigatórios faltando!" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Define iterações padrão baseado no algoritmo
    if (iterations == -1) {
        if (algorithm == "randomized") {
            iterations = 30;
        } else if (algorithm == "reactive") {
            iterations = 300;
        }
    }
    
    // Gera semente se não fornecida
    if (!seedProvided) {
        seed = Utils::generateSeed();
    }
    
    std::cout << "\n=== L(p,q)-Coloring Solver ===" << std::endl;
    std::cout << "Instância: " << filename << std::endl;
    std::cout << "p = " << p << ", q = " << q << std::endl;
    std::cout << "Algoritmo: " << algorithm << std::endl;
    std::cout << "Semente: " << seed << std::endl;
    
    // Lê o grafo
    Graph* graph = Graph::readFromFile(filename);
    if (graph == nullptr) {
        std::cerr << "Erro ao ler o grafo!" << std::endl;
        return 1;
    }
    
    // Cria solver
    LPQColoring solver(graph, p, q, seed);
    
    // Executa algoritmo
    Solution solution;
    double executionTime = 0.0;
    
    if (algorithm == "greedy") {
        std::cout << "\nExecutando algoritmo guloso..." << std::endl;
        executionTime = Utils::measureExecutionTime([&]() {
            solution = solver.greedy();
        });
    }
    else if (algorithm == "randomized") {
        std::cout << "\nExecutando algoritmo guloso randomizado..." << std::endl;
        std::cout << "Alpha = " << alpha << std::endl;
        std::cout << "Iterações = " << iterations << std::endl;
        executionTime = Utils::measureExecutionTime([&]() {
            solution = solver.greedyRandomized(alpha, iterations);
        });
    }
    else if (algorithm == "reactive") {
        std::cout << "\nExecutando algoritmo guloso randomizado reativo..." << std::endl;
        std::cout << "Alphas = ";
        for (size_t i = 0; i < alphas.size(); i++) {
            std::cout << alphas[i];
            if (i < alphas.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        std::cout << "Iterações = " << iterations << std::endl;
        std::cout << "Tamanho do bloco = " << blockSize << std::endl;
        executionTime = Utils::measureExecutionTime([&]() {
            solution = solver.greedyRandomizedReactive(alphas, iterations, blockSize);
        });
    }
    else {
        std::cerr << "Algoritmo inválido: " << algorithm << std::endl;
        std::cerr << "Use: greedy, randomized ou reactive" << std::endl;
        delete graph;
        return 1;
    }
    
    // Verifica validade da solução
    std::cout << "\nVerificando validade da solução..." << std::endl;
    bool valid = solver.isSolutionValid(solution);
    
    // Resultados
    std::cout << "\n=== Resultados ===" << std::endl;
    std::cout << "Tempo de execução: " << executionTime << " segundos" << std::endl;
    std::cout << "Maior cor utilizada: " << solution.maxColor << std::endl;
    std::cout << "Solução válida: " << (valid ? "SIM" : "NÃO") << std::endl;
    
    // Salva solução em arquivo se especificado
    if (!outputFile.empty()) {
        Utils::printSolution(solution, outputFile);
    }
    
    // Salva resultados em CSV
    ExecutionResult result;
    result.datetime = Utils::getCurrentDateTime();
    result.instance = filename;
    result.p = p;
    result.q = q;
    result.algorithm = algorithm;
    
    // Preenche coluna alpha apropriadamente para cada algoritmo
    if (algorithm == "randomized") {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << alpha;
        result.alpha = oss.str();
    } else if (algorithm == "reactive") {
        // Para reactive, mostra todos os alphas usados
        std::ostringstream oss;
        for (size_t i = 0; i < alphas.size(); i++) {
            oss << std::fixed << std::setprecision(2) << alphas[i];
            if (i < alphas.size() - 1) oss << ";";
        }
        result.alpha = oss.str();
    } else {
        result.alpha = "N/A";
    }
    
    result.iterations = iterations;
    result.blockSize = (algorithm == "reactive") ? blockSize : 0;
    result.seed = seed;
    result.executionTime = executionTime;
    result.bestSolution = solution.maxColor;
    result.avgSolution = solution.maxColor;  // Para uma única execução, avg = best
    result.bestAlpha = (algorithm == "randomized") ? alpha : 
                       (algorithm == "reactive" && !alphas.empty()) ? alphas[0] : 0.0;
    
    Utils::saveResultToCSV(result, csvFile);
    std::cout << "\nResultados salvos em: " << csvFile << std::endl;
    
    // Cleanup
    delete graph;
    
    return 0;
}
