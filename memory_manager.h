#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

// Initialize memory manager with given pool size
void mem_init(size_t size);

// Allocate memory block of given size
void* mem_alloc(size_t size);

// Free previously allocated memory block
void mem_free(void* block);

// Resize previously allocated memory block
void* mem_resize(void* block, size_t size);

// Deinitialize memory manager and free all resources
void mem_deinit();

#endif // MEMORY_MANAGER_H
