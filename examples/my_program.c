#include <stdio.h>
#include "allocator.h"

int main() {
    int *arr = malloc(10 * sizeof(int));
    if (arr == NULL) {
        return 1;
    }
    
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }
    
    arr = realloc(arr, 20 * sizeof(int));
    
    free(arr);
    return 0;
}