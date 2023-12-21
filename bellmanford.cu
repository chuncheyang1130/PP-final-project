#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "CycleTimer.h"
#include <cuda.h>

#define INF 1e9

struct Edge{
    int src;
    int dst;
    int weight;
};

__global__ BellmanFordKernel(std::vector<Edge>& edges, int srcNode, int dstNode, int numNodes){

    std::vector<int> dist(numNodes, INF);

    int N = numNodes - 1;
    dist[srcNode] = 0;

    int edge_num = edges.size();

    // while (N--){
    //     for (const auto& edge : edges){
    //         if (dist[edge.src] != INF)
    //             dist[edge.dst] = std::min(dist[edge.src] + edge.weight, dist[edge.dst]);
    //     }
    // }

    omp_set_num_threads(4);

    std::vector<std::vector<int>> local_dist(4, std::vector<int>(dist));

    while (N--) {

        // for(int i = 0; i < numNodes; i++){
        //     std::printf("local_dist[%d] = %d\n", i, local_dist[0][i]);
        // }
        // std::printf("\n");

        #pragma omp parallel
        {
            int id = omp_get_thread_num();            

            #pragma omp for
            for (int i = 0; i < edge_num; i++){
                Edge edge = edges[i];

                if (local_dist[id][edge.src] != INF){
                    local_dist[id][edge.dst] = std::min(local_dist[id][edge.src] + edge.weight, local_dist[id][edge.dst]);
                }      
            }
            
            #pragma omp for
            for (int i = 0; i < numNodes; i++){
                dist[i] = std::min({local_dist[0][i], local_dist[1][i], local_dist[2][i], local_dist[3][i]});
                // std::printf("dist[%d] = %d\n", i, dist[i]);
            }

            for (int i = 0; i < numNodes; i++){
                local_dist[id][i] = dist[i];
            }
            
        }

        
    }

    // #pragma omp parallel for
    for (int i = 0; i < edge_num; i++){
        Edge edge = edges[i];
        if (dist[edge.src] != INF && dist[edge.dst] > dist[edge.src] + edge.weight){
            std::printf("NEGATIVE CYCLE !!\n");
            return -1;
        }
    }

    return dist[dstNode];
    
}

int BellmanFord(Edge* edges, int srcNode, int dstNode, int numNodes, int numEdges){
    Edge* deviceArray;
    cudaMalloc(&deviceArray, numEdges * sizeof(struct Edge));

    
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./bellmanford_thread file.txt srcNode dstNode\n");
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
    std::printf("[numNode]: %d [numEdges]: %d\n", numNodes, numEdges);

    Edge* edges = (Edge*)malloc(numEdges * 2 * sizeof(struct Edge));

    int srcNode = std::atoi(argv[2]);
    int dstNode = std::atoi(argv[3]);
    std::printf("[srcNode]: %u [dstNode]: %u\n", srcNode, dstNode);

    if (srcNode >= numNodes || dstNode >= numNodes){
        std::printf("[Error] Given src or dst node is in the graph\n");
        return 1;
    }

    int source, target, weight;
    for (int i = 0; i < numEdges; i++) {
        ifs >> source >> target >> weight;

        // Edge e1 = {source, target, weight};
        // edges.push_back(e1);
        edges[i*2].src = source;
        edges[i*2+1].src = source;

        edges[i*2].dst = target;
        edges[i*2+1].dst = target;

        edges[i*2].weight = weight;
        edges[i*2+1].weight = weight;

        // Edge e2 = {target, source, weight};
        // edges.push_back(e2);
    }

    std::printf("Successfully construct Edge vector\n");
    ifs.close();

    double startTime = CycleTimer::currentSeconds();
    int minDist = BellmanFord(edges, srcNode, dstNode, numNodes, numEdges*2);
    double endTime = CycleTimer::currentSeconds();

    std::printf("[BellmanFord thread]:\t\t[%lf] ms\n", (endTime - startTime) * 1000);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}