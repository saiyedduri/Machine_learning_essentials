
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv) {
    int i;
    int N = 10;

    #pragma omp parallel for schedule(dynamic)
    for (i = 0; i < N; i++) {
        int num = omp_get_thread_num();  // Declaring num inside the loop
        printf("Thread %d does iteration %d\n", num, i);
    }

    return 0;
}

