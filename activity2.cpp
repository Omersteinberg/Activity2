#include <iostream>
#include <cstdlib>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 1000
using namespace std;

void randomVector(int vector[], int size)
{
    #pragma omp parallel for default(none) private(size) shared(vector) schedule(static, 10000)
    for (int i = 0; i < size; i++)
    {
        // Populate vector with random values in parallel
        vector[i] = rand() % 100;
    }
}

int main(){

    unsigned long size = 100000000;

    srand(time(0));

    int *v1, *v2, *v3;

    omp_set_num_threads(NUM_THREADS);

    // Start clock
    double start = omp_get_wtime();

    // Allocate memort for storing vectors
    v1 = (int *) malloc(size * sizeof(int *));
    v2 = (int *) malloc(size * sizeof(int *));
    v3 = (int *) malloc(size * sizeof(int *));

    randomVector(v1, size);
    randomVector(v2, size);

    #pragma omp barrier 
    
    int total = 0;

    // Paralellise vector addition
    #pragma omp parallel for default(none) private(size) shared(v3, v2, v1) schedule(static, 10000)
    for (int i = 0; i < size; i++)
    {
        v3[i] += v1[i] + v2[i];

        #pragma omp atomic
        total += v3[i];
    }

    double stop = omp_get_wtime();

    cout << endl << "(static scheduling (10000)) Time taken by function: "
         << (stop - start) << " seconds" << endl;

    return 0;
}
