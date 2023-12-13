CXX = g++
CFLAGS = -O2 -std=c++17

.PHONY: all run clean

all: dijkstra 

dijkstra: dijkstra_serial.cc
	$(CXX) $(CFLAGS) $< -o $@ 

clean: 
	rm dijkstra
	
run: 
	./dijkstra ./tools/output.txt 100 500