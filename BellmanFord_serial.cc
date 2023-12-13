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
        std::printf("usage: ./BellmanFord file.txt node_num edge_num");
        return 1;
    }

    unsigned int numNodes = std::atoi(argv[2]);
    unsigned int numEdges = std::atoi(argv[3]);
    std::printf("[Nodes]: %u [Edges]: %u\n", numNodes, numEdges);
    
    std::vector<Edge> edges;
    
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
        Edge e = {source, target, weight};
        edges.push_back(e);
    }

    std::printf("Successfully construct adjacency matrix\n");
    ifs.close();

    int minDist = BellmanFord(edges, srcNode, dstNode, numNodes);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}