int compare_arrays(int *a, int *b, int len) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}