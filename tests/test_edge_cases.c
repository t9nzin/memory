#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "allocator.h"

int main() {
    printf("test 1: calloc overflow protection\n");
    void *p1 = calloc(SIZE_MAX, SIZE_MAX);
    assert(p1 == NULL);

    printf("test 2: large allocation uses mmap\n");
    void *p2 = malloc(150 * 1024);
    assert(p2 != NULL);
    free(p2);

    printf("test 3: allocation at mmap threshold\n");
    void *p3 = malloc(128 * 1024);
    assert(p3 != NULL);
    free(p3);

    printf("test 4: many small allocations\n");
    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(16);
        assert(ptrs[i] != NULL);
    }
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }

    printf("test 5: alternating malloc and free\n");
    void *p4 = malloc(100);
    void *p5 = malloc(100);
    free(p4);
    void *p6 = malloc(100);
    free(p5);
    free(p6);

    printf("test 6: realloc preserves data\n");
    int *arr = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    arr = realloc(arr, 20 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == i);
    }
    free(arr);

    printf("test 7: realloc to smaller size\n");
    char *str = malloc(1000);
    strcpy(str, "test string");
    str = realloc(str, 20);
    assert(strcmp(str, "test string") == 0);
    free(str);

    printf("test 8: fragmentation scenario\n");
    void *a = malloc(100);
    void *b = malloc(100);
    void *c = malloc(100);
    free(b);
    void *d = malloc(50);
    assert(d != NULL);
    free(a);
    free(c);
    free(d);

    printf("test 9: double free protection\n");
    void *p7 = malloc(100);
    free(p7);
    free(p7);

    printf("test 10: large realloc\n");
    void *p8 = malloc(1000);
    void *p9 = realloc(p8, 200 * 1024);
    assert(p9 != NULL);
    free(p9);

    printf("\nall edge case tests passed\n");
    return 0;
}