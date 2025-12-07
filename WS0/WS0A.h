void print_2d_array(int *arr, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", *(arr + i * width + j));
        }
        printf("\n");
    }
}