echo -e "=== Scale 1 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph4096_65536 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph4096_65536 $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./graph4096_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph4096_65536.pkl -a "bellman-ford"

echo -e "=== Scale 2 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph8192_65536 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph8192_65536 $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./graph8192_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph8192_65536.pkl -a "bellman-ford"

echo -e "=== Scale 3 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph16384_65536 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph16384_65536 $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./graph16384_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph16384_65536.pkl -a "bellman-ford"

echo -e "=== Scale 4 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph32768_65536 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph32768_65536 $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./graph32768_65536 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph32768_65536.pkl -a "bellman-ford"