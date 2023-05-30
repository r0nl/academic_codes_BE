#include <stdio.h>
#include <time.h>

#define N 10

__global__ void matrixMult(int *a, int *b, int *c) {
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    int sum = 0;
    for (int k = 0; k < N; k++) {
        sum += a[i * N + k] * b[k * N + j];
    }

    c[i * N + j] = sum;
}

int main() {
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    int size = N * N * sizeof(int);

    // Allocate memory for host matrices
    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);

    // Initialize host matrices
    for (int i = 0; i < N * N; i++) {
        a[i] = rand() % 10;
        b[i] = rand() % 10;
    }

    // Allocate memory for device matrices
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    // Copy host matrices to device
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    // Launch kernel with 2x2 threads per block
    clock_t start = clock();
    dim3 threadsPerBlock(2, 2);
    dim3 numBlocks(N / threadsPerBlock.x, N / threadsPerBlock.y);
    matrixMult<<<numBlocks, threadsPerBlock>>>(d_a, d_b, d_c);
    clock_t end = clock();

    // Copy result from device to host
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    // Print result
    printf("\nMatrix A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", a[i * N + j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", b[i * N + j]);
        }
        printf("\n");
    }

    printf("\nResult Matrix:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", c[i * N + j]);
        }
        printf("\n");
    }

    // Calculate elapsed time
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nElapsed time: %f seconds\n", elapsed_time);

    // Free memory
    free(a);
    free(b);
    free(c);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
