import sys
import random
import networkx as nx
import pickle
from argparse import ArgumentParser

def saveGraph(G : nx.Graph):
    with open('general_graph.pkl', 'wb') as f:
        pickle.dump(G, f)

def loadGraph(file : str):
    with open(file, 'rb') as f:
        graph = pickle.load(f)

    return graph

def genConnectedGraph(G : nx.Graph):

    CCs = {}

    nodes = list(G.nodes())

    for node in nodes:
        CCs[node] = [node]

    cc_num = len(nodes)

    while cc_num > 1:

        src_cc = 0
        dst_cc = 0

        while src_cc == dst_cc:
            src_cc = random.randint(0, cc_num-1)
            dst_cc = random.randint(0, cc_num-1)
        
        if src_cc > dst_cc:
            src_cc, dst_cc = dst_cc, src_cc

        src_cc_nodes = CCs[src_cc]
        dst_cc_nodes = CCs[dst_cc]

        src_cc_index = random.randint(0, len(src_cc_nodes)-1)
        dst_cc_index = random.randint(0, len(dst_cc_nodes)-1)

        source = src_cc_nodes[src_cc_index]
        target = dst_cc_nodes[dst_cc_index]
        weight = random.randint(1, 20)

        G.add_edge(source, target, weight=weight)

        CCs[src_cc] += CCs[dst_cc]
        CCs[dst_cc] = CCs[cc_num-1]

        del CCs[cc_num-1]

        cc_num -= 1
    
    if nx.is_connected(G):
        print("All nodes are reachable !!", file=sys.stderr)
    else:
        print("Not all nodes are reachable !!", file=sys.stderr)


if __name__ == '__main__':
    parser = ArgumentParser()
    # parser.add_argument('--algo', type=str, help='type of algorithm')
    parser.add_argument('-n', type=int, help='number of nodes')
    parser.add_argument('-e', type=int, help='number of edges')
    # parser.add_argument('-d', type=int, help='minimum distance between src and dst node')
    args = parser.parse_args()
    
    if args.e < args.n - 1:
        print("Not a fully connected graph !!", file=sys.stderr)
        sys.exit()
    elif args.e > args.n * (args.n-1) / 2:
        print("May have repeated edges !!", file=sys.stderr)
        sys.exit()

    G = nx.Graph()

    for node in range(args.n):
        G.add_node(node)

    genConnectedGraph(G)

    edge_num_remained = args.e - (args.n - 1)

    while edge_num_remained > 0:

        source, target = 0, 0

        while source == target or G.has_edge(source, target):

            source = random.randint(0, args.n-1)
            target = random.randint(0, args.n-1)

        weight = random.randint(1, 20)

        G.add_edge(source, target, weight=weight)
        edge_num_remained -= 1

    graphInfo = list(G.edges(data=True))
    print(args.n, args.e)
    for info in graphInfo:
        print(info[0], info[1], info[2]['weight'])

    saveGraph(G)