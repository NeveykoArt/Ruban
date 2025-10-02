#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (a > b) ? a : b

#define BACKPACK_MAX 1000000

int weights[] = {3, 5, 9};
int values[] = {8, 14, 26};
int n = 3;

int main() {
    int *f = (int*)calloc(BACKPACK_MAX + 1, sizeof(int));
    if (!f) {
        printf("Память не выделилась\n");
        return -1;
    }

    int **count = (int**)malloc((BACKPACK_MAX + 1) * sizeof(int*));
    if (!count) {
        printf("Память не выделилась\n");
        free(f);
        return -1;
    }
    for (int i = 0; i <= BACKPACK_MAX; i++) {
        count[i] = (int*)calloc(n, sizeof(int));
        if (!count[i]) {
            printf("Память не выделилась\n");
            for (int j = 0; j < i; j++) free(count[j]);
            free(count);
            free(f);
            return -1;
        }
    }

    for (int w = 0; w <= BACKPACK_MAX; w++) {
        for (int i = 0; i < n; i++) {
            int weight = weights[i];
            int value = values[i];
            if (w - weight >= 0) {
                int candidate = f[w - weight] + value;
                if (candidate > f[w]) {
                    f[w] = candidate;
                    for (int k = 0; k < n; k++) {
                        count[w][k] = count[w - weight][k];
                    }
                    count[w][i]++;
                }
            }
        }
        //printf("f[%d] = %d\n", w, f[w]);
    }

    printf("Максимальная стоимость: %d\n", f[BACKPACK_MAX]);
    printf("Предметы:\n");
    for (int i = 0; i < n; i++) {
        printf("Вес %d: %d\n", weights[i], count[BACKPACK_MAX][i]);
    }

    for (int i = 0; i <= BACKPACK_MAX; i++) {
        free(count[i]);
    }
    free(count);
    free(f);

    return 0;
}
