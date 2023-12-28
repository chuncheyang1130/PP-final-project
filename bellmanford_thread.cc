#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "CycleTimer.h"
#include <omp.h>

#define INF 1e9
#define ITER_NUM 10

struct Edge{
    int src;
    int dst;
    int weight;
};

int BellmanFord(Edge* edges, int srcNode, int dstNode, int numNodes, int numEdges){

    omp_set_num_threads(4);

    int* dist = (int*)malloc(numNodes * sizeof(int));

    int N = numNodes - 1;

    #pragma omp parallel for
    for(int i = 0; i < numNodes; i++)
        dist[i] = INF;

    dist[srcNode] = 0;

    // while (N--){
    //     for (const auto& edge : edges){
    //         if (dist[edge.src] != INF)
    //             dist[edge.dst] = std::min(dist[edge.src] + edge.weight, dist[edge.dst]);
    //     }
    // }

    int* local_dist[4];

    #pragma omp parallel
    {
        int id = omp_get_thread_num();

        local_dist[id] = (int*)malloc(numNodes * sizeof(int));

        memcpy(local_dist[id], dist, numNodes * sizeof(int));
    }

    while (N--) {

        // for(int i = 0; i < numNodes; i++){
        //     std::printf("local_dist[%d] = %d\n", i, local_dist[0][i]);
        // }
        // std::printf("\n");

        #pragma omp parallel
        {
            int id = omp_get_thread_num();            

            #pragma omp for
            for (int i = 0; i < numEdges; i++){
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

            memcpy(local_dist[id], dist, numNodes * sizeof(int));
            
        }

        
    }

    // #pragma omp parallel for
    for (int i = 0; i < numEdges; i++){
        Edge edge = edges[i];
        if (dist[edge.src] != INF && dist[edge.dst] > dist[edge.src] + edge.weight){
            std::printf("NEGATIVE CYCLE !!\n");
            return -1;
        }
    }

    return dist[dstNode];
    
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./bellmanford_thread file.txt srcNode dstNode\n");
        return 1;
    }

    Edge* edges;
    
    std::ifstream ifs;
    ifs.open(argv[1], std::ifstream::in);
    if (!ifs.good()) {
        std::printf("[Error] Cannot open file %s\n", argv[1]);
        return 1;
    }

    int numNodes, numEdges;
    ifs >> numNodes >> numEdges;
    std::printf("[numNodes]: %d [numEdges]: %d\n", numNodes, numEdges);

    edges = (Edge*)malloc(numEdges * 2 * sizeof(struct Edge));

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

        edges[i*2].src = source;
        edges[i*2].dst = target;
        edges[i*2].weight = weight;

        edges[i*2+1].src = target;
        edges[i*2+1].dst = source;
        edges[i*2+1].weight = weight;
    }

    std::printf("Successfully construct Edge vector\n");
    ifs.close();

    double avgTime = 0.0;
    int minDist = 0;

    for (int i = 0; i < ITER_NUM; i++){
        double startTime = CycleTimer::currentSeconds();
        minDist = BellmanFord(edges, srcNode, dstNode, numNodes, numEdges*2);
        double endTime = CycleTimer::currentSeconds();

        avgTime += endTime - startTime;
    }

    avgTime /= ITER_NUM;

    std::printf("[BellmanFord thread]:\t\t[%lf] ms\n", avgTime * 1000);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}