#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

typedef struct {
    long long a;
    long long b;
    long long cost;
    long long flow;
    long long remainflow;
} Vector;

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

long long getEdgeIndex(Vector* v, long long vectorsCnt, long long a, long long b) {
    for (long long i = 0; i < vectorsCnt; i++) {
        if (v[i].a == a && v[i].b == b) {
            return i;
        }
    }
    return -1;
}

bool elExists(long long* array, long long size, long long item) {
    for (long long i = 0; i < size; i++) {
        if (array[i] == item) {
            return true;
        }
    }
    return false;
}

long long getChilds(Vector* v, long long vectorsCnt, long long a, long long* result) {
    long long count = 0;
    for (long long i = 0; i < vectorsCnt; i++) {
        if (v[i].a == a && v[i].remainflow > 0) {
            bool exists = false;
            for (long long j = 0; j < count; j++) {
                if (result[j] == v[i].b) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                result[count++] = v[i].b;
            }
        }
    }
    return count;
}

void reverseArray(long long* arr, long long size) {
    for (long long i = 0; i < size / 2; i++) {
        long long j = size - i - 1;
        long long temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

long long getMin(long long a, long long b) {
    return (a < b) ? a : b;
}

// ==================== АЛГОРИТМ ЭДМОНДСА-КАРПА (BFS) ====================

bool karpBFS(Vector* v, long long vertexCnt, long long vectorsCnt,
             long long start, long long end, long long* parent) {
    bool* visited = (bool*)calloc(vertexCnt, sizeof(bool));
    long long* queue = (long long*)malloc(vertexCnt * sizeof(long long));

    if (!visited || !queue) {
        free(visited);
        free(queue);
        return false;
    }

    long long queueFront = 0;
    long long queueRear = 0;

    for (long long i = 0; i < vertexCnt; i++) {
        parent[i] = -1;
    }

    queue[queueRear++] = start;
    visited[start] = true;

    while (queueFront < queueRear) {
        long long current = queue[queueFront++];

        if (current == end) {
            free(visited);
            free(queue);
            return true;
        }

        for (long long i = 0; i < vectorsCnt; i++) {
            if (v[i].a == current && v[i].remainflow > 0 && !visited[v[i].b]) {
                queue[queueRear++] = v[i].b;
                parent[v[i].b] = current;
                visited[v[i].b] = true;
            }
        }
    }

    free(visited);
    free(queue);
    return false;
}

long long edmondsKarp(Vector* v, long long vertexCnt, long long vectorsCnt,
                      long long start, long long end) {
    long long maxFlow = 0;
    long long iteration = 0;

    long long* parent = (long long*)malloc(vertexCnt * sizeof(long long));

    while (karpBFS(v, vertexCnt, vectorsCnt, start, end, parent)) {
        iteration++;

        long long pathFlow = LLONG_MAX;

        for (long long vertex = end; vertex != start; vertex = parent[vertex]) {
            long long prev = parent[vertex];
            if (prev == -1) break;

            long long edgeIdx = getEdgeIndex(v, vectorsCnt, prev, vertex);
            if (edgeIdx >= 0) {
                pathFlow = getMin(pathFlow, v[edgeIdx].remainflow);
            }
        }

        for (long long vertex = end; vertex != start; vertex = parent[vertex]) {
            long long prev = parent[vertex];
            if (prev == -1) break;

            long long forwardIdx = getEdgeIndex(v, vectorsCnt, prev, vertex);
            if (forwardIdx >= 0) {
                v[forwardIdx].flow += pathFlow;
                v[forwardIdx].remainflow -= pathFlow;
            }

            long long reverseIdx = getEdgeIndex(v, vectorsCnt, vertex, prev);
            if (reverseIdx >= 0) {
                v[reverseIdx].remainflow += pathFlow;
            }
        }

        maxFlow += pathFlow;
        printf("[Karp] Итерация %lld: поток %lld\n", iteration, pathFlow);
    }

    free(parent);
    printf("[Karp] Максимальный поток: %lld (за %lld итераций)\n", maxFlow, iteration);
    return maxFlow;
}

// ==================== АЛГОРИТМ ФОРДА-ФАЛКЕРСОНА (DFS) ====================

long long fordFulkersonDFS(Vector* v, bool* visited, long long current,
                          long long end, long long vectorsCnt, long long minCapacity) {
    if (current == end) {
        return minCapacity;
    }

    visited[current] = true;

    for (long long i = 0; i < vectorsCnt; i++) {
        if (v[i].a == current && !visited[v[i].b] && v[i].remainflow > 0) {
            long long newMinCapacity = getMin(minCapacity, v[i].remainflow);
            long long delta = fordFulkersonDFS(v, visited, v[i].b, end,
                                               vectorsCnt, newMinCapacity);

            if (delta > 0) {
                v[i].flow += delta;
                v[i].remainflow -= delta;

                long long reverseIdx = getEdgeIndex(v, vectorsCnt, v[i].b, v[i].a);
                if (reverseIdx >= 0) {
                    v[reverseIdx].remainflow += delta;
                }

                return delta;
            }
        }
    }

    return 0;
}

long long fordFulkerson(Vector* v, long long vertexCnt, long long vectorsCnt,
                        long long start, long long end) {
    long long maxFlow = 0;
    long long iteration = 0;

    while (1) {
        iteration++;

        bool* visited = (bool*)calloc(vertexCnt, sizeof(bool));
        long long delta = fordFulkersonDFS(v, visited, start, end,
                                          vectorsCnt, LLONG_MAX);
        free(visited);

        if (delta == 0) {
            break;
        }

        maxFlow += delta;
        printf("[Falkerson] Итерация %lld: поток %lld\n", iteration, delta);
    }

    printf("[Falkerson] Максимальный поток: %lld (за %lld итераций)\n", maxFlow, iteration);
    return maxFlow;
}

// ==================== ИНИЦИАЛИЗАЦИЯ И ПРОВЕРКИ ====================

Vector* initializeGraphWithReverseEdges(const char* filename,
                                        long long* vertexCnt,
                                        long long* vectorsCnt) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    long long originalEdges;
    if (fscanf(file, "%lld %lld", vertexCnt, &originalEdges) != 2) {
        fclose(file);
        return NULL;
    }

    *vectorsCnt = originalEdges * 2;
    Vector* vectors = (Vector*)malloc(*vectorsCnt * sizeof(Vector));

    for (long long i = 0; i < originalEdges; i++) {
        long long a, b, w;
        if (fscanf(file, "%lld %lld %lld", &a, &b, &w) != 3) {
            free(vectors);
            fclose(file);
            return NULL;
        }

        long long forwardIdx = i * 2;
        vectors[forwardIdx].a = a;
        vectors[forwardIdx].b = b;
        vectors[forwardIdx].cost = w;
        vectors[forwardIdx].flow = 0;
        vectors[forwardIdx].remainflow = w;

        long long reverseIdx = i * 2 + 1;
        vectors[reverseIdx].a = b;
        vectors[reverseIdx].b = a;
        vectors[reverseIdx].cost = w;
        vectors[reverseIdx].flow = 0;
        vectors[reverseIdx].remainflow = 0;
    }

    fclose(file);
    return vectors;
}

bool checkFlowConservation(Vector* v, long long vectorsCnt,
                          long long vertexCnt, long long start, long long end) {
    long long* inflow = (long long*)calloc(vertexCnt, sizeof(long long));
    long long* outflow = (long long*)calloc(vertexCnt, sizeof(long long));

    for (long long i = 0; i < vectorsCnt; i++) {
        if (v[i].flow > 0) {
            outflow[v[i].a] += v[i].flow;
            inflow[v[i].b] += v[i].flow;
        }
    }

    bool valid = true;
    for (long long i = 0; i < vertexCnt; i++) {
        if (i != start && i != end) {
            if (inflow[i] != outflow[i]) {
                printf("Ошибка сохранения потока в вершине %lld: приток=%lld, отток=%lld\n",
                       i, inflow[i], outflow[i]);
                valid = false;
            }
        }
    }

    printf("Поток из источника: %lld\n", outflow[start] - inflow[start]);
    printf("Поток в сток: %lld\n", inflow[end] - outflow[end]);

    free(inflow);
    free(outflow);
    return valid;
}

Vector* copyGraph(Vector* original, long long vectorsCnt) {
    Vector* copy = (Vector*)malloc(vectorsCnt * sizeof(Vector));
    if (!copy) return NULL;

    for (long long i = 0; i < vectorsCnt; i++) {
        copy[i].a = original[i].a;
        copy[i].b = original[i].b;
        copy[i].cost = original[i].cost;
        copy[i].flow = original[i].flow;
        copy[i].remainflow = original[i].remainflow;
    }

    return copy;
}

int main() {
    const char* filename = "max.in";
    long long vertexCnt, vectorsCnt;

    Vector* vectors = initializeGraphWithReverseEdges(filename, &vertexCnt, &vectorsCnt);
    if (!vectors) {
        fprintf(stderr, "Ошибка инициализации графа\n");
        return 1;
    }

    printf("Граф загружен: %lld вершин, %lld рёбер\n", vertexCnt, vectorsCnt);
    printf("Источник: 0, Сток: %lld\n\n", vertexCnt - 1);

    long long start = 0;
    long long end = vertexCnt - 1;

    printf("=== АЛГОРИТМ ФОРДА-ФАЛКЕРСОНА (DFS) ===\n");
    Vector* graphFF = copyGraph(vectors, vectorsCnt);
    if (!graphFF) {
        fprintf(stderr, "Ошибка копирования графа\n");
        free(vectors);
        return 1;
    }

    fordFulkerson(graphFF, vertexCnt, vectorsCnt, start, end);

    printf("\nПроверка корректности (Форд-Фалкерсон):\n");
    checkFlowConservation(graphFF, vectorsCnt, vertexCnt, start, end);

    printf("\n=== АЛГОРИТМ ЭДМОНДСА-КАРПА (BFS) ===\n");
    Vector* graphEK = copyGraph(vectors, vectorsCnt);
    if (!graphEK) {
        fprintf(stderr, "Ошибка копирования графа\n");
        free(vectors);
        free(graphFF);
        return 1;
    }

    edmondsKarp(graphEK, vertexCnt, vectorsCnt, start, end);

    printf("\nПроверка корректности (Эдмондс-Карп):\n");
    checkFlowConservation(graphEK, vectorsCnt, vertexCnt, start, end);

    printf("\n=== ПОТОКИ ПО РЁБРАМ (из алгоритма Эдмондса-Карпа) ===\n");
    for (long long i = 0; i < vectorsCnt; i += 2) {
        if (graphEK[i].flow > 0) {
            printf("%lld -> %lld: %lld/%lld\n",
                   graphEK[i].a, graphEK[i].b,
                   graphEK[i].flow, graphEK[i].cost);
        }
    }

    free(vectors);
    free(graphFF);
    free(graphEK);

    return 0;
}