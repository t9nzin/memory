#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "allocator.h"

int main() {
    printf("test 1: basic malloc and free\n");
    void *p1 = malloc(100);
    assert(p1 != NULL);
    free(p1);

    printf("test 2: malloc zero returns NULL\n");
    void *p2 = malloc(0);
    assert(p2 == NULL);

    printf("test 3: multiple sequential allocations\n");
    void *p3 = malloc(50);
    void *p4 = malloc(100);
    void *p5 = malloc(200);
    assert(p3 && p4 && p5);
    free(p3);
    free(p4);
    free(p5);

    printf("test 4: calloc initializes to zero\n");
    int *arr = calloc(10, sizeof(int));
    assert(arr != NULL);
    for (int i = 0; i < 10; i++) {
        assert(arr[i] == 0);
    }
    free(arr);

    printf("test 5: write and read data\n");
    char *str = malloc(20);
    strcpy(str, "hello world");
    assert(strcmp(str, "hello world") == 0);
    free(str);

    printf("test 6: realloc NULL acts like malloc\n");
    void *p6 = realloc(NULL, 100);
    assert(p6 != NULL);
    free(p6);

    printf("test 7: realloc size 0 acts like free\n");
    void *p7 = malloc(100);
    void *p8 = realloc(p7, 0);
    assert(p8 == NULL);

    printf("test 8: realloc grows allocation\n");
    char *p9 = malloc(10);
    strcpy(p9, "test");
    char *p10 = realloc(p9, 100);
    assert(p10 != NULL);
    assert(strcmp(p10, "test") == 0);
    free(p10);

    printf("test 9: realloc shrinks allocation\n");
    void *p11 = malloc(200);
    void *p12 = realloc(p11, 50);
    assert(p12 != NULL);
    free(p12);

    printf("test 10: free NULL is safe\n");
    free(NULL);

    printf("\nall basic tests passed\n");
    return 0;
}