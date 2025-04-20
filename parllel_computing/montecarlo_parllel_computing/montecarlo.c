#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

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
	        unsigned int seed = (unsigned int)(time(NULL) ^ omp_get_thread_num() ^ i);

        	x = ((double)rand_r(&seed))/RAND_MAX;
        	y = ((double)rand_r(&seed))/RAND_MAX;	
		if ((x*x)+(y*y)<=1){
			hit++;
		}
	}
	printf("Pi=%16.16f\n",(4.0*hit)/simulations);
	return 0;
}
