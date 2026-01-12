#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)

int main() {
    int i, j;
    int **in;
    int **maxPoints;
    int n;

    if (scanf("%d", &n) != 1 || n < 1 || n > 100) return -1;

    in = (int **)malloc(n * sizeof(int *));
    maxPoints = (int **)malloc(n * sizeof(int *));

    if (in == NULL || maxPoints == NULL) {
	free(in);
	free(maxPoints);
	return -1;
    }

    for (i = 0; i < n; i++) {
        in[i] = (int *)malloc((i + 1) * sizeof(int));
        maxPoints[i] = (int *)malloc((i + 1) * sizeof(int));
	if (in[i] == NULL || maxPoints[i] == NULL) {
		for (j = 0; j < i; j++) {
			free(in[j]);
			free(maxPoints[j]);
		}
		free(in);
		free(maxPoints);
		return -1;
	}

    }

    for (i = 0; i < n; i++)
        for (j = 0; j <= i; j++) {
            if (scanf("%d", &in[i][j]) != 1 || in[i][j] < -100 || in[i][j] > 100) { free(in); free(maxPoints); return -1; }
            maxPoints[i][j] = -101;
        }

    maxPoints[0][0] = in[0][0];

    for (i = 1; i < n; i++) {
        for (j = 0; j <= i; j++) {
            if (j)
                maxPoints[i][j] = max(maxPoints[i][j], maxPoints[i - 1][j - 1] + in[i][j]);
            if (j < i)
                maxPoints[i][j] = max(maxPoints[i][j], maxPoints[i - 1][j] + in[i][j]);
        }
    }

    int result = -101;
    for (i = 0; i < n; i++) {
        if (maxPoints[n - 1][i] > result)
            result = maxPoints[n - 1][i];
    }

    printf("%d\n", result);

    for (i = 0; i < n; i++) {
        free(in[i]);
        free(maxPoints[i]);
    }
    free(in);
    free(maxPoints);

    return 0;
}

