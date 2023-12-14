#include <vector>
#include <string>
#include <climits>
#include <fstream>
#include <iostream>
#include "CycleTimer.h"

int minimumDistance(std::vector<int> dist, std::vector<bool> visited) {
    int min = INT_MAX, min_index;
    for (std::size_t v = 0; v < dist.size(); v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

int dijkstra(std::vector<std::vector<int>> adjMatrix, int srcNode, int dstNode, unsigned int numNodes) {
    std::vector<int> dist(numNodes);
    std::vector<bool> visited(numNodes);

    for (int i = 0; i < numNodes; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[srcNode] = 0;
    for (int i = 0; i < numNodes-1; i++) {
        
        int u = minimumDistance(dist, visited);
        visited[u] = true;
        
        for (int v = 0; v < numNodes; v++) {
            if (!visited[v] && adjMatrix[u][v] && dist[u] + adjMatrix[u][v] < dist[v])
                dist[v] = dist[u] + adjMatrix[u][v];
        }
    }
    return dist[dstNode];
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./dijkstra file.txt node_num edge_num");
        return 1;
    }
    unsigned int numNodes = std::atoi(argv[2]);
    unsigned int numEdges = std::atoi(argv[3]);
    std::printf("[Nodes]: %u [Edges]: %u\n", numNodes, numEdges);
    
    std::vector<std::vector<int>> adjMatrix(numNodes, std::vector<int>(numNodes, 0));
    
    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in);
    if (!ifs.good()) {
        std::printf("[Error] Cannot open file %s\n", argv[1]);
        return 1;
    }
    int srcNode, dstNode;
    ifs >> srcNode >> dstNode;
    std::printf("[srcNode]: %d [dstNode]: %d\n", srcNode, dstNode);
    int source, target, weight;
    for (unsigned int i = 0; i < numEdges; i++) {
        ifs >> source >> target >> weight;
        adjMatrix[source][target] = weight;
    }
    std::printf("Successfully construct adjacency matrix\n");
    ifs.close();
    
    double startTime = CycleTimer::currentSeconds();
    int minDist = dijkstra(adjMatrix, srcNode, dstNode, numNodes);
    double endTime = CycleTimer::currentSeconds();

    std::printf("[Dijkstra Serial]:\t\t[%lf] ms\n", endTime - startTime);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}