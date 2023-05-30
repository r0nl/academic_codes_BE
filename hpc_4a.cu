#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Number of elements in the vectors
#define N 1000000

// Kernel function to add vectors
__global__ void addVectors(float *a, float *b, float *c) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < N) {
        c[idx] = a[idx] + b[idx];
    }
}

int main() {
    float *h_a, *h_b, *h_c;
    float *d_a, *d_b, *d_c;
    int size = N * sizeof(float);

    // Allocate memory for host vectors
    h_a = (float *)malloc(size);
    h_b = (float *)malloc(size);
    h_c = (float *)malloc(size);

    // Initialize host vectors
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < N; i++) {
        h_a[i] = rand() / (float)RAND_MAX;
        h_b[i] = rand() / (float)RAND_MAX;
    }

    // Allocate memory for device vectors
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    // Copy host vectors to device
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // Launch kernel with 1 block of 256 threads
    clock_t start = clock();
    addVectors<<<(N + 255) / 256, 256>>>(d_a, d_b, d_c);
    cudaDeviceSynchronize();
    clock_t end = clock();

    // Copy result from device to host
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    // Verify result
    int error = 0;
    for (int i = 0; i < N; i++) {
        if (h_c[i] != h_a[i] + h_b[i]) {
            printf("Error: h_c[%d] = %f, h_a[%d] = %f, h_b[%d] = %f\n",
                   i, h_c[i], i, h_a[i], i, h_b[i]);
            error++;
            break;
        }
    }
    printf("Error: %d\n", error);

    // Calculate elapsed time
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    // Free memory
    free(h_a);
    free(h_b);
    free(h_c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
