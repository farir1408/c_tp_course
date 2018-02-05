#include <stdio.h>
#include <stdlib.h>
#define ERROR "[error]"

int* findMax(int* arr, size_t n, size_t m) {
    for (size_t j = 0; j < n -1; j++) {

        for (size_t i = 0; i < n - j - 1; i++) {

            if (arr[i] <= arr[i + 1]) {
                int tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
            }
        }
    }

    int* result = (int*)calloc(m, sizeof(int));

    if (result == NULL) {
        return result;
    }

    for (size_t i = 0; i < m; i++)
        result[i] = arr[i];

    return result;
}

int main() {
    int n = 0;
    int m = 0;
    int elem = 0;
    int count = 0;

    if (1 != scanf("%d", &n) || n <= 0) {
        printf("%s", ERROR);
        return 0;
    }

    int* arr = (int*)calloc(n, sizeof(int));

    if (arr == NULL) {
        printf("%s", ERROR);
        return 0;
    }

    while (count < n) {
        if (1 != scanf("%d", &elem)) {
            printf("%s", ERROR);
            free(arr);
            return 0;
        }

        arr[count++] = elem;
    }

    if (1 != scanf("%d", &m) || m < 0) {
        printf("%s", ERROR);
        free(arr);
        return 0;
    }

    if (n < m) {
        printf("%s", ERROR);
        free(arr);
        return 0;
    }

    int* resultArr = findMax(arr, (size_t)n, (size_t)m);

    if (resultArr != NULL) {
        for (int i = 0; i < m; i++)
            printf("%d ", resultArr[i]);
    }

    free(resultArr);
    free(arr);

    return 0;
}
