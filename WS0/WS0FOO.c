#include <stdio.h>

int main() {
    FILE *file = fopen("foo.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int sum = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        sum += num;
    }

    fclose(file);
    printf("Sum of elements: %d\n", sum);
    return 0;
}
