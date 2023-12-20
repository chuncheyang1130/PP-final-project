CXX = g++
CFLAGS = -O2 -std=c++17 -g -Wall

.PHONY: all clean

all: dijkstra bellmanford

bellmanford: bellmanford_serial bellmanford_thread

dijkstra: dijkstra_serial dijkstra_thread

dijkstra_serial: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

dijkstra_thread: dijkstra_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

bellmanford_serial: bellmanford_serial.cc
	$(CXX) $(CFLAGS) $< -o $@

bellmanford_thread: bellmanford_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

clean: clean_dijkstra clean_bellmanford

clean_dijkstra:
	rm dijkstra_serial dijkstra_thread

clean_bellmanford:
	rm bellmanford_serial bellmanford_thread
	
run_serial: 
	./dijkstra_serial ./tools/output2.txt 10000 50000

run_thread:
	./dijkstra_thread ./tools/output2.txt 10000 50000
