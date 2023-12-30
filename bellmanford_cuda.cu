#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "CycleTimer.h"
#include <cuda.h>

#define INF 1e9
#define ITER_NUM 10

struct Edge{
    int src;
    int dst;
    int weight;
};

__global__ void BellmanFordKernel(Edge* edges, int* dist, int* parent, int numNodes, int numEdges){

    int threadId = blockIdx.x * blockDim.x + threadIdx.x;

    if (threadId >= numEdges)
        return;

    Edge edge = edges[threadId];
    // printf("threadId: %d\n", threadId);

    if (dist[edge.src] != INF && dist[edge.src] + edge.weight < dist[edge.dst]){
        atomicMin(&dist[edge.dst], dist[edge.src] + edge.weight);

        int oldParent = atomicCAS(&parent[edge.dst], parent[edge.dst], edge.src);
    }
        
}

int BellmanFord(Edge* edges, int srcNode, int dstNode, int numNodes, int numEdges, bool printRoute){
    Edge* deviceEdgeArray;
    cudaMalloc(&deviceEdgeArray, numEdges * sizeof(struct Edge));
    cudaMemcpy(deviceEdgeArray, edges, numEdges * sizeof(struct Edge), cudaMemcpyHostToDevice);
    
    int* dist = (int*)malloc(numNodes * sizeof(int));
    int* parent = (int*)malloc(numNodes * sizeof(int));
    
    for (int i = 0; i < numNodes; i++){
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[srcNode] = 0;

    int* deviceDistArray;
    cudaMalloc(&deviceDistArray, numNodes * sizeof(int));
    cudaMemcpy(deviceDistArray, dist, numNodes * sizeof(int), cudaMemcpyHostToDevice);

    int* deviceParentArray;
    cudaMalloc(&deviceParentArray, numNodes * sizeof(int));
    cudaMemcpy(deviceParentArray, parent, numNodes * sizeof(int), cudaMemcpyHostToDevice);
    
    int N = numNodes - 1;
    int threadPerBlock = 64;
    int numBlocks = (numEdges + threadPerBlock - 1) / threadPerBlock;

    while(N--){
        BellmanFordKernel<<<numBlocks, threadPerBlock>>>(deviceEdgeArray, deviceDistArray, deviceParentArray, numNodes, numEdges);
    }

    cudaMemcpy(dist, deviceDistArray, numNodes * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < numEdges; i++){
        Edge edge = edges[i];
        if (dist[edge.src] != INF && dist[edge.dst] > dist[edge.src] + edge.weight){
            std::printf("NEGATIVE CYCLE !!\n");
            return -1;
        }
    }

    cudaFree(deviceEdgeArray);
    cudaFree(deviceDistArray);
    
    int min_dist = dist[dstNode];
    free(dist);

    if (printRoute){
        int* route = (int*)malloc(numNodes * sizeof(int));
        int numRouteNodes = 1;
        route[0] = dstNode;

        int curNode = dstNode;
        
        while(curNode != srcNode){
            route[numRouteNodes] = parent[curNode];
            curNode = parent[curNode];
            numRouteNodes++;
        }

        std::printf("The shortest path is:\n");
    
        for (int i = numRouteNodes-1; i >= 1; i--){
            std::printf("%d -> ", route[i]);
        }

        std::printf("%d\n", route[0]);

        free(route);
    }

    return min_dist;
    
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./bellmanford_cuda file.txt srcNode dstNode\n");
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
        edges[i*2+1].src = target;

        edges[i*2].dst = target;
        edges[i*2+1].dst = source;

        edges[i*2].weight = weight;
        edges[i*2+1].weight = weight;

        // Edge e2 = {target, source, weight};
        // edges.push_back(e2);
    }

    std::printf("Successfully construct Edge vector\n");
    ifs.close();

    double avgTime = 0.0;
    int minDist = 0;
    bool printRoute = false;

    for (int i = 0; i < ITER_NUM; i++){
        double startTime = CycleTimer::currentSeconds();
        minDist = BellmanFord(edges, srcNode, dstNode, numNodes, numEdges*2, printRoute);
        double endTime = CycleTimer::currentSeconds();

        avgTime += endTime - startTime;
    }

    avgTime /= ITER_NUM;

    printRoute = true;
    std::printf("[BellmanFord cuda]:\t\t[%lf] ms\n", avgTime * 1000);
    minDist = BellmanFord(edges, srcNode, dstNode, numNodes, numEdges*2, printRoute);
    std::printf("The minimum distance from %d to %d is: %d\n\n", srcNode, dstNode, minDist);
    
    return 0;
}