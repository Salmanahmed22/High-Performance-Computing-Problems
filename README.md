# High Performance Computing Problems

This repository contains HPC assignment solutions in C using:
- MPI (distributed-memory parallelism)
- OpenMP (shared-memory parallelism)
- Hybrid MPI + OpenMP

## Repository Structure

- A1/
  - problem1.c: Distributed max value and global index using MPI.
  - problem2.c: Parallel Caesar cipher encode/decode using MPI.
- A2/
  - problem1_v1.c: Parallel prime counting in a numeric range using MPI.
  - problem2.c: Matrix addition with MPI scatter/gather.
- A3/
  - problem1.c: Matrix-vector multiplication with OpenMP.
  - problem2.c: Standard deviation with OpenMP reductions.
  - problem3.c: Hybrid MPI + OpenMP array sum.

## Requirements

Install the following tools:
- C compiler (GCC recommended)
- MPI implementation (OpenMPI or MS-MPI)
- OpenMP support (usually included with GCC)

## Build and Run

Run all commands from the repository root.

### A1

Build:

```bash
mpicc A1/problem1.c -o A1/problem1
mpicc A1/problem2.c -o A1/problem2
```

Run examples:

```bash
mpirun -np 4 A1/problem1
mpirun -np 4 A1/problem2
```

### A2

Build:

```bash
mpicc A2/problem1_v1.c -o A2/problem1_v1
mpicc A2/problem2.c -o A2/problem2
```

Run examples:

```bash
mpirun -np 4 A2/problem1_v1
mpirun -np 4 A2/problem2
```

### A3

Build:

```bash
gcc -fopenmp A3/problem1.c -o A3/problem1
gcc -fopenmp A3/problem2.c -lm -o A3/problem2
mpicc -fopenmp A3/problem3.c -o A3/problem3
```

Run examples:

```bash
A3/problem1
A3/problem2
mpirun -np 4 A3/problem3
```

## Notes

- Most MPI programs in this repo use rank 0 as the master process and expect user input from stdin.
- For best behavior, run MPI programs with at least 2 processes.
- A1/problem2.c currently contains a hardcoded Linux file path for file input mode:
  - /home/Omar/Desktop/Problem2/name.txt
  Update this path for your environment if you use that mode.

## Assignment Assets

Each assignment folder includes the original PDF and screenshots used for submission/reference.

## Date

README added on 2026-04-26.
