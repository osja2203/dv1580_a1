#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pointer to the start of the memory pool
static Block* memory_pool = NULL;
static void* pool_start = NULL;    // Start address of the allocated pool

// Function to initialize the memory pool
void mem_init(size_t size) {
    if (size == 0) {
        fprintf(stderr, "Error: Memory size must be greater than 0.\n");
        return;
    }

    pool_start = malloc(size);  // Allocate memory pool
    if (!pool_start) {
        fprintf(stderr, "Error: Could not allocate memory pool.\n");
        return;
    }

    // Initialize the first block in the pool
    memory_pool = (Block*)pool_start;
    memory_pool->size = size - sizeof(Block);  // Subtract Block size to leave space for metadata
    memory_pool->free = 1;   // Mark the block as free
    memory_pool->next = NULL;

    printf("Memory manager initialized with %zu bytes.\n", size);
}

// Function to allocate a block of memory from the pool
void* mem_alloc(size_t size) {
    if (size == 0 || !memory_pool) {
        fprintf(stderr, "Error: Invalid allocation request or uninitialized memory.\n");
        return NULL;
    }

    Block* current = memory_pool;
    while (current) {
        if (current->free && current->size >= size) {
            // If the block is large enough, split it if possible
            size_t remaining_size = current->size - size;

            // Only split if the remaining size is large enough to create a new block
            if (remaining_size >= sizeof(Block)) {  // Ensure remaining space is large enough for new Block
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = remaining_size - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
            }

            // Mark the block as allocated
            current->size = size;
            current->free = 0;
            return (void*)((char*)current + sizeof(Block));
        }
        current = current->next;
    }

    fprintf(stderr, "Error: No suitable memory block found.\n");
    return NULL;
}

// Function to free a previously allocated block
void mem_free(void* block) {
    if (!block || (char*)block < (char*)pool_start || (char*)block >= (char*)pool_start + memory_pool->size) {
        fprintf(stderr, "Error: Invalid pointer for freeing.\n");
        return;
    }

    Block* current = (Block*)((char*)block - sizeof(Block));
    current->free = 1;

    // Merge adjacent free blocks to reduce fragmentation
    Block* temp = memory_pool;
    while (temp) {
        if (temp->free && temp->next && temp->next->free) {
            // Merge with the next free block
            temp->size += sizeof(Block) + temp->next->size;
            temp->next = temp->next->next;
        } else {
            temp = temp->next;
        }
    }
}

// Function to resize an allocated block
void* mem_resize(void* block, size_t size) {
    if (!block) {
        return mem_alloc(size); // If the block is NULL, allocate new memory
    }

    Block* current = (Block*)((char*)block - sizeof(Block));
    if (current->size >= size) {
        return block; // If the block is already large enough, return the same block
    }

    // Allocate new memory and copy data from the old block
    void* new_block = mem_alloc(size);
    if (new_block) {
        memcpy(new_block, block, current->size); // Copy data to the new block
        mem_free(block); // Free the old block
    }
    return new_block;
}

// Function to deinitialize the memory manager and free the memory pool
void mem_deinit() {
    if (!pool_start) {
        fprintf(stderr, "Error: Memory is not initialized.\n");
        return;
    }

    free(pool_start);  // Free the memory pool
    pool_start = NULL;
    memory_pool = NULL;

    printf("Memory manager deinitialized.\n");
}
