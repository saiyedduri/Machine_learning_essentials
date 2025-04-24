#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

	#pragma omp parallel
	{
		int numthreads,num;
		numthreads=omp_get_num_threads();// Get the total number of threads in the parallel region
		num=omp_get_thread_num();// Get the ID of the current thread
        printf("Hello from thread %d of %d\n", num, numthreads);
	}

	return 0;
}
