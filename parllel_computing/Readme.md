# HPC Cluster Computing: Parallel Programming Guide
This repository aims to learn parllel computing/HPC on cluster

## Table of Contents
1. [Cluster Basics](#cluster-basics)
   - [Login vs Compute Nodes](#login-vs-compute-nodes)
   - [OpenMP Overview](#openmp-overview)
   - [Job Submission](#job-submission)
   - [Job Status Monitoring](#job-status-monitoring)
   
2. [Parallel Computing Models](#parallel-computing-models)
   - [Shared vs Distributed Memory](#shared-vs-distributed-memory)
   - [Understanding Nodes, CPUs, and Threads](#understanding-nodes-cpus-and-threads)

3. [OpenMP Programming](#openmp-programming)
   - [Basic OpenMP Example](#basic-openmp-example)
   - [Thread Management Example](#thread-management-example)
   - [Thread Safety and Race Conditions](#thread-safety-and-race-conditions)
   - [Parallel For Loops](#parallel-for-loops)
   - [Scheduling Options](#scheduling-options)

4. [Job Script Examples](#job-script-examples)
   - [Basic Job Script](#basic-job-script)
   - [OpenMP Job Script](#openmp-job-script)
   
5. [Case Study: Monte Carlo Simulation](#case-study-monte-carlo-simulation)
   - [Algorithm Overview](#algorithm-overview)
   - [Implementation with OpenMP](#implementation-with-openmp)
   - [Strong Scaling Test](#strong-scaling-test)

## Cluster Basics

### Login vs Compute Nodes

| Feature       | Login Node | Compute Node |
|--------------|-----------|-------------|
| Purpose      | User interaction, job submission, file management | Running computational jobs.Compute nodes are used HPC calculations on cluster through a job script |
| Accessibility | Direct user access via SSH | Accessed via job scheduler like Portable Batch management System(PBS) |
| Resources    | Limited CPU, memory | High-performance CPUs, GPUs, memory |
| Multi-user   | Shared | Often exclusive to a job |

### OpenMP Overview
OpenMP (Open Multi-Processing) is a set of compiler directives and runtime functions that allow you to write parallel code easily in C, C++, and Fortran. It enables shared-memory multiprocessing.

### Job Submission
Submit a job script to the cluster using `qsub`:
```bash
qsub job.script
```

Example output:
```
(base) [sy37tovi@mlogin01 parllel_computing]$ qsub change_pwd_cluster
927540.mmaster02
```

### Job Status Monitoring
Monitor jobs with `qstat`. Delete jobs with `qdel <job-id>`:

Status of job script:
| Job ID          | Name           | User      | Time Use | S | Queue      |
|-----------------|----------------|-----------|----------|---|------------|
| 929936.mmaster02| first-example  | sy37tovi  |        0 | R | teachingq  |

Status codes:
| S    | Meaning   | Description                                                               |
|------|-----------|---------------------------------------------------------------------------|
| Q    | Queued    | Job is waiting in the queue to be scheduled.                             |
| R    | Running   | Job is currently running on compute nodes.                               |
| C    | Completed | Job has finished running (you might see this only briefly or in logs).   |
| E    | Exiting   | Job is in the process of ending (wrapping up, cleaning up output, etc).  |
| H    | Held      | Job is being held and won't be scheduled until released.                 |
| S    | Suspended | Job has been suspended (paused) — usually by an admin or scheduler.      |
| W    | Waiting   | Job is waiting for its start time (e.g., a scheduled future run).        |

## Parallel Computing Models

### Shared vs Distributed Memory

| Feature          | Shared Memory | Distributed Memory |
|-----------------|--------------|-------------------|
| **Definition**   | All processors access a common memory space. | Each processor has its own local memory. |
| **Communication** | Data is shared through global memory. | Processors communicate via message passing (e.g., MPI). |
| **Programming Model** | OpenMP, Pthreads | MPI (Message Passing Interface) |
| **Scalability**  | Limited by memory bandwidth and bus speed. | Scales well across multiple nodes. |
| **Performance**  | Fast communication but limited scalability. | Higher latency due to network communication. |
| **Hardware Example** | Multi-core CPUs, SMP (Symmetric Multiprocessing) systems. | Cluster of computers, supercomputers. |
| **Best For** | Programs running on a single machine with multiple cores. | Large-scale parallel computing across multiple nodes. |

### Understanding Nodes, CPUs, and Threads
- **Node**: A node is a machine or a computer in a cluster that usually has multiple CPU cores.
- **CPU/Core**: This is a single processing unit inside a node. Typically, each core can run one thread at a time.
- **Thread**: This is a unit of execution within a program. In OpenMP, threads are typically created within a process, and each thread can be mapped to a separate core.

## OpenMP Programming

### Basic OpenMP Example
File: `open_mpi.c`
```c
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

    #pragma omp parallel
    {
        int numthreads,num;
        numthreads=omp_get_num_threads();
        num=omp_get_thread_num();
        printf("Hello from thread %d of %d\n", num, numthreads);
    }
    return 0;
}
```

Compilation command:
```bash
gcc -o output.bin open_mpi.c -O0 -fopenmp
```

Job script (`job_script_4threads`):
```bash
#PBS -N first-example
#PBS -q teachingq
#PBS -l select=1:ncpus=4:mpiprocs=1
#PBS -l walltime=00:01:00
#PBS -o log.out1
#PBS -e log.err1
export OMP_NUM_THREADS=4

echo -e "Job started from $(pwd)."
echo "Changing directory to..."
PBS_O_WORKDIR=/home/sy37tovi/parllel_computing/open_mp_example
cd $PBS_O_WORKDIR
echo -e "$(pwd)"

./output.bin
```

Example output:
```
Job started from /home/sy37tovi/pbs.929936.mmaster02.x8z.
Changing directory to...
/home/sy37tovi/parllel_computing/open_mp_example
Hello from thread 0 of 4
Hello from thread 3 of 4
Hello from thread 1 of 4
Hello from thread 2 of 4
```

### Thread Management Example
File: `open_mpi2.c`
```c
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

    int numthreads = omp_get_num_threads();
    int num = omp_get_thread_num();

    printf("Hello from the master thread %d of %d\n\n", num, numthreads);

    #pragma omp parallel
    {
        numthreads = omp_get_num_threads();
        num = omp_get_thread_num();
        printf("     Hello from the forked thread %d of %d\n", num, numthreads);
    }

    printf("\nHello (again) from the master thread %d of %d\n", num, numthreads);

    return 0;
}
```

Compilation:
```bash
gcc -o output2.bin open_mpi2.c -O0 -fopenmp
```

Job script (`job_script_4threads2`):
```bash
#PBS -N first-example
#PBS -q teachingq
#PBS -l select=1:ncpus=4:mpiprocs=1
#PBS -o log.out2
#PBS -e log.err2
export OMP_NUM_THREADS=4

echo -e "Job started from $(pwd)."
echo "Changing directory to..."
PBS_O_WORKDIR=/home/sy37tovi/parllel_computing/open_mp_example
cd $PBS_O_WORKDIR
echo -e "$(pwd)"

./output2.bin
```

Output (showing race condition):
```
Job started from /home/sy37tovi/pbs.929938.mmaster02.x8z.
Changing directory to...
/home/sy37tovi/parllel_computing/open_mp_example
Hello from the master thread 0 of 1

     Hello from the forked thread 0 of 4
     Hello from the forked thread 2 of 4
     Hello from the forked thread 1 of 4
     Hello from the forked thread 3 of 4

Hello (again) from the master thread 3 of 4
```

### Thread Safety and Race Conditions
Fixed example:
```c
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

    int numthreads = omp_get_num_threads();
    int num = omp_get_thread_num();

    printf("Hello from the master thread %d of %d\n\n", num, numthreads);

    #pragma omp parallel
    {
        numthreads = omp_get_num_threads();
        num = omp_get_thread_num();
        printf("     Hello from the forked thread %d of %d\n", num, numthreads);
    }

    // FIX: Recomputing these values after parallel section to get the master thread
    numthreads = omp_get_num_threads();
    num = omp_get_thread_num();
    printf("\nHello (again) from the master thread %d of %d\n", num, numthreads);

    return 0;
```

Output after fix:
```
Job started from /home/sy37tovi/pbs.929983.mmaster02.x8z.
Changing directory to...
/home/sy37tovi/parllel_computing/open_mp_example
Hello from the master thread 0 of 1

     Hello from the forked thread 0 of 4
     Hello from the forked thread 1 of 4
     Hello from the forked thread 3 of 4
     Hello from the forked thread 2 of 4

Hello (again) from the master thread 0 of 1
```

### Parallel For Loops
```c
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv) {
    int i;
    int N = 10;

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        int num = omp_get_thread_num();
        printf("Thread %d does iteration %d\n", num, i);
    }
    return 0;
}
```

Output:
```
Job started from /home/sy37tovi/pbs.932784.mmaster02.x8z.
Changing directory to...
/home/sy37tovi/parllel_computing/open_mp_for_loop
Thread 0 does iteration 0
Thread 0 does iteration 1
Thread 0 does iteration 2
Thread 2 does iteration 6
Thread 2 does iteration 7
Thread 3 does iteration 8
Thread 3 does iteration 9
Thread 1 does iteration 3
Thread 1 does iteration 4
Thread 1 does iteration 5
```

### Scheduling Options
Present in: `/parllel_computing/open_mp_for_loop/scheduling` folder:

| **Schedule Type** | **How it Works** |
|-------------------|------------------------------------------------------------------------------------|
| `static`          | Iterations are divided evenly before the loop starts. Fast but rigid.            |
| `dynamic`         | Threads grab chunks of iterations as they finish work. Better for load balancing.|
| `guided`          | Like dynamic, but chunk sizes shrink over time. Good for decreasing workloads.   |
| `auto`            | Lets the compiler/runtime decide the best scheduling strategy.                   |
| `runtime`         | Uses the `OMP_SCHEDULE` environment variable to decide at runtime.               |

Different schedule examples:
- `schedule(static,1)`: Round-robin distribution, one iteration at a time
- `schedule(dynamic)`: Threads ask for work as they finish (default chunk size = 1)
- `schedule(dynamic,3)`: Threads grab 3 iterations at a time

## Case Study: Monte Carlo Simulation

### Algorithm Overview
Monte Carlo simulation is a statistical technique used to understand the impact of risk and uncertainty in mathematical models and decision-making.

The program estimates the value of π using:
1. Imagine a square with side length 1
2. Inside the square, fit a quarter circle of radius 1 (from (0,0) to (1,1))
3. Randomly throw points inside the square
4. The ratio of points inside the circle to total points approximates the area of the quarter circle

Since:
- Area of quarter circle = (π * r²) / 4 = π / 4
- Area of square = 1

Then:
- π ≈ 4 × (number of points inside circle / total number of points)

### Implementation with OpenMP
File: `montecarlo.c`
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
        unsigned int i,hit=0;
        unsigned int simulations;

        if (argc<2 || atoi(argv[1])<=0){
                 printf("Usage: ./<exe> <unsigned int = problem size>\n");  
                simulations=10;
        }else {
                simulations=atoi(argv[1]);
        }
        double x,y;

        #pragma omp parllel for private(x,y) reduction(+:hit)
        for(i=0; i<simulations;i++)
        {
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

Job script for the Monte Carlo simulation:
```bash
#PBS -N first-example
#PBS -q teachingq
#PBS -l select=1:ncpus=4:mpiprocs=1
#PBS -l walltime=00:01:00
#PBS -o log.out1
#PBS -e log.err1
export OMP_NUM_THREADS=4

echo -e "Job started from $(pwd)."
echo "Changing directory to..."
PBS_O_WORKDIR=/home/sy37tovi/parllel_computing/montecarlo_parllel_computing
cd $PBS_O_WORKDIR
echo -e "$(pwd)"

./montecarlo.bin 400000000
```

The `reduction(+:hit)` clause is important because:
- Each thread gets its own local copy of hit, initialized to 0
- After the loop, OpenMP adds up all the local hits into the shared/global hit variable
- This prevents race conditions when multiple threads try to update the same variable

### Strong Scaling Test
A strong scaling test measures how the performance of a parallel program improves when you increase the number of processors (threads/cores) while keeping the problem size fixed.
