#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i, j;
    int N, M;
    int **ways;

    freopen("knight.in", "r", stdin);
    freopen("knight.out", "w", stdout);

    if (scanf("%d %d\n", &N, &M) != 2) {
	return -1;
    }

    ways = (int **)malloc(N * sizeof(int *));
    for (i = 0; i < N; i++)
        ways[i] = (int *)malloc(M * sizeof(int));

    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            ways[i][j] = 0;

    if (N >= 3 && M >= 2)
        ways[2][1] = 1;
    if (N >= 2 && M >= 3)
        ways[1][2] = 1;

    for (i = 2; i < N; i++)
        for (j = 2; j < M; j++)
            ways[i][j] = ways[i - 2][j - 1] + ways[i - 1][j - 2];

    if (N == 1 && M == 1)
        printf("1\n");
    else
        printf("%d\n", ways[N - 1][M - 1]);

    for (i = 0; i < N; i++)
        free(ways[i]);
    free(ways);

    return 0;
}

