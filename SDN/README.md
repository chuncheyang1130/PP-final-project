# SDN Codes
- Env: Mininet v2.3.1b4 (latest) + Ryu v4.34

## Execution
### Generate Mininet topo's Python script and the topo's graph with desired node number:
- Edit generate_mininet_topo.py
```
vim generate_mininet_topo.py
```

- Change the variable NODE_NUM
  - Replace NODE_NUM with 25, 100, or 500
```
### MODIFY THIS
NODE_NUM = 100
### END
```

- Execute the Script
```
python3 generate_mininet_topo.py
```
## Run Mininet and Ryu for evaluation
- Edit controller_bellmanford.py and controller_dijkstra.py
  - Replace ALGO with "dijkstra" of "bellmanford"
```
vim controller_bellmanford.py
vim controller_dijkstra.py
```

- Change the variable NODE_NUM
  - Replace NODE_NUM with 25, 100, or 500
```
## MODIFY THIS
NODE_NUM = 100
## END
```

- Execute Ryu controller
  - Replace ALGO with "dijkstra" of "bellmanford"
  - You can only run 1 controller at a time
```
ryu-manager controller_{ALGO}.py --observe-links
```

- Execute Mininet
  - Replace NODE_NUM with 20, 100, or 500
  - You can only run 1 topo at a time
```
sudo mn --custom mininet_topo_{NODE_NUM}.py --topo mytopo --controller=remote
```
- Ping host of Ping all hosts for testing
  - You can simply enter 'pingall' in Mininet shell
  - To exit Mininet, just enter 'exit' in Mininet shell
  - To exit Ryu, you can either use Ctrl-C or enter 'sudo mn -c' to end the controller
  
## Calculate the average execution time of each algorithm under different topologies
- Execute the Script
  - The script requires all data from each controller and algorithms
  - To reset the data, simply remove the ./time_data/{ALGO}_time_{NODE_NUM}.txt
```
python3 cal_avg_time.py
```
