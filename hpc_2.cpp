#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;

// Define the number of threads to use for parallelization
const int NUM_THREADS = 32;

// Function to perform Bubble Sort in parallel
void bubbleSortParallel(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        int first = i % 2;
        #pragma omp parallel for shared(arr, first) num_threads(NUM_THREADS)
        for (int j = first; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform Bubble Sort sequentially
void bubbleSortSequential(vector<int> &arr) {
    int n = arr.size();
    bool swapped = true;
    int i, j;

    // Loop through all array elements
    while (swapped) {
        swapped = false;
        for (i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i+1]) {
                // Swap arr[i] and arr[i+1]
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                swapped = true;
            }
        }
    }
}

// Function to merge two subarrays of arr[]
void merge(vector<int>& arr, int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temp arrays
    vector<int> L(n1), R(n2);

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    // Merge the temp arrays back into arr[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to perform Sequential Merge Sort in parallel
void mergeSortSequential(vector<int>& arr, 
               int const begin, 
               int const end)
{
    // Returns recursively
    if (begin >= end)
        return; 
  
    auto mid = begin + (end - begin) / 2;
    mergeSortSequential(arr, begin, mid);
    mergeSortSequential(arr, mid + 1, end);
    merge(arr, begin, mid, end);
}

// Function to perform merge sort recursively in parallel
void mergeSortParallel(vector<int>& arr, int l, int r) {
    if (l >= r) {
        // Base case: array has at most one element, so it is already sorted
        return;
    }

    // Compute the middle index
    int m = l + (r - l) / 2;

    // Recursively sort the left and right halves of the array in parallel
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            mergeSortParallel(arr, l, m);
        }

        #pragma omp section
        {
            mergeSortParallel(arr, m + 1, r);
        }
    }

    // Merge the sorted subarrays
    merge(arr, l, m, r);
}

// Function to print an array
void printArray(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    // Define the input array
    vector<int> arr1, arr2, arr3, arr4;
    arr1 = arr2 = arr3 = arr4 = {887, 61, 813, 762, 203, 179, 141, 616, 714, 715, 838, 154, 291, 123, 536, 891, 806, 107, 544, 495, 150, 255, 472, 286, 643, 707, 314, 368, 402, 956, 566, 156, 862, 134, 514, 588, 186, 644, 29, 599, 850, 341, 131, 17, 504, 943, 227, 935, 922, 284, 903, 90, 198, 63, 64, 11, 798, 781, 80, 469, 947, 840, 944, 147, 872, 828, 672, 265, 748, 825, 901, 206, 793, 563, 246, 872, 20, 568, 240, 896, 859, 624, 594, 279, 945, 954, 259, 684, 783, 596, 520, 772, 949, 944, 105, 847, 472, 445, 696, 843, 874, 763, 146, 930, 869, 318, 254, 597, 427, 618, 520, 139, 927, 717, 275, 845, 49, 439, 720, 449, 616, 909, 948, 74, 429, 474, 368, 954, 393, 899, 907, 224, 388, 88, 523, 936, 449, 423, 195, 38, 922, 480, 572, 312, 797, 537, 969, 610, 216, 708, 251, 44, 915, 835, 787, 180, 55, 663, 13, 77, 579, 515, 911, 577, 944, 523, 264, 639, 654, 191, 724, 932, 718, 495, 416, 798, 664, 12, 936, 45, 31, 613, 459, 979, 755, 892, 855, 897, 709, 39, 34, 904, 455, 661, 862, 624, 41, 144, 288, 278, 343, 734, 101, 642, 110, 370, 639, 999, 485, 135, 350, 636, 726, 187, 662, 583, 775, 767, 347, 591, 437, 723, 890, 397, 497, 675, 404, 193, 203, 192, 667, 21, 651, 697, 493, 985, 70, 853, 413, 321, 612, 15, 256, 782, 63, 555, 43, 924, 343, 393, 728, 15, 473, 436, 430, 993, 471, 121, 873, 206, 949, 242, 884, 920, 532, 345, 688, 160, 527, 500, 805, 997, 865, 267, 368, 916, 843, 231, 539, 653, 668, 973, 464, 842, 848, 875, 716, 633, 561, 541, 838, 568, 935, 510, 797, 340, 613, 206, 58, 839, 962, 110, 724, 856, 96, 741, 584, 566, 781, 702, 68, 158, 742, 391, 224, 806, 633, 142, 657, 742, 621, 17, 871, 906, 506, 990, 497, 973, 723, 296, 747, 702, 302, 259, 394, 917, 967, 640, 246, 550, 12, 70, 72, 138, 726, 155, 633, 319, 572, 29, 372, 534, 721, 569, 101, 326, 270, 484, 40, 643, 876, 23, 806, 614, 453, 623, 342, 342, 25, 675, 295, 273, 231, 466, 740, 888, 336, 186, 602, 41, 31, 984, 927, 631, 269, 926, 255, 261, 958, 821, 421, 727, 260, 209, 206, 11, 758, 233, 188, 366, 194, 692, 589, 577, 793, 917, 418, 422, 30, 918, 449, 976, 161, 813, 515, 982, 899, 966, 517, 665, 534, 546, 359, 487, 693, 866, 199, 391, 492, 664, 276, 812, 38, 154, 359, 64, 295, 93, 44, 795, 756, 529, 621, 110, 270, 168, 365, 727, 608, 42, 365, 315, 509, 775, 536, 240, 584, 250, 976, 169, 91, 139, 505, 318, 817, 361, 196, 443, 748, 895, 554, 223, 84, 865, 841, 535, 170, 45, 503, 188, 367, 301, 541, 85, 444, 246, 837, 861, 584, 537, 359, 854, 584, 926, 21, 250, 292, 106, 881, 244};

    // Sort the array using parallel merge sort
    double startTime = omp_get_wtime();
    mergeSortParallel(arr1, 0, arr1.size() - 1);
    double endTime = omp_get_wtime();
    cout << endl << "Sequential Merge Sort: " << endTime - startTime << " seconds" << endl << endl;

    // Sort the array using sequential bubble sort
    startTime = omp_get_wtime();
    mergeSortSequential(arr2, 0, arr2.size() - 1);
    endTime = omp_get_wtime();
    cout << "Parallel Merge Sort: " << endTime - startTime << " seconds" << endl << endl;

    // Sort the array using parallel bubble sort
    startTime = omp_get_wtime();
    bubbleSortParallel(arr3);
    endTime = omp_get_wtime();
    cout << "Sequential Bubble Sort: " << endTime - startTime << " seconds" << endl << endl;

    // Sort the array using sequential bubble sort
    startTime = omp_get_wtime();
    bubbleSortSequential(arr4);
    endTime = omp_get_wtime();
    cout << "Parallel Bubble Sort: " << endTime - startTime << " seconds" << endl << endl;

    return 0;
}