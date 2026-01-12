#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)

int check(int *arrayMax, const char *path, int i) {
    if (path[i] == '.')
        return arrayMax[i];
    if (path[i] == '\"')
        return arrayMax[i] + 1;
    return -1;
}

int main() {
    freopen("lepus.in", "r", stdin);
    freopen("lepus.out", "w", stdout);

    int n, i;
    if (scanf("%d\n", &n) != 1) {
        return -1;
    }

    char *path = (char *)malloc((n + 1) * sizeof(char));
    if (!path) {
        return -1;
    }

    if (fgets(path, n + 1, stdin) == NULL) {
        free(path);
        return -1;
    }

    for (i = 0; i < n; i++) {
        if (path[i] == '\n') {
            path[i] = '\0';
            break;
        }
    }

    int *arrayMax = (int *)malloc(n * sizeof(int));
    if (!arrayMax) {
        free(path);
        return -1;
    }

    for (i = 0; i < n; i++)
        arrayMax[i] = -1;

    arrayMax[0] = 0;

    for (i = 1; i < n; i++) {
        if (arrayMax[i - 1] != -1)
            arrayMax[i] = check(arrayMax, path, i - 1);
        if (i >= 3 && arrayMax[i - 3] != -1)
            arrayMax[i] = max(arrayMax[i], check(arrayMax, path, i - 3));
        if (i >= 5 && arrayMax[i - 5] != -1)
            arrayMax[i] = max(arrayMax[i], check(arrayMax, path, i - 5));
    }

    if (path[n - 1] == '\"' && arrayMax[n - 1] != -1)
        arrayMax[n - 1]++;

    printf("%d\n", arrayMax[n - 1]);

    free(arrayMax);
    free(path);

    return 0;
}

