#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "allocator.h"

#define NUM_ALLOCS 10000

double benchmark_malloc_free() {
    clock_t start = clock();
    
    for (int i = 0; i < NUM_ALLOCS; i++) {
        void *ptr = malloc(64);
        free(ptr);
    }
    
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double benchmark_varied_sizes() {
    clock_t start = clock();
    
    for (int i = 0; i < NUM_ALLOCS; i++) {
        size_t size = (i % 10 + 1) * 32;
        void *ptr = malloc(size);
        free(ptr);
    }
    
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double benchmark_realloc() {
    clock_t start = clock();
    
    void *ptr = malloc(16);
    for (int i = 0; i < NUM_ALLOCS; i++) {
        ptr = realloc(ptr, (i % 100 + 1) * 16);
    }
    free(ptr);
    
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double benchmark_fragmentation() {
    clock_t start = clock();
    void *ptrs[100];
    
    for (int round = 0; round < NUM_ALLOCS / 100; round++) {
        for (int i = 0; i < 100; i++) {
            ptrs[i] = malloc((i % 10 + 1) * 16);
        }
        for (int i = 0; i < 100; i += 2) {
            free(ptrs[i]);
        }
        for (int i = 0; i < 100; i += 2) {
            ptrs[i] = malloc((i % 10 + 1) * 16);
        }
        for (int i = 0; i < 100; i++) {
            free(ptrs[i]);
        }
    }
    
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

double benchmark_large_allocs() {
    clock_t start = clock();
    
    for (int i = 0; i < 100; i++) {
        void *ptr = malloc(150 * 1024);
        free(ptr);
    }
    
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    printf("running benchmarks with %d allocations\n\n", NUM_ALLOCS);
    
    printf("benchmark 1: sequential malloc/free (64 bytes)\n");
    double t1 = benchmark_malloc_free();
    printf("time: %.4f seconds\n\n", t1);
    
    printf("benchmark 2: varied size allocations\n");
    double t2 = benchmark_varied_sizes();
    printf("time: %.4f seconds\n\n", t2);
    
    printf("benchmark 3: repeated realloc\n");
    double t3 = benchmark_realloc();
    printf("time: %.4f seconds\n\n", t3);
    
    printf("benchmark 4: fragmentation scenario\n");
    double t4 = benchmark_fragmentation();
    printf("time: %.4f seconds\n\n", t4);
    
    printf("benchmark 5: large allocations (mmap)\n");
    double t5 = benchmark_large_allocs();
    printf("time: %.4f seconds\n\n", t5);
    
    printf("total time: %.4f seconds\n", t1 + t2 + t3 + t4 + t5);
    
    return 0;
}