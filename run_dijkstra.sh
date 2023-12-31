echo -e "=== Scale 1 ==="
#### dijkstra seriol ####
./dijkstra_serial ./graph4096_65536 $1 $2

#### dijkstra thread ####
./dijkstra_thread ./graph4096_65536 $1 $2

#### dijkstra cuda ####
./dijkstra_cuda ./graph4096_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph4096_65536.pkl -a "dijkstra"

echo -e "=== Scale 2 ==="
#### dijkstra seriol ####
./dijkstra_serial ./graph8192_65536 $1 $2

#### dijkstra thread ####
./dijkstra_thread ./graph8192_65536 $1 $2

#### dijkstra cuda ####
./dijkstra_cuda ./graph8192_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph8192_65536.pkl -a "dijkstra"

echo -e "=== Scale 3 ==="
#### dijkstra seriol ####
./dijkstra_serial ./graph16384_65536 $1 $2

#### dijkstra thread ####
./dijkstra_thread ./graph16384_65536 $1 $2

#### dijkstra cuda ####
./dijkstra_cuda ./graph16384_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph16384_65536.pkl -a "dijkstra"

echo -e "=== Scale 4 ==="
#### dijkstra seriol ####
./dijkstra_serial ./graph32768_65536 $1 $2

#### dijkstra thread ####
./dijkstra_thread ./graph32768_65536 $1 $2

#### dijkstra cuda ####
./dijkstra_cuda ./graph32768_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph32768_65536.pkl -a "dijkstra"