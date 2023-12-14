CXX = g++
CFLAGS = -O2 -std=c++17 -g -Wall

.PHONY: all clean

all: dijkstra_serial dijkstra_thread bellmanford_serial

dijkstra_serial: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

dijkstra_thread: dijkstra_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

bellmanford_serial: bellmanford_serial.cc
	$(CXX) $(CFLAGS) $< -o $@

clean: 
	rm dijkstra_serial dijkstra_thread bellmanford_serial
	
run_serial: 
	./dijkstra_serial ./tools/output2.txt 10000 50000

run_thread:
	./dijkstra_thread ./tools/output2.txt 10000 50000
