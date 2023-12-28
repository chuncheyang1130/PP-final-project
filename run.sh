#### bellmanford seriol ####
./bellmanford_serial ./output.txt $1 $2

#### bellmanford thread ####
./bellmanford_thread ./output.txt $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./output.txt $1 $2

#### bellmanford mpi #### 
# parallel-scp -h ~/hosts.txt -r ./bellmanford_mpi ~/bellmanford_mpi 2>/dev/null
# parallel-scp -h ~/hosts.txt -r ./output.txt ~/output.txt 2>/dev/null
# mpirun -np 3 --hostfile ~/hosts.txt ~/bellmanford_mpi ~/output.txt $1 $2 

#### solution ####
python3 ./tools/solution.py -s $1 -d $2