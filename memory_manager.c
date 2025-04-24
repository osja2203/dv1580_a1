#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// A struct to keep track of each block of memory
typedef struct MemBlock {
    size_t offset;          // Where in memory the block starts
    size_t size;            // How big the block is
    int is_free;            // 1 if the block is free, 0 if it's used
    struct MemBlock* next;  // Pointer to the next block in the list
} MemBlock;

// Global variables
static char* memory_pool = NULL;     // The main memory area
static size_t pool_size = 0;         // Total size of memory_pool
static MemBlock* block_list = NULL;  // First block in the list

// Initialize the memory system
void mem_init(size_t size) {
    // Step 1: Allocate memory for the memory pool
    memory_pool = malloc(size);
    if (!memory_pool) {
        fprintf(stderr, "Error: Could not allocate memory pool\n");
        exit(EXIT_FAILURE);
    }

    // Step 2: Allocate a metadata block to represent the entire memory
    block_list = malloc(sizeof(MemBlock));
    if (!block_list) {
        free(memory_pool);
        fprintf(stderr, "Error: Could not allocate block list\n");
        exit(EXIT_FAILURE);
    }

    // Step 3: Set up the metadata for the first block
    pool_size = size;
    block_list->offset = 0;
    block_list->size = size;
    block_list->is_free = 1;
    block_list->next = NULL;
}

// Allocate a block of memory
void* mem_alloc(size_t size) {
    // Step 1: If size is 0, return the first free block
    if (size == 0) {
        for (MemBlock* curr = block_list; curr; curr = curr->next) {
            if (curr->is_free) {
                return memory_pool + curr->offset;
            }
        }
        return NULL;
    }

    // Step 2: Find a free block that's big enough
    for (MemBlock* curr = block_list; curr; curr = curr->next) {
        if (curr->is_free && curr->size >= size) {

            // Step 3: If the block is larger than needed, split it
            if (curr->size > size) {
                MemBlock* new_block = malloc(sizeof(MemBlock));
                new_block->offset = curr->offset + size;
                new_block->size = curr->size - size;
                new_block->is_free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->is_free = 0;
                curr->next = new_block;
            } else {
                // Step 4: If the size matches exactly, mark it as used/allocated
                curr->is_free = 0;
            }

            // Step 5: Return a pointer to the memory block
            return memory_pool + curr->offset;
        }
    }

    // Step 6: No suitable block was found
    return NULL;
}

// Free a previously allocated memory block
void mem_free(void* ptr) {
    // Step 1: If the pointer is NULL, do nothing
    if (!ptr) return;

    // Step 2: Find where in the memory pool the pointer points to (Calculate offset)
    size_t offset = (char*)ptr - memory_pool;
    MemBlock* prev = NULL;

    // Step 3: Find the memory block that matches this offset
    for (MemBlock* curr = block_list; curr; curr = curr->next) {
        if (curr->offset == offset) {

            // Step 4: If it's already free, do nothing
            if (curr->is_free) return;

            // Step 5: Mark the block as free
            curr->is_free = 1;

            // Step 6: Merge with next block if it's free
            if (curr->next && curr->next->is_free) {
                MemBlock* next_block = curr->next;
                curr->size += next_block->size;
                curr->next = next_block->next;
                free(next_block);
            }

            // Step 7: Merge with previous block if it's free
            if (prev && prev->is_free) {
                prev->size += curr->size;
                prev->next = curr->next;
                free(curr);
            }

            return;
        }

        // Step 8: Save the current block as the previous for next loop
        prev = curr;
    }
}

// Resize an existing memory block
void* mem_resize(void* ptr, size_t size) {
    // Step 1: If the pointer is NULL, allocate a new block
    if (!ptr) return mem_alloc(size);

    // Step 2: If the size is 0, free the memory and return NULL
    if (size == 0) {
        mem_free(ptr);
        return NULL;
    }

    // Step 3: Calculate the offset in memory
    size_t offset = (char*)ptr - memory_pool;

    // Step 4: Find the block that starts at this offset
    for (MemBlock* curr = block_list; curr; curr = curr->next) {
        if (curr->offset == offset) {

            // Step 5: If current block is big enough, try shrink it
            if (curr->size >= size) {
                if (curr->size > size) {
                    MemBlock* new_block = malloc(sizeof(MemBlock));
                    if (!new_block) return NULL;

                    new_block->offset = curr->offset + size;
                    new_block->size = curr->size - size;
                    new_block->is_free = 1;
                    new_block->next = curr->next;

                    curr->size = size;
                    curr->next = new_block;
                }
                return ptr;
            } else {
                // Step 6: Check if the next block is free and we can join it with this one to make enough space
                if (curr->next && curr->next->is_free &&
                    (curr->size + curr->next->size) >= size) {

                    curr->size += curr->next->size;
                    MemBlock* next_block = curr->next;
                    curr->next = next_block->next;
                    free(next_block);

                    // Step 7: After merging; if we now have more space than we need, split off the extra into a new free block
                    if (curr->size > size) {
                        MemBlock* new_block = malloc(sizeof(MemBlock));
                        if (!new_block) return NULL;

                        new_block->offset = curr->offset + size;
                        new_block->size = curr->size - size;
                        new_block->is_free = 1;
                        new_block->next = curr->next;

                        curr->size = size;
                        curr->next = new_block;
                    }

                    return ptr;
                } else {
                    // Step 8: If we still doesnt fit in place, try to allocate to a new bigger block somewhere else
                    void* new_ptr = mem_alloc(size);
                    if (new_ptr) {
                        // Step 9: Copy data to new block and free the old one
                        memcpy(new_ptr, ptr, curr->size);
                        mem_free(ptr);
                    }
                    return new_ptr;
                }
            }
        }
    }
    // Step 10: If no block was found, return NULL
    return NULL;
}

// Shut down the memory system and free everything
void mem_deinit() {
    // Step 1: Free the memory pool
    if (memory_pool) {
        free(memory_pool);
        memory_pool = NULL;
        pool_size = 0;
    }

    // Step 2: Free all the block metadata
    MemBlock* curr = block_list;
    while (curr) {
        MemBlock* next = curr->next;
        free(curr);
        curr = next;
    }

    // Step 3: Clear the block list pointer
    block_list = NULL;
}
