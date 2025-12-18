#include <stdio.h>
#include <string.h>

#define RUKZAK_MAX 17
#define ITEM_COUNT 4

// 8/3 = 2,67
// 14/5 = 2,8
// 26/9 = 2,89
// 30/10 = 3

int weights[ITEM_COUNT] = {3, 5, 9, 10};
int values[ITEM_COUNT]  = {8, 14, 26, 30};
int max_item_count[ITEM_COUNT] = {10, 40, 10, 7};

void findMax(int rukzak_max, int weights[], int values[], int max_item_count[],
             int* max_value, int result_count[]) {

    int f[RUKZAK_MAX + 1];
    int count[RUKZAK_MAX + 1][ITEM_COUNT];
    memset(f, 0, sizeof(f));
    memset(count, 0, sizeof(count));

    for (int w = 0; w <= rukzak_max; w++) {
        for (int i = 0; i < ITEM_COUNT; i++) {
            if (w - weights[i] >= 0) {
                if (count[w - weights[i]][i] < max_item_count[i]) {
                    int val = f[w - weights[i]] + values[i];
                    if (val > f[w]) {
                        f[w] = val;
                        for (int j = 0; j < ITEM_COUNT; j++) {
                            count[w][j] = count[w - weights[i]][j];
                        }
                        count[w][i] = count[w - weights[i]][i] + 1;
                    }
                }
            }
        }

        if (w > 0 && f[w-1] > f[w]) {
            f[w] = f[w-1];
            for (int j = 0; j < ITEM_COUNT; j++) {
                count[w][j] = count[w-1][j];
            }
        }
    }

    *max_value = f[rukzak_max];
    for (int i = 0; i < ITEM_COUNT; i++) {
        result_count[i] = count[rukzak_max][i];
    }
}

void print_rukzak_result(int weights[], int values[], int max_item_count[], int item_count[], int max_value) {
    int total_weight = 0;
    printf("Состав:\n");
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (item_count[i] > 0) {
            printf("  - Взято %d предметов стоимостью %d, вес каждого: %d\tобщая стоимость всех предметов этого типа - %d\tобщий вес всех предметов этого типа - %d\n", item_count[i], values[i], weights[i], item_count[i] * values[i], item_count[i] * weights[i]);
            total_weight += weights[i] * item_count[i];
        }
    }
    printf("Максимальная стоимость: %d\n", max_value);
    printf("Общий вес: %d\n", total_weight);
}

int main() {
    int max_value;
    int item_count[ITEM_COUNT];

    findMax(RUKZAK_MAX, weights, values, max_item_count, &max_value, item_count);
    print_rukzak_result(weights, values, max_item_count, item_count, max_value);

    return 0;
}