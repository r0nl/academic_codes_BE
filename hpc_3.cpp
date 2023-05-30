#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

#define N 10

void printArray(int *arr) {
    int i;
    cout << "[";
    for (i = 0; i < N; i++) {
        cout << arr[i];
        if (i < N - 1) cout << ", ";
    }
    cout << "]" << endl;
}

int main() {
    int i, sum = 0, min = 100, max = -1;
    double avg;

    int *arr = new int[N];

    // Initialize the array with random values between 0 and 99
    srand(time(NULL));
    for (i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }

    // Compute the sum, min, max and average using parallel reduction
    #pragma omp parallel for reduction(+:sum) reduction(min:min) reduction(max:max)
    for (i = 0; i < N; i++) {
        sum += arr[i];
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }
    avg = (double) sum / N;

    cout << "Array: " <<endl;
    printArray(arr);

    cout << "Sum: " << sum << endl;
    
    cout << "Min: " << min << endl;
    
    cout << "Max: " << max << endl;
    
    cout << "Average: " << avg << endl;

    delete[] arr;

    return 0;
}
