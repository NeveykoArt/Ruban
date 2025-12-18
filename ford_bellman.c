#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>

#define INF DBL_MAX

typedef struct {
    int a;
    int b;
    double cost;
} Edge;

typedef struct {
    double** table;
    int* prev;
    int rows;
    int cols;
} AlgorithmResult;

int* get_path(int* prev, int t, int* path_length) {
    int* path = NULL;
    int length = 0;
    int capacity = 10;

    path = (int*)malloc(capacity * sizeof(int));

    while (t != -1) {
        if (length >= capacity) {
            capacity *= 2;
            path = (int*)realloc(path, capacity * sizeof(int));
        }
        path[length++] = t;
        t = prev[t];
    }

    for (int i = 0; i < length / 2; i++) {
        int temp = path[i];
        path[i] = path[length - 1 - i];
        path[length - 1 - i] = temp;
    }

    *path_length = length;
    return path;
}

void init_graph(int* peaks, int* edges, int* start_peak, Edge** graph) {
    *start_peak = 0;
    *peaks = 5;
    *edges = 7;

    *graph = (Edge*)malloc((*edges) * sizeof(Edge));

    (*graph)[0] = (Edge){0, 1, 25};
    (*graph)[1] = (Edge){0, 2, 15};
    (*graph)[2] = (Edge){0, 3, 7};
    (*graph)[3] = (Edge){0, 4, 2};
    (*graph)[4] = (Edge){1, 2, 6};
    (*graph)[5] = (Edge){2, 3, 4};
    (*graph)[6] = (Edge){3, 4, 3};
}

void free_result(AlgorithmResult* result) {
    if (result->table != NULL) {
        for (int i = 0; i < result->rows; i++) {
            free(result->table[i]);
        }
        free(result->table);
    }
    if (result->prev != NULL) {
        free(result->prev);
    }
}

double** create_table(int rows, int cols) {
    double** table = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        table[i] = (double*)malloc(cols * sizeof(double));
    }
    return table;
}

void copy_array(double* dest, double* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

AlgorithmResult solve_sparse(int n, int m, int v, Edge* edges) {
    AlgorithmResult result;
    result.rows = n;
    result.cols = n;

    double* d = (double*)malloc(n * sizeof(double));
    double* d_next = (double*)malloc(n * sizeof(double));
    result.prev = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        d[i] = INF;
        result.prev[i] = -1;
    }
    d[v] = 0;

    result.table = create_table(n, n);
    copy_array(result.table[0], d, n);

    //алгоритм
    for (int i = 0; i < n - 1; i++) {
        copy_array(d_next, d, n);

        for (int j = 0; j < m; j++) {
            int a = edges[j].a;
            int b = edges[j].b;
            double w = edges[j].cost;

            if (d[a] != INF && d[a] + w < d_next[b]) {
                d_next[b] = d[a] + w;
                result.prev[b] = a;
            }

            if (d[b] != INF && d[b] + w < d_next[a]) {
                d_next[a] = d[b] + w;
                result.prev[a] = b;
            }
        }

        copy_array(d, d_next, n);
        copy_array(result.table[i + 1], d, n);
    }

    free(d);
    free(d_next);

    return result;
}

AlgorithmResult solve_dense(int n, int m, int v, Edge* edges) {
    AlgorithmResult result;
    result.rows = n;
    result.cols = n;

    double* d = (double*)malloc(n * sizeof(double));
    double* d_next = (double*)malloc(n * sizeof(double));
    result.prev = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        d[i] = INF;
        result.prev[i] = -1;
    }
    d[v] = 0;

    double** adj = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        adj[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            adj[i][j] = INF;
        }
    }

    for (int i = 0; i < m; i++) {
        int a = edges[i].a;
        int b = edges[i].b;
        double cost = edges[i].cost;

        if (cost < adj[a][b]) {
            adj[a][b] = cost;
        }
        if (cost < adj[b][a]) {
            adj[b][a] = cost;
        }
    }

    result.table = create_table(n, n);
    copy_array(result.table[0], d, n);

    //алгоритм
    for (int i = 0; i < n - 1; i++) {
        copy_array(d_next, d, n);

        for (int u = 0; u < n; u++) {
            for (int w = 0; w < n; w++) {
                if (d[u] != INF && adj[u][w] != INF && d[u] + adj[u][w] < d_next[w]) {
                    d_next[w] = d[u] + adj[u][w];
                    result.prev[w] = u;
                }
            }
        }

        copy_array(d, d_next, n);
        copy_array(result.table[i + 1], d, n);
    }

    free(d);
    free(d_next);
    for (int i = 0; i < n; i++) {
        free(adj[i]);
    }
    free(adj);

    return result;
}

void print_table(double** table, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        printf("Итерация %d: ", i);
        for (int j = 0; j < cols; j++) {
            if (table[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%.0f ", table[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int peaks, edges_count, start_peak;
    Edge* graph;

    init_graph(&peaks, &edges_count, &start_peak, &graph);

    AlgorithmResult dense_result = solve_dense(peaks, edges_count, start_peak, graph);

    AlgorithmResult sparse_result = solve_sparse(peaks, edges_count, start_peak, graph);

    printf("\nСравнение Dense vs Sparse по итерациям:\n");
    printf("\nDense:\n");
    print_table(dense_result.table, dense_result.rows, dense_result.cols);

    printf("\nSparse:\n");
    print_table(sparse_result.table, sparse_result.rows, sparse_result.cols);

    printf("\nКратчайшие пути:\n");
    for (int t = 0; t < peaks; t++) {
        int path_length;
        int* path = get_path(dense_result.prev, t, &path_length);

        printf("До вершины %d: расстояние = ", t);
        if (dense_result.table[dense_result.rows - 1][t] == INF) {
            printf("INF, путь: недостижима");
        } else {
            printf("%.0f, путь: ", dense_result.table[dense_result.rows - 1][t]);
            for (int i = 0; i < path_length; i++) {
                printf("%d", path[i]);
                if (i < path_length - 1) {
                    printf(" -> ");
                }
            }
        }
        printf("\n");

        free(path);
    }

    free(graph);
    free_result(&dense_result);
    free_result(&sparse_result);

    return 0;
}
