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
    // int* reduce_dist = (int*)malloc(numNodes * sizeof(int));

    int N = numNodes - 1;

    for(int i = 0; i < numNodes; i++)
        dist[i] = INF;

    dist[srcNode] = 0;

    int edge_per_rank = (numEdges + world_size - 1) / world_size;
    int start_ind = world_rank * edge_per_rank;
    int end_ind = start_ind + edge_per_rank > numEdges ? numEdges : start_ind + edge_per_rank;

    std::printf("rank: %d, start_ind: %d, end_ind: %d\n", world_rank, start_ind, end_ind);


    while (N--) {

        for (int i = start_ind; i < end_ind; i++){
            Edge edge = edges[i];

            if (dist[edge.src] != INF){
                dist[edge.dst] = std::min(dist[edge.dst], dist[edge.src] + edge.weight);
            }      
        }

        // std::printf("rank %d barrier\n", world_rank);

        MPI_Allreduce(MPI_IN_PLACE, dist, numNodes, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

        // if(world_rank == 0 && N == 0){
        //     for (int i = 0; i < numNodes; i++){
        //         std::printf("%d ", dist[i]);
        //     }
        //     std::printf("\n");
        // }
        

        // memcpy(dist, reduce_dist, numNodes * sizeof(int));
        
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

    MPI_Allreduce(MPI_IN_PLACE, &has_neg_cycle, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD); 

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

        // for (int i = 0; i < numEdges*2; i++){
        //     std::printf("%d %d %d\n", edges[i].src, edges[i].dst, edges[i].weight);
        // }

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

    int block_lengths[3] = {1, 1, 1};
    MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint offsets[3];

    offsets[0] = offsetof(struct Edge, src);
    offsets[1] = offsetof(struct Edge, dst);
    offsets[2] = offsetof(struct Edge, weight);

    MPI_Datatype mpi_edge;
    MPI_Type_create_struct(3, block_lengths, offsets, types, &mpi_edge);
    MPI_Type_commit(&mpi_edge);

    // std::printf("mpi")

    if(world_rank == 0){

        MPI_Request requests[LEN];

        for(int dst_rank = 1; dst_rank < world_size; dst_rank++){
            MPI_Isend(*edges_ptr, numEdges*2, mpi_edge, dst_rank, tag, MPI_COMM_WORLD, &requests[dst_rank]);
        }

    }else{
        MPI_Request req;

        *edges_ptr = (Edge*)malloc(numEdges * 2 * sizeof(struct Edge));
        
        MPI_Irecv(*edges_ptr, numEdges*2, mpi_edge, src_rank, tag, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, &status);

        Edge* edges = *edges_ptr;

        // if (world_rank == 2){
        //     for (int i = 0; i < numEdges*2; i++){
        //         std::printf("%d %d %d\n", edges[i].src, edges[i].dst, edges[i].weight);
        //     }
        // }

    }

    MPI_Type_free(&mpi_edge);

}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::printf("usage: ./bellmanford_mpi file.txt srcNode dstNode\n");
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

    double elapsedTime = endTime - startTime;
    double avgElapsedTime;

    MPI_Reduce(&elapsedTime, &avgElapsedTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (world_rank == 0){
        std::printf("[BellmanFord mpi]:\t\t[%lf] ms\n", (avgElapsedTime / world_size) * 1000);
        std::printf("The minimum distance from %d to %d is: %d\n", srcNode, dstNode, minDist);
    }

    MPI_Finalize();
    
    
    return 0;
}