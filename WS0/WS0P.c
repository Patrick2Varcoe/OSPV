#include <stdio.h>
#include "WS0P.h"


int main() {
    // Part 1: Pointer to local variable n, increase by one, print
    int n = 5;
    int *ptr_to_n = &n;
    (*ptr_to_n)++;
    printf("n after increment: %d\n", n);

    // Part 2: Array with 3 elements, pointer to array, loop to print elements and addresses
    int arr[3] = {10, 30, 2000};
    int *ptr_arr = arr;
    for (int i = 0; i < 3; i++) {
        printf("Element %d: %d, Address: %p\n", i, *(ptr_arr + i), (ptr_arr + i));
    }

    // Part 3: Test the compare_arrays function
    int arr1[3] = {1, 2, 3};
    int arr2[3] = {1, 2, 3};
    int arr3[3] = {1, 2, 4};
    printf("arr1 == arr2: %d\n", compare_arrays(arr1, arr2, 3)); // Should print 1
    printf("arr1 == arr3: %d\n", compare_arrays(arr1, arr3, 3)); // Should print 0

    return 0;
}
