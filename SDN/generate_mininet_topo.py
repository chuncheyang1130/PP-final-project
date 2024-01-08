import networkx as nx
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.link import TCLink
from mininet.node import RemoteController
import random
import matplotlib.pyplot as plt

## MODIFY THIS
NODE_NUM=100
## END

topo_file_name = f'./topo_txts/topo_{NODE_NUM}.txt'
topo_node_num = int(NODE_NUM)
host_node_map = {
        20 : 0, 
        100 : 1, 
        500 : 2
}
host_nodes = [ 
        [0, 1, 7, 9, 10, 13, 15, 19],
        [1, 4, 11, 22, 38, 47, 61, 99],
        [9, 41, 49, 148, 244, 154, 137, 47, 377, 413, 438, 184, 306, 58, 77, 493, 349, 344]
]
host_node = host_nodes[host_node_map[topo_node_num]]
#host_node = [0, 1, 7, 9, 10, 13, 15, 19] #for 20 nodes
#host_node = [1, 4, 11, 22, 38, 47, 61, 99] #for 100 nodes
#host_node = [9, 41, 49, 148, 244, 154, 137, 47, 377, 413, 438, 184, 306, 58, 77, 493, 349, 344] #for 500 nodes

def visualize_mininet_topo(filename, save_path='./topo_graphs/topo.png'):
    G = nx.Graph()

    with open(filename, 'r') as topo_file:
        # Read the first line containing the number of nodes and edges
        first_line = topo_file.readline()
        num_nodes, num_edges = map(int, first_line.split())

        # Read the remaining lines containing edges
        for _ in range(num_edges):
            line = topo_file.readline()
            src, dst, weight = map(int, line.split())
            G.add_edge(src, dst, weight=weight)

    pos = nx.spring_layout(G)

    # Separate hosts and switches for coloring
    hosts = [node for node in G.nodes() if node in host_node]
    switches = [node for node in G.nodes() if node in host_node]

    # Draw switches in blue and hosts in red
    nx.draw_networkx_nodes(G, pos, nodelist=switches, node_color='b', node_size=400)
    nx.draw_networkx_nodes(G, pos, nodelist=hosts, node_color='r', node_size=400)

    nx.draw_networkx_edges(G, pos, edgelist=G.edges(), width=1.0, alpha=0.5)

    labels = {node: node for node in G.nodes()}
    nx.draw_networkx_labels(G, pos, labels, font_size=8, font_color='black')

    # Save the graph as an image file
    plt.savefig(save_path)
    plt.show()
weights = []

def write_mininet_topo(filename, controller_ip):
    links = []
    nodes = []
    edges = []
    imin = False
    #with open('./topo_100.txt', 'r') as topo_file:
    #with open('./topo_500.txt', 'r') as topo_file:
    with open(topo_file_name, 'r') as topo_file:
        for line in topo_file:
            if imin:
                if line.split(' ')[0] not in nodes:
                    nodes.append(line.split(' ')[0])
                if line.split(' ')[1] not in nodes:
                    nodes.append(line.split(' ')[1])
                edges.append((line.split(' ')[0], line.split(' ')[1], line.split(' ')[2]))
            imin = True

    with open(filename, 'w') as file:
        file.write("import random\n")
        file.write("import threading\n")
        file.write("import os\n")
        file.write("import socket\n")
        file.write("\n")
        file.write("from mininet.net import Mininet\n")
        file.write("from mininet.node import Controller, RemoteController, OVSKernelSwitch, UserSwitch, OVSSwitch, Host\n")
        file.write("from mininet.cli import CLI\n")
        file.write("from mininet.log import setLogLevel\n")
        file.write("from mininet.link import TCLink, Link\n")
        file.write("from mininet.topo import Topo\n")
        file.write("\n")
        file.write("class Network(Topo):\n")
        file.write("\n")
        file.write("    def __init__(self):\n")
        file.write("        Topo.__init__(self)\n")
        # Mark nodes at positions 5, 10, 15, etc., as hosts
        for node in nodes:
            node = int(node)
            if node in host_node:
                file.write(f"        h{node} = self.addHost('h{node}')\n")
            else:
                file.write(f"        s{node} = self.addSwitch('s{node}')\n")
        for edge in edges:
            src, dst, weight = edge[0], edge[1], edge[2]
            src = int(src)
            dst = int(dst)
            weight = weight.split('\n')[0]
            if src in host_node and dst in host_node:
                file.write(f"        self.addLink(h{src}, h{dst}, cls=TCLink, delay='{weight}ms')\n")
            elif src in host_node:
                file.write(f"        self.addLink(h{src}, s{dst}, cls=TCLink, delay='{weight}ms')\n")
            elif dst in host_node:
                file.write(f"        self.addLink(s{src}, h{dst}, cls=TCLink, delay='{weight}ms')\n")
            else:
                file.write(f"        self.addLink(s{src}, s{dst}, cls=TCLink, delay='{weight}ms')\n")
        
        file.write("\ntopos = {\n")
        file.write("        'mytopo': (lambda: Network())\n")
        file.write("}\n")

def visualize_topology(graph, save_path='topology.png'):
    pos = nx.spring_layout(graph)

    # Separate hosts and switches for coloring
    hosts = [node for node in graph.nodes() if node % 5 == 0]
    switches = [node for node in graph.nodes() if node % 5 != 0]

    # Draw switches in blue and hosts in red
    nx.draw_networkx_nodes(graph, pos, nodelist=switches, node_color='b', node_size=400)
    nx.draw_networkx_nodes(graph, pos, nodelist=hosts, node_color='r', node_size=400)

    nx.draw_networkx_edges(graph, pos, edgelist=graph.edges(), width=1.0, alpha=0.5)

    labels = {node: f'{node}' for node in graph.nodes()}
    nx.draw_networkx_labels(graph, pos, labels, font_size=8, font_color='black')

    # Save the graph as an image file
    plt.savefig(save_path)
    plt.close()

if __name__ == "__main__":
    # Controller IP Address (replace with your controller's IP)
    controller_ip = '127.0.0.1'


    # Write the Mininet topology script to a file
    write_mininet_topo(f'./mininet_topo_{topo_node_num}.py', controller_ip)
    visualize_mininet_topo(topo_file_name, save_path=f'./topo_graphs/topo_{topo_node_num}.png')
    print(f'Mininet topo file "./mininet_topo_{topo_node_num}.py" is generated.')
    print(f'Topo graph "topo_{topo_node_num}.png" can be found in directory ./topo_graphs/')
