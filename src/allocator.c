#include <stdint.h> 
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <assert.h>
#include "allocator.h"
#include <stdio.h>


struct block_meta {
    size_t size;
    struct block_meta* next;  
    bool free; 
    bool is_mmap; 
}; 

const size_t META_SIZE = sizeof(struct block_meta);
const size_t MMAP_THRESHOLD = 128 * 1024; // 128 KB
struct block_meta *free_list = NULL; 

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
    struct block_meta *current = free_list;

    while (current != NULL && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current; 
}

struct block_meta *request_space(struct block_meta *last, size_t size) {
    void *request = sbrk(size + META_SIZE);
    
    if (request == (void*)-1) {
        return NULL;
    }
    
    struct block_meta *block = (struct block_meta *)request;
    block->size = size;
    block->next = NULL;
    block->free = false;
    block->is_mmap = false;
    
    if (last) {
        last->next = block;
    }
    
    return block;
}

void split_block(struct block_meta *block, size_t size) {
   if (block->size >= size + META_SIZE + sizeof(void*)) {
      struct block_meta *new_block = (struct block_meta*)((char*)(block + 1) + size);
      new_block->size = block->size - size - META_SIZE; 
      new_block->next = block->next; 
      new_block->free = true; 
      new_block->is_mmap = false; 
      
      block->size = size; 
      block->next = new_block; 
   }
}

void coalesce_block(struct block_meta *block) {
   if (block->next && block->next->free) {
      block->size += META_SIZE + block->next->size ; 
      block->next = block->next->next; 
   }

   struct block_meta *current = free_list;

   while (current && current->next != block) {
      current = current->next; 
   }

   if (current && current->free) {
      current->size += META_SIZE + block->size; 
      current->next = block->next; 
   }
}

void *malloc(size_t size) {
   printf("using my malloc!");
    struct block_meta *block; 

    if (size == 0 ) {
        return NULL; 
    }

    size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); 

    if (size >= MMAP_THRESHOLD) {
      void *ptr = mmap(NULL, size + META_SIZE, 
                  PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 

      if (ptr == MAP_FAILED) {
         return NULL; 
      }

      block = (struct block_meta *)ptr;

      block->size = size; 
      block->next = NULL; 
      block->free = false;
      block->is_mmap = true; 
    }

    else {
      if (!free_list) {
         block = request_space(NULL, size); 
         if (!block) { 
               return NULL; 
         }
         free_list = block; 

      } else {
         struct block_meta *last = free_list;
         block = find_free_block(&last, size); 
         if (!block) {
               block = request_space(last, size);
               if (!block) {
                  return NULL; 
               }
         } else {
               split_block(block, size);
               block->free = false; 
         }
      }
   }
    return (block + 1); 
}

void *calloc(size_t n, size_t size) {
   if (n != 0 && size > SIZE_MAX / n) {
      return NULL; 
   }

   void *ptr = malloc(n * size); 
   if (ptr == NULL) {
      return NULL; 
   }

   memset(ptr, 0, n * size); 
   return ptr; 
}

void *realloc(void *ptr, size_t size) {
   printf("using my realloc!");
   if (!ptr) {
      return malloc(size); 
   }

   if (size == 0) {
      free(ptr);
      return NULL; 
   }

   struct block_meta *block = (struct block_meta*)ptr - 1;

   size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);

   if (block->size >= size) {
      split_block(block, size);
      return ptr;
   }

   if (!block->is_mmap && block->next && block->next->free && 
       block->size + META_SIZE + block->next->size >= size) {
      block->size += META_SIZE + block->next->size;
      block->next = block->next->next;
      split_block(block, size);
      return ptr;
   }
   
   void *new_ptr = malloc(size);
   if (!new_ptr) {
      return NULL;
   }
   
   memcpy(new_ptr, ptr, block->size);
   free(ptr);
   return new_ptr;
}

void free(void* ptr) {
   printf("hey there!");
   if (!ptr) {                            
      return;
   }

   struct block_meta *block = (struct block_meta*)ptr - 1;

   if (block->is_mmap) {
      munmap(block, block->size + META_SIZE);
   }

   else {
      block->free = true;
      coalesce_block(block); 
   }
}