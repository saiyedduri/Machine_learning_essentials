#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){

	int i;
	int N=10;
	int num;

	#pragma omp parallel private(num) 
    {
        num=omp_get_thread_num();

        #pragma omp for 
        for(i=0;i<N;i++){
            printf("Thread %d does iteration %d\n", num, i);
        }
	}

	return 0;
}
