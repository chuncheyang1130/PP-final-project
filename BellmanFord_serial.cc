#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#define INF 1e9

struct Edge{
    int src;
    int dst;
    int weight;
};

int BellmanFord(std::vector<Edge>& edges, int srcNode, int dstNode, int numNodes){

    std::vector<int> dist(numNodes, INF);

    int N = numNodes - 1;
    dist[srcNode] = 0;

    while (N--){
        for (const auto& edge : edges){
            if (dist[edge.src] != INF)
                dist[edge.dst] = std::min(dist[edge.src] + edge.weight, dist[edge.dst]);
        }
    }

    for (const auto& edge : edges){
        if (dist[edge.src] != INF && dist[edge.dst] > dist[edge.src] + edge.weight){
            std::printf("NEGATIVE CYCLE !!\n");
            return -1;
        }
    }

    return dist[dstNode];
    
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./BellmanFord file.txt srcNode dstNode");
        return 1;
    }

    std::vector<Edge> edges;
    
    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in);
    if (!ifs.good()) {
        std::printf("[Error] Cannot open file %s\n", argv[1]);
        return 1;
    }

    int numNodes, numEdges;
    ifs >> numNodes >> numEdges;
    std::printf("[numNode]: %d [numEdges]: %d\n", numNodes, numEdges);

    int srcNode = std::atoi(argv[2]);
    int dstNode = std::atoi(argv[3]);
    std::printf("[srcNode]: %u [Edges]: %u\n", srcNode, dstNode);

    int source, target, weight;
    for (unsigned int i = 0; i < numEdges; i++) {
        ifs >> source >> target >> weight;
        Edge e1 = {source, target, weight};
        edges.push_back(e1);

        Edge e2 = {source, target, weight};
        edges.push_back(e2);
    }

    std::printf("Successfully construct adjacency matrix\n");
    ifs.close();

    int minDist = BellmanFord(edges, srcNode, dstNode, numNodes);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}