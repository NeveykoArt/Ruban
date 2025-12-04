#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int stones[] = {0, 1, 2, 3, 5, 7, 10, 11, 14, 18, 19, 20, 22, 25};
int stones_len = 14;

int jump_lengths_len;
int *jump_lengths;

double **jump_counts;

double min3(double a, double b, double c) {
    double m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

double jump(int position, int length) {
    if (position - length < 0)
        return DBL_MAX;

    double comp[3] = {DBL_MAX, DBL_MAX, DBL_MAX};

    comp[0] = jump_counts[length - 1][position - length];
    comp[1] = jump_counts[length][position - length];
    if (length < jump_lengths_len - 1) {
        comp[2] = jump_counts[length + 1][position - length];
    }
    double m = min3(comp[0], comp[1], comp[2]);
    return m + 1.0;
}

void backwards(double **array, int max_stone) {
    int *out;
    int out_len;
    int pos = max_stone;

    int jump_len = 0;
    double min_val = DBL_MAX;

    for (int i = 0; i < jump_lengths_len; i++) {
        if (array[i][pos] < min_val) {
            min_val = array[i][pos];
            jump_len = i;
        }
    }

    out = malloc(sizeof(int) * (max_stone + 1));
    if (min_val == DBL_MAX) {
        out_len = 1;
        out[0] = -1;
        return;
    }

    int idx = 0;

    while (pos != 0) {
        out[idx++] = pos;
        pos -= jump_len;

        double comp[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
        if (jump_len > 0)
            comp[0] = array[jump_len - 1][pos];
        comp[1] = array[jump_len][pos];
        if (jump_len < jump_lengths_len - 1)
            comp[2] = array[jump_len + 1][pos];

        int min_i = 1;
        if (comp[0] < comp[min_i]) min_i = 0;
        if (comp[2] < comp[min_i]) min_i = 2;
        jump_len += min_i - 1;
    }
    out[idx++] = 0;
    out_len = idx;

    printf("Path:");
    for (int i = 0; i < out_len; i++) {
        printf(" %d", out[i]);
    }
    printf("\n");
}

void print_table(int stones_len, int* stones, int jump_lengths_len, double **jump_counts) {
    printf("  ");
    for (int si = 0; si < stones_len; si++) {
        printf(" %2d", stones[si]);
    }
    printf("\n");

    for (int xl = 0; xl < jump_lengths_len; xl++) {
        printf("%d", jump_lengths[xl]);
        for (int si = 0; si < stones_len; si++) {
            int y = stones[si];
            if (jump_counts[xl][y] == DBL_MAX)
                printf("  -");
            else
                printf(" %2.0f", jump_counts[xl][y]);
        }
        printf("\n");
    }
}

int main() {
    int max_stone = stones[stones_len - 1];
    jump_lengths_len = max_stone;
    jump_lengths = malloc(sizeof(int) * jump_lengths_len);
    for (int i = 0; i < jump_lengths_len; i++)
        jump_lengths[i] = i;

    jump_counts = malloc(sizeof(double*) * jump_lengths_len);
    for (int i = 0; i < jump_lengths_len; i++) {
        jump_counts[i] = malloc(sizeof(double) * (max_stone + 1));
        for (int j = 0; j <= max_stone; j++) {
            jump_counts[i][j] = DBL_MAX;
        }
    }
    jump_counts[0][0] = 0.0;

    for (int si = 1; si < stones_len; si++) {
        int x = stones[si];
        for (int yl = 1; yl < jump_lengths_len; yl++) {
            jump_counts[yl][x] = jump(x, yl);
        }
    }

    print_table(stones_len, stones, jump_lengths_len, jump_counts);

    backwards(jump_counts, max_stone);

    for (int i = 0; i < jump_lengths_len; i++) {
        free(jump_counts[i]);
    }
    free(jump_counts);
    free(jump_lengths);

    return 0;
}
