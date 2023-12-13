import sys
import random
import networkx as nx
from argparse import ArgumentParser

def checkDAG(G : nx.DiGraph):
    return nx.is_directed_acyclic_graph(G)

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--algo', type=str, help='type of algorithm')
    parser.add_argument('-n', type=int, help='number of nodes')
    parser.add_argument('-e', type=int, help='number of edges')
    parser.add_argument('-d', type=int, help='minimum distance between src and dst node')
    args = parser.parse_args()
    
    G = nx.DiGraph()
    if args.algo == 'Dijkstra':
        for node in range(args.n):
            G.add_node(node)
        
        edge = 0
        while edge < args.e:
            source = random.randint(0, args.n-1)
            target = random.randint(0, args.n-1)
            weight = random.randint(1, 20)

            if source != target and not G.has_edge(source, target):
                G.add_edge(source, target, weight=weight)
                edge += 1

    elif args.algo == 'BellmanFord':
        for node in range(args.n):
            G.add_node(node)
        
        edge = 0
        while(edge < args.e):
            source = random.randint(0, args.n-1)
            target = random.randint(0, args.n-1)
            weight = random.randint(-10, 10)

            if not G.has_edge(source, target):
                G.add_edge(source, target, weight=weight)
                edge += 1 

        if checkDAG(G):
            print("Is Directed Acyclic Graph")
        else:
            print("Is not Directed Acyclic Graph")

    else:
        print('Unsupported Algorithm', file=sys.stderr)
        sys.exit()

    srcNode = random.randint(0, args.n-1)
    dstNode = random.randint(0, args.n-1)
    while srcNode == dstNode or not nx.has_path(G, srcNode, dstNode) or nx.shortest_path_length(G, srcNode, dstNode) < args.d:
        dstNode = random.randint(0, args.n-1)
        print(srcNode, dstNode, file=sys.stderr)    
        
    print(srcNode, dstNode)

    graphInfo = list(G.edges(data=True))
    for info in graphInfo:
        print(info[0], info[1], info[2]['weight'])

    if args.algo == 'Dijkstra':
        print(nx.shortest_path_length(G, srcNode, dstNode, weight='weight', method='dijkstra'), file=sys.stderr)
    elif args.algo == 'BellmanFord':
        print(nx.shortest_path_length(G, srcNode, dstNode, weight='weight', method='bellman-ford'), file=sys.stderr)