#### bellmanford seriol ####
./bellmanford_serial ./output.txt $1 $2

#### bellmanford thread ####
./bellmanford_thread ./output.txt $1 $2

#### bellmanford cuda ####
./bellmanford_cuda ./output.txt $1 $2

#### bellmanford mpi #### 
parallel-scp -r -A -h ~/hosts.txt ./bellmanford_mpi ~/bellmanford_mpi
parallel-scp -r -A -h ~/hosts.txt ./output.txt ~/output.txt
mpirun -np 8 --hostfile ~/hosts.txt bellmanford_mpi ./output.txt $1 $2

#### solution ####
python3 ./tools/solution.py -s $1 -d $2