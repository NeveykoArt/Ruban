#include <stdio.h>
#include <stdint.h>

#define BOARD_SIZE 8

#define min(a, b) ((a < b) ? a : b)

int main() {
    int i, j;
    int arrayIn[BOARD_SIZE][BOARD_SIZE];
    int arrayCost[BOARD_SIZE][BOARD_SIZE];

    freopen("king2.in", "r", stdin);
    freopen("king2.out", "w", stdout);

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (scanf("%d", &arrayIn[i][j]) != 1) {
		return -1;
	    }
            arrayCost[i][j] = INT32_MAX;
        }
    }

    arrayCost[BOARD_SIZE - 1][0] = 0;

    for (i = BOARD_SIZE - 1; i >= 0; i--) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (i < 7) {
                arrayCost[i][j] = min(arrayCost[i][j], arrayCost[i + 1][j] + arrayIn[i][j]);
                if (j > 0) {
                    arrayCost[i][j] = min(arrayCost[i][j], arrayCost[i + 1][j - 1] + arrayIn[i][j]);
                }
            }
            if (j > 0) {
                arrayCost[i][j] = min(arrayCost[i][j], arrayCost[i][j - 1] + arrayIn[i][j]);
            }
        }
    }

    printf("%d\n", arrayCost[0][BOARD_SIZE - 1]);

    return 0;
}
