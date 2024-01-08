# PP-final-project

## How to use random graph generator in tools/graph.py

```
python3 ./tools/general_graph.py -n NodeNum -e EdgeNum > output.txt
```

The output format for ```tools/general_graph.py``` is like `NodeNum EdgeNum` and then several lines of `src dst weight`

```
100 400
0 76 2
1 99 3
2 18 9
2 93 17

...

99 35 5
```

## How to execute BellmanFord algorithm
- `./run_bellmanford.sh srcNode dstNode`
- The executables in the shell script are generated by Makefile

## How to execute Dijkstra algorithm
- `./run_dijkstra.sh srcNode dstNode`
- The executables in the shell script are generated by Makefile


## SDN Codes
- Env: Mininet v2.3.1b4 (latest) + Ryu v4.34
- Source codes and README.md are in ./SDN
