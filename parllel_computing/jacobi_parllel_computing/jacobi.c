#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// read parameter from command line so program can be used as ./jacobi.bin 10000 for a 10000x10000 matrix
void readParameters(int argc, char** argv, unsigned int *N);
// compute the entries of the matrix A and the vector b
void fillSystem(double **A, double *b, unsigned int N);
// printing the system
void printSystem(double **A, double *b, unsigned int N);
// compute the residuum r = b - Ax, is zero, when Ax = b
void computeResiduum(double **A, double *b, double *x, double *r, unsigned int N);
// compute the norm of a vector
double computeNorm(double *x, unsigned int N);
// Jacobi method
void jacobiMethod(double **A, double *b, double *x, unsigned int N, unsigned int max_iter, double tolerance);

// main function
int main(int argc, char** argv) {
    unsigned int N;
    readParameters(argc, argv, &N);
    printf("Problem size is %u\n", N);
    
    // allocate memory for the matrix A and the vectors b and x 
    double **A = (double **)malloc(N * sizeof(double *));
    for (unsigned int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
    }
    double *b = (double *)malloc(N * sizeof(double));
    double *x = (double *)calloc(N, sizeof(double));
    
    // fill the system with values
    fillSystem(A, b, N);
    
    // print the system if small enough to ensure correctness
    if (N < 11) printSystem(A, b, N);
    
    // perform Jacobi method
    jacobiMethod(A, b, x, N, 1000, 1e-6);
    
    // free allocated memory, since A is a pointer of pointers, we need to free each row separately
    for (unsigned int i = 0; i < N; i++) free(A[i]);
    free(A);
    free(b);
    free(x);
    
    return 0;
}

void readParameters(int argc, char** argv, unsigned int *N) {
    if (argc < 2 || atoi(argv[1]) <= 0) {
        printf("Usage: ./<exe> <unsigned int = problem size>\n");
        *N = 10;
    } else {
        *N = atoi(argv[1]);
    }
}

void fillSystem(double **A, double *b, unsigned int N) {
    clock_t start = clock();
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            A[i][j] = -pow(2.0, -pow(2, abs((int)i - (int)j))); // to use pow you must compile with -lm flag (links to math.h)
        }
        A[i][i] = 1.0;
        b[i] = 1.0;
    }
    clock_t end = clock();
    printf("Computation (fill) took: %.5f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
}

void printSystem(double **A, double *b, unsigned int N) {
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < N; j++) {
            printf("%.5f ", A[i][j]);
        }
        printf("| %.5f\n", b[i]);
    }
}

void computeResiduum(double **A, double *b, double *x, double *r, unsigned int N) {
    for (unsigned int i = 0; i < N; i++) {
        r[i] = b[i];
        for (unsigned int j = 0; j < N; j++) {
            r[i] -= A[i][j] * x[j];
        }
    }
}

double computeNorm(double *x, unsigned int N) {
    double norm = 0.0;
    
    #pragma omp parllel for reduction(+:norm)
    for (unsigned int i = 0; i < N; i++) {
        norm += x[i] * x[i];
    }
    return sqrt(norm);
}


void jacobiMethod(double **A, double *b, double *x, unsigned int N, unsigned int max_iter, double tolerance) {
    printf("\nStart of Jacobi method with\n\tproblem size N = %u\n\tmax_iter = %u\n\ttolerance = %.0e\n", N, max_iter, tolerance);
    
    // goal is a norm reduction of the residuum by a factor of tolerance
    // compute the initial residuum
    double *r0 = (double *)malloc(N * sizeof(double));
    computeResiduum(A, b, x, r0, N);
    // compute the norm of the initial residuum
    double norm_r0 = computeNorm(r0, N);
    printf("Norm of residuum at start ||r0|| = %.5f\n", norm_r0);
    
    // make space for the residuum vector r and the temporary vector x_
    double *r = (double *)malloc(N * sizeof(double));
    double *x_ = (double *)malloc(N * sizeof(double));
    unsigned int k;
    double norm_r;
    
    clock_t start = clock();
    #pragma omp parllel for
    for (k = 0; k < max_iter; k++) {
        // one (element-wise) Jacobi iteration
        for (unsigned int i = 0; i < N; i++) {
            x_[i] = b[i];
            for (unsigned int j = 0; j < i; j++) {
                x_[i] -= A[i][j] * x[j];
            }
            for (unsigned int j = i + 1; j < N; j++) {
                x_[i] -= A[i][j] * x[j];
            }
            x_[i] /= A[i][i];
        }
        for (unsigned int i = 0; i < N; i++) {
            x[i] = x_[i];
        }
        computeResiduum(A, b, x, r, N);
        norm_r = computeNorm(r, N);
        if (norm_r < tolerance * norm_r0) break;
    }
    clock_t end = clock();
    
    printf("End of Jacobi method with\n\titerations = %u\n\tnorm of residuum = %.5f\n", k, norm_r);
    printf("Computation (Jacobi) took: %.5f seconds. (Per iteration %.5f)\n", 
           (double)(end - start) / CLOCKS_PER_SEC, (double)(end - start) / ((k + 1) * CLOCKS_PER_SEC));
    
    // free allocated memory (malloc requires free)
    free(r0);
    free(r);
    free(x_);
}

