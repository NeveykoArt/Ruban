#include <stdio.h>
#include <stdlib.h>

#define max(a,b) ((a>b) ? a : b)

int main() {
    int *arrayMax;
    int *array;
    int n, i;

    freopen("ladder.in", "r", stdin);
    freopen("ladder.out", "w", stdout);

    if (scanf("%d", &n) != 1) {
        return -1;
    }

    array = (int *)malloc(n * sizeof(int));
    arrayMax = (int *)malloc((n + 1) * sizeof(int));
    if (!array || !arrayMax) {
        return -1;
    }

    for (i = 0; i < n; i++) {
        if (scanf("%d", &array[i]) != 1) {
            free(array);
            free(arrayMax);
            return -1;
        }
    }

    arrayMax[0] = 0;
    arrayMax[1] = array[0];
    for (i = 2; i <= n; i++) {
        arrayMax[i] = arrayMax[i - 1] + array[i - 1];
        arrayMax[i] = max(arrayMax[i], arrayMax[i - 2] + array[i - 1]);
    }

    printf("%d\n", arrayMax[n]);

    free(array);
    free(arrayMax);

    return 0;
}
