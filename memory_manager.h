#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>  // For size_t

// Block structure to manage memory allocations
typedef struct Block {
    size_t size;    // Size of the block (excluding the Block structure)
    int free;       // Indicates if the block is free (1) or allocated (0)
    struct Block* next; // Pointer to the next block in the pool
} Block;

// Function declarations
void mem_init(size_t size);          // Initializes the memory pool
void* mem_alloc(size_t size);        // Allocates memory
void mem_free(void* block);          // Frees previously allocated memory
void* mem_resize(void* block, size_t size); // Resizes a block
void mem_deinit();                   // Deinitializes the memory manager and frees memory

#endif // MEMORY_MANAGER_H
