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
    
    numthreads = omp_get_num_threads();
    num = omp_get_thread_num();
    printf("\nHello (again) from the master thread %d of %d\n", num, numthreads);

    return 0;
}
