#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "CycleTimer.h"
#include <omp.h>
#include <mpi.h>

#define INF 1e9
#define LEN 4

struct Edge{
    int src;
    int dst;
    int weight;
};

int BellmanFord(Edge* edges, int srcNode, int dstNode, int numNodes, int numEdges){

    int world_rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int* dist = (int*)malloc(numNodes * sizeof(int));

    int N = numNodes - 1;

    for(int i = 0; i < numNodes; i++)
        dist[i] = INF;

    dist[srcNode] = 0;

    int edge_per_rank = (numEdges + world_size - 1) / world_size;
    int start_ind = world_rank * edge_per_rank;
    int end_ind = start_ind + edge_per_rank > numEdges ? numEdges : start_ind + edge_per_rank;


    while (N--) {

        for (int i = start_ind; i < end_ind; i++){
            Edge edge = edges[i];

            if (dist[edge.src] != INF){
                dist[edge.dst] = std::min(dist[edge.dst], dist[edge.src] + edge.weight);
            }      
        }

        MPI_Allreduce(dist, dist, numNodes, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        
    }

    int has_neg_cycle = 0;

    for (int i = start_ind; i < end_ind; i++){
        Edge edge = edges[i];
        if (dist[edge.src] != INF && dist[edge.dst] > dist[edge.src] + edge.weight){
            std::printf("NEGATIVE CYCLE !!\n");
            has_neg_cycle = 1;
            break;
        }
    }

    MPI_Allreduce(&has_neg_cycle, &has_neg_cycle, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD); 

    if (has_neg_cycle)
        return -1;

    return dist[dstNode];
    
}

void createEdges(int* numNodes_ptr, int* numEdges_ptr, Edge** edges_ptr, char* graph_file){
    int world_rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Status status;
    int src_rank = 0;
    int tag = 0;

    int numNodes, numEdges;

    if (world_rank == 0){

        std::ifstream ifs;
        ifs.open(graph_file, std::ifstream::in);
        if (!ifs.good()) {
            std::printf("[Error] Cannot open file %s\n", graph_file);
            // return 1;
        }

        ifs >> numNodes >> numEdges;

        *numNodes_ptr = numNodes;
        *numEdges_ptr = numEdges;

        *edges_ptr = (Edge*)malloc(numEdges * 2 * sizeof(struct Edge));

        Edge* edges = *edges_ptr;

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

        for (int dst_rank = 1; dst_rank < world_size; dst_rank++){
            MPI_Send(numNodes_ptr, 1, MPI_INT, dst_rank, tag, MPI_COMM_WORLD);
        }

        for (int dst_rank = 1; dst_rank < world_size; dst_rank++){
            MPI_Send(numEdges_ptr, 1, MPI_INT, dst_rank, tag, MPI_COMM_WORLD);
        }

    }else{

        MPI_Recv(numNodes_ptr, 1, MPI_INT, src_rank, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(numEdges_ptr, 1, MPI_INT, src_rank, tag, MPI_COMM_WORLD, &status);

        numNodes = *numNodes_ptr;
        numEdges = *numEdges_ptr;        
    }


    if(world_rank == 0){

        MPI_Request requests[LEN];

        for(int dst_rank = 1; dst_rank < world_size; dst_rank++){
            MPI_Isend(*edges_ptr, numEdges, MPI_INT, dst_rank, tag, MPI_COMM_WORLD, &requests[dst_rank]);
        }

    }else{
        MPI_Request req;

        *edges_ptr = (Edge*)malloc(numEdges * 2 * sizeof(struct Edge));
        
        MPI_Irecv(*edges_ptr, numEdges, MPI_INT, src_rank, tag, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);

    }

}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./bellmanford_thread file.txt srcNode dstNode\n");
        return 1;
    }

    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    int numNodes = -1, numEdges = -1;
    Edge* edges;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    int srcNode = std::atoi(argv[2]);
    int dstNode = std::atoi(argv[3]);
    

    createEdges(&numNodes, &numEdges, &edges, argv[1]);

    if (world_rank == 0){
        std::printf("[numNodes]: %d [numEdges]: %d\n", numNodes, numEdges);
        std::printf("[srcNode]: %u [dstNode]: %u\n", srcNode, dstNode);

        if (srcNode >= numNodes || dstNode >= numNodes){
            std::printf("[Error] Given src or dst node is in the graph\n");
            return 1;
        }
    }
    
    double startTime = CycleTimer::currentSeconds();
    int minDist = BellmanFord(edges, srcNode, dstNode, numNodes, numEdges*2);
    double endTime = CycleTimer::currentSeconds();

    std::printf("[BellmanFord thread]:\t\t[%lf] ms\n", (endTime - startTime) * 1000);
    std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    
    return 0;
}