#include <vector>
#include <string>
#include <climits>
#include <fstream>
#include <iostream>
#include "CycleTimer.h"

#define ITER_NUM 10

int minimumDistance(std::vector<int> dist, std::vector<bool> visited) {
    int min = INT_MAX, min_index = 0;
    for (std::size_t v = 0; v < dist.size(); v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

int dijkstra(std::vector<std::vector<int>> adjMatrix, int srcNode, int dstNode, int numNodes) {
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
        std::printf("usage: ./dijkstra_serial file.txt srcNode dstNode\n");
        return 1;
    }
    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in);
    if (!ifs.good()) {
        std::printf("[Error] Cannot open file %s\n", argv[1]);
        return 1;
    }

    int numNodes, numEdges;
    ifs >> numNodes >> numEdges;
    std::printf("[numNodes]: %d [numEdges]: %d\n", numNodes, numEdges);
    std::vector<std::vector<int>> adjMatrix(numNodes, std::vector<int>(numNodes, 0));
    
    int srcNode = std::atoi(argv[2]);
    int dstNode = std::atoi(argv[3]);
    std::printf("[srcNode]: %d [dstNode]: %d\n", srcNode, dstNode);
    
    if (srcNode >= numNodes || dstNode >= numNodes){
        std::printf("[Error] Given src or dst node is in the graph\n");
        return 1;
    }

    int source, target, weight;
    for (int i = 0; i < numEdges; i++) {
        ifs >> source >> target >> weight;
        adjMatrix[source][target] = weight;
        adjMatrix[target][source] = weight;
    }
    std::printf("Successfully construct adjacency matrix\n");
    ifs.close();
    
    double avgTime = 0.0f;
    int minDist;
    for (int i = 0; i < ITER_NUM; i++) {
        double startTime = CycleTimer::currentSeconds();
        minDist = dijkstra(adjMatrix, srcNode, dstNode, numNodes);
        double endTime = CycleTimer::currentSeconds();
        avgTime += (endTime - startTime);
    }
    avgTime /= ITER_NUM;

    std::printf("[Dijkstra Serial]:\t\t[%lf] ms\n", avgTime * 1000);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}