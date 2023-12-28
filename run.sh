echo -e "=== Scale 1 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph10000_20000 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph10000_20000 $1 $2

#### bellmanford cuda ####
# ./bellmanford_cuda ./graph10000_20000 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph10000_20000.pkl

echo -e "=== Scale 2 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph10000_40000 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph10000_40000 $1 $2

#### bellmanford cuda ####
# ./bellmanford_cuda ./graph10000_40000 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph10000_40000.pkl

echo -e "=== Scale 3 ==="
#### bellmanford seriol ####
./bellmanford_serial ./graph10000_80000 $1 $2

#### bellmanford thread ####
./bellmanford_thread ./graph10000_80000 $1 $2

#### bellmanford cuda ####
# ./bellmanford_cuda ./graph10000_40000 $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2 -f ./graph10000_80000.pkl