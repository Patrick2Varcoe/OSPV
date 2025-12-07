#include <stdio.h>
#include "WS0A.h"



int main() {
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    int width = 4;
    int height = 3;
    print_2d_array((int *)arr, width, height);
    return 0;
}
