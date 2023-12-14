CXX = g++
CFLAGS = -O2 -std=c++17

.PHONY: all run clean

all: dijkstra 

dijkstra: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

clean: 
	rm dijkstra
	
run: 
	./dijkstra ./tools/output2.txt 10000 50000
