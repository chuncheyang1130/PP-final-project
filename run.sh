#### bellmanford seriol ####
./bellmanford_serial ./output.txt $1 $2

#### bellmanford thread ####
./bellmanford_thread ./output.txt $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./output.txt $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2