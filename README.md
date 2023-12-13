# PP-final-project

### How to use random graph generator in tools/graph.py

```
python3 graph.py --algo [Dijkstra | BellmanFord] -n NodeNum -e EdgeNum -d Distance > output.txt 
```
The output format is like `src dst weight`

```
0 76 2
1 99 3
2 18 9
2 93 17

...

99 35 5
```