# Parallel Optimization Project

This project implements parallel algorithms for finding the minimum of a mathematical function using two different parallelization methods:
- **OpenMP**: Parallel execution using shared memory.
- **MPI**: Parallel execution using distributed memory (message passing).


##  Running the OpenMP Version (step2)
```bash
gcc -fopenmp step2.c -o step2 -lm
./step2
```
##  Running the MPI Version (step_mpi)
```bash
mpicc step_mpi.c -o step_mpi -lm
for i in $(seq 1 12); do
    mpirun -np $i ./step_mpi
done
```

## Prerequisites

Make sure you have the following installed:
- **GCC** compiler with OpenMP support (`gcc`)
- **MPI** library (`mpich` or `openmpi`)
- **Python** with `pandas` and `matplotlib` for result analysis

You can install the required packages on Ubuntu using:

```bash
sudo apt update
sudo apt install gcc g++ libomp-dev mpich python3 python3-pip
pip install pandas matplotlib
```
