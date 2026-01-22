#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <set>

class Graph {
private:
    int numVertices;
    std::vector<std::vector<int>> adjList;  // Lista de adjacência
    std::vector<std::set<int>> distance2Neighbors;  // Vizinhos a distância 2
    
    void computeDistance2Neighbors();

public:
    Graph(int n);
    
    void addEdge(int u, int v);
    int getNumVertices() const;
    const std::vector<int>& getNeighbors(int v) const;
    const std::set<int>& getDistance2Neighbors(int v) const;
    
    // Métodos auxiliares
    int getDegree(int v) const;
    void printGraph() const;
    
    // Leitura de arquivo
    static Graph* readFromFile(const std::string& filename);
};

#endif
