CXX = g++
CFLAGS = -O2 -std=c++17 -g -Wall

.PHONY: all run clean

all: dijkstra_serial dijkstra_thread

dijkstra_serial: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

dijkstra_thread: dijkstra_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

clean: 
	rm dijkstra_serial dijkstra_thread
	
run_serial: 
	./dijkstra_serial ./tools/output2.txt 10000 50000

run_thread:
	./dijkstra_thread ./tools/output2.txt 10000 50000