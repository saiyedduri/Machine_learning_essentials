# High Performance Computing (HPC) Cluster Guide

## Table of Contents
1. [Cluster Basics](#cluster-basics)
   - [Login vs Compute Nodes](#login-vs-compute-nodes)
   - [Job Submission Basics](#job-submission-basics)
   - [Job Status Monitoring](#job-status-monitoring)
   
2. [Parallel Computing Models](#parallel-computing-models)
   - [Shared vs Distributed Memory](#shared-vs-distributed-memory)
   - [OpenMP Overview](#openmp-overview)

3. [OpenMP Programming](#openmp-programming)
   - [Basic OpenMP Example](#basic-openmp-example)
   - [Thread Management](#thread-management)
   - [Thread Safety and Race Conditions](#thread-safety-and-race-conditions)
   - [Parallel For Loops](#parallel-for-loops)
   - [Scheduling Options](#scheduling-options)
   - [Reduction Operations](#reduction-operations)

4. [Job Script Examples](#job-script-examples)
   - [Basic Job Script](#basic-job-script)
   - [OpenMP Job Script](#openmp-job-script)
   
5. [Case Study: Monte Carlo Simulation](#case-study-monte-carlo-simulation)
   - [Algorithm Overview](#algorithm-overview)
   - [Implementation with OpenMP](#implementation-with-openmp)
   - [Strong Scaling Test](#strong-scaling-test)

## Cluster Basics

### Login vs Compute Nodes
Login nodes are your gateway to the cluster and are used for job submission, file management, and other interactive tasks. Compute nodes are where your actual computational work happens.

| Feature       | Login Node | Compute Node |
|--------------|-----------|-------------|
| Purpose      | User interaction, job submission, file management | Running computational jobs |
| Accessibility | Direct user access via SSH | Accessed via job scheduler (PBS) |
| Resources    | Limited CPU, memory | High-performance CPUs, GPUs, memory |
| Multi-user   | Shared | Often exclusive to a job |

### Job Submission Basics
Jobs are submitted to the cluster using the `qsub` command:
```bash
qsub job.script
```

### Job Status Monitoring
Monitor your jobs with `qstat`. Delete jobs with `qdel <job-id>`.

Status codes for jobs:
- Q: Queued (waiting)
- R: Running
- C: Completed
- E: Exiting
- H: Held
- S: Suspended
- W: Waiting for scheduled time

## Parallel Computing Models

### Shared vs Distributed Memory
Understanding the difference between shared and distributed memory is fundamental to parallel programming.

| Feature          | Shared Memory | Distributed Memory |
|-----------------|--------------|-------------------|
| Definition   | All processors access a common memory space | Each processor has its own local memory |
| Communication | Data is shared through global memory | Processors communicate via message passing (e.g., MPI) |
| Programming Model | OpenMP, Pthreads | MPI (Message Passing Interface) |
| Scalability  | Limited by memory bandwidth and bus speed | Scales well across multiple nodes |
| Hardware Example | Multi-core CPUs, SMP systems | Cluster of computers, supercomputers |

### OpenMP Overview
OpenMP (Open Multi-Processing) is a set of compiler directives and runtime functions that enable shared-memory multiprocessing in C, C++, and Fortran.

## OpenMP Programming

### Basic OpenMP Example
```c
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){
    #pragma omp parallel
    {
        int numthreads = omp_get_num_threads();
        int num = omp_get_thread_num();
        printf("Hello from thread %d of %d\n", num, numthreads);
    }
    return 0;
}
```

Compilation:
```bash
gcc -o output.bin open_mpi.c -O0 -fopenmp
```

### Thread Management
OpenMP creates threads from the master thread. Key concepts:
- Node: A computer in a cluster with multiple CPU cores
- CPU/Core: A single processing unit that can run one thread at a time
- Thread: A unit of execution within a program

### Thread Safety and Race Conditions
Variables are shared by default in OpenMP, which can lead to race conditions. Use `private` and other clauses to manage this.

### Parallel For Loops
OpenMP can distribute loop iterations across threads:

```c
#pragma omp parallel for
for (i = 0; i < N; i++) {
    // This work will be divided among threads
}
```

### Scheduling Options
OpenMP offers different scheduling strategies for loop parallelization:

| Schedule Type | How it Works |
|---------------|--------------|
| `static` | Iterations are divided evenly before the loop starts |
| `dynamic` | Threads grab chunks of iterations as they finish work |
| `guided` | Like dynamic, but chunk sizes shrink over time |
| `auto` | Compiler/runtime decides the best strategy |
| `runtime` | Uses the `OMP_SCHEDULE` environment variable |

### Reduction Operations
The `reduction` clause helps manage operations like summing across threads:

```c
#pragma omp parallel for reduction(+:sum)
for (i = 0; i < N; i++) {
    sum += data[i];
}
```

## Job Script Examples

### Basic Job Script
```bash
#PBS -N job-name
#PBS -q teachingq
#PBS -l select=1:ncpus=1:mpiprocs=1
#PBS -l walltime=00:01:00
#PBS -o log.out
#PBS -e log.err
echo -e "Job started from $(pwd)"
echo "Changing directory to..."
PBS_O_WORKDIR=~/path/to/work/directory/
cd $PBS_O_WORKDIR
echo -e "$(pwd)"
./your_executable
```

### OpenMP Job Script
```bash
#PBS -N openmp-job
#PBS -q teachingq
#PBS -l select=1:ncpus=4:mpiprocs=1
#PBS -l walltime=00:01:00
#PBS -o log.out
#PBS -e log.err
export OMP_NUM_THREADS=4

echo -e "Job started from $(pwd)."
echo "Changing directory to..."
PBS_O_WORKDIR=/path/to/work/directory
cd $PBS_O_WORKDIR
echo -e "$(pwd)"

./your_openmp_executable
```

## Case Study: Monte Carlo Simulation

### Algorithm Overview
Monte Carlo simulation uses random sampling to estimate mathematical results. The example estimates π by:
1. Generating random points in a unit square
2. Counting points inside a quarter circle
3. Calculating π ≈ 4 × (points inside circle / total points)

### Implementation with OpenMP
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
        unsigned int i, hit=0;
        unsigned int simulations;

        if (argc<2 || atoi(argv[1])<=0){
                printf("Usage: ./<exe> <unsigned int = problem size>\n");  
                simulations=10;
        } else {
                simulations=atoi(argv[1]);
        }
        double x,y;

        #pragma omp parallel for private(x,y) reduction(+:hit)
        for(i=0; i<simulations; i++) {
                x = ((double)rand_r())/RAND_MAX;
                y = ((double)rand_r())/RAND_MAX;
                if ((x*x)+(y*y)<=1){
                        hit++;
                }
        }
        printf("Pi=%16.16f\n",(4.0*hit)/simulations);
        return 0;
}
```

### Strong Scaling Test
A strong scaling test measures how performance improves when increasing the number of processors while keeping the problem size fixed.
