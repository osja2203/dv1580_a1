#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize the list
void list_init(Node** head, size_t size) {
    (void)size; // Ignore the size parameter (not used here)
    *head = NULL; //Set the head to NULL (empty list)
}

// Insert a node at the end of the list
void list_insert(Node** head, uint16_t data) {
    // Step 1: Allocate memory for the new node
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error: Memory allocation\n");
        return;
    }

    // Step 2: Set the data and mark next as NULL
    new_node->data = data;
    new_node->next = NULL;

    // Step 3: If list is empty, new node becomes the head
    if (*head == NULL) {
        *head = new_node;
    } else {
        // Step 4: Otherwise go to the end of the list
        Node* current = *head;
        while (current->next) {
            current = current->next;
        }

        // Step 5: Attach new node to the end
        current->next = new_node;
    }
}

// Insert a node after a specific node
void list_insert_after(Node* prev_node, uint16_t data) {
    // Step 1: Check if previous node exists
    if (!prev_node) {
        fprintf(stderr, "Error: prev_node is NULL in list_insert_after.\n");
        return;
    }

    // Step 2: Allocate memory for the new node
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Error: Memory allocation failed in list_insert_after.\n");
        return;
    }

    // Step 3: Set data and insert after previous node
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

// Insert a node before a specific node
void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    // Step 1: Check if the target node is valid
    if (!next_node) {
        fprintf(stderr, "Error: next_node is NULL in list_insert_before.\n");
        return;
    }

    // Step 2: Special case: insert before head
    if (*head == next_node) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        if (!new_node) {
            fprintf(stderr, "Error: Memory allocation failed in list_insert_before.\n");
            return;
        }
        new_node->data = data;
        new_node->next = *head;
        *head = new_node;
    } else {
        // Step 3: Find the node before the target
        Node* current = *head;
        while (current && current->next != next_node) {
            current = current->next;
        }

        // Step 4: If not found, print error
        if (!current) {
            fprintf(stderr, "Error: next_node is not found in list_insert_before.\n");
            return;
        }

        // Step 5: Insert new node after the one before target
        list_insert_after(current, data);
    }
}

// Delete a node by its data value
void list_delete(Node** head, uint16_t data) {
    // Step 1: Check if the list is empty
    if (!*head) {
        fprintf(stderr, "Error: Data %u not found in list_delete.\n", data);
        return;
    }

    // Step 2: Start from the head
    Node* current = *head;
    Node* prev = NULL;

    // Step 3: Traverse to find the node with matching data
    while (current && current->data != data) {
        prev = current;
        current = current->next;
    }

    // Step 4: If not found, print error
    if (!current) {
        fprintf(stderr, "Error: Data %u not found in list_delete.\n", data);
        return;
    }

    // Step 5: If the node is the head, move head to next
    if (!prev) {
        *head = current->next;
    } else {
        // Step 6: Skip over the node to delete it
        prev->next = current->next;
    }

    // Step 7: Free memory
    free(current);
}

// Search for a node by its data value
Node* list_search(Node** head, uint16_t data) {
    // Step 1: Start at the head
    Node* current = *head;

    // Step 2: Traverse the list to find the data
    while (current) {
        // Step 3: If node found, return it
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    // Step 4: Node not found
    return NULL;
}

// Display the whole list
void list_display(Node** head) {
    // Step 1: Print the opening bracket
    printf("[");

    // Step 2: Start at the head
    Node* current = *head;

    // Step 3: Print each node’s data
    while (current) {
        printf("%u", current->data);

        // Step 4: Print a comma if there’s another node after
        if (current->next) {
            printf(", ");
        }
        current = current->next;
    }

    // Step 5: Print the closing bracket
    printf("]");
}

// Display a part of the list (range)
void list_display_range(Node** head, Node* start_node, Node* end_node) {
    // Step 1: If list is empty, print empty brackets
    if (!*head) {
        printf("[]");
        return;
    }

    // Step 2: If no start is given, start from the beginning
    if (!start_node) {
        start_node = *head;
    }

    // Step 3: Print from start to end node
    printf("[");
    Node* current = start_node;
    int first = 1;

    while (current) {
        if (!first) {
            printf(", ");
        }
        printf("%u", current->data);

        if (current == end_node) {
            break;
        }
        current = current->next;
        first = 0;
    }

    // Step 4: Close the output
    printf("]");
}

// Count how many nodes are in the list
int list_count_nodes(Node** head) {
    // Step 1: Start counting at 0
    int count = 0;
    Node* current = *head;

    // Step 2: Go through the list and count each node
    while (current) {
        count++;
        current = current->next;
    }

    // Step 3: Return the count
    return count;
}

// Free all memory and clear the list
void list_cleanup(Node** head) {
    // Step 1: Start at the head
    Node* current = *head;

    // Step 2: Loop and free each node
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    // Step 3: Set head to NULL (empty list)
    *head = NULL;
}
