#include "../include/Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <queue>

Graph::Graph(int n) : numVertices(n) {
    adjList.resize(n);
    distance2Neighbors.resize(n);
}

void Graph::addEdge(int u, int v) {
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
        std::cerr << "Erro: vértices inválidos (" << u << ", " << v << ")" << std::endl;
        return;
    }
    
    // Evita duplicatas
    if (std::find(adjList[u].begin(), adjList[u].end(), v) == adjList[u].end()) {
        adjList[u].push_back(v);
    }
    if (std::find(adjList[v].begin(), adjList[v].end(), u) == adjList[v].end()) {
        adjList[v].push_back(u);
    }
}

int Graph::getNumVertices() const {
    return numVertices;
}

const std::vector<int>& Graph::getNeighbors(int v) const {
    return adjList[v];
}

const std::set<int>& Graph::getDistance2Neighbors(int v) const {
    return distance2Neighbors[v];
}

int Graph::getDegree(int v) const {
    return adjList[v].size();
}

void Graph::computeDistance2Neighbors() {
    // Para cada vértice, encontra vizinhos a distância 2
    for (int v = 0; v < numVertices; v++) {
        distance2Neighbors[v].clear();
        
        // Para cada vizinho direto
        for (int neighbor : adjList[v]) {
            // Para cada vizinho do vizinho
            for (int neighbor2 : adjList[neighbor]) {
                // Se não é o próprio vértice e não é vizinho direto
                if (neighbor2 != v && 
                    std::find(adjList[v].begin(), adjList[v].end(), neighbor2) == adjList[v].end()) {
                    distance2Neighbors[v].insert(neighbor2);
                }
            }
        }
    }
}

void Graph::printGraph() const {
    std::cout << "Grafo com " << numVertices << " vértices:" << std::endl;
    for (int i = 0; i < numVertices; i++) {
        std::cout << "Vértice " << i << " -> ";
        for (int neighbor : adjList[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

Graph* Graph::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return nullptr;
    }
    
    std::string line;
    int numVertices = 0;
    int numEdges = 0;
    Graph* graph = nullptr;
    
    while (std::getline(file, line)) {
        // Ignora linhas vazias e comentários
        if (line.empty() || line[0] == 'c') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "p") {
            // Linha de problema: p edge <numVertices> <numEdges>
            std::string format;
            iss >> format >> numVertices >> numEdges;
            graph = new Graph(numVertices);
            std::cout << "Lendo grafo: " << numVertices << " vértices, " 
                      << numEdges << " arestas" << std::endl;
        }
        else if (type == "e" && graph != nullptr) {
            // Linha de aresta: e <u> <v>
            int u, v;
            iss >> u >> v;
            // Formato DIMACS usa índices 1-based, convertemos para 0-based
            graph->addEdge(u - 1, v - 1);
        }
    }
    
    file.close();
    
    if (graph != nullptr) {
        graph->computeDistance2Neighbors();
        std::cout << "Grafo lido com sucesso!" << std::endl;
    }
    
    return graph;
}
