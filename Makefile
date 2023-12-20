CXX = g++
CFLAGS = -O2 -std=c++17 -g -Wall
NVCC = nvcc
NVFLAGS = -O2 -Xcompiler -lrt -lm
.PHONY: all clean

all: dijkstra bellmanford

bellmanford: bellmanford_serial bellmanford_thread

dijkstra: dijkstra_serial dijkstra_thread dijkstra_cuda

dijkstra_serial: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

dijkstra_thread: dijkstra_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

dijkstra_cuda: dijkstra_cuda.cu
	$(NVCC) $(NVFLAGS) $< -o $@ 

bellmanford_serial: bellmanford_serial.cc
	$(CXX) $(CFLAGS) $< -o $@

bellmanford_thread: bellmanford_thread.cc
	$(CXX) $(CFLAGS) $< -o $@ -fopenmp

clean: clean_dijkstra clean_bellmanford

clean_dijkstra:
	rm dijkstra_serial dijkstra_thread dijkstra_cuda

clean_bellmanford:
	rm bellmanford_serial bellmanford_thread
	
run_serial: 
	./dijkstra_serial ./tools/output.txt 10000 50000

run_thread:
	./dijkstra_thread ./tools/output.txt 10000 50000

run_cuda: 
	./dijkstra_cuda ./tools/output.txt 10000 50000