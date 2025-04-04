#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>  // Include this to use uint16_t

// Initializes the linked list
void list_init(Node** head, size_t size) {
    *head = NULL;  // Initialize the head to NULL
}

// Inserts a new node at the end of the list
void list_insert(Node** head, uint16_t data) {
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;  // If the list is empty, the new node becomes the head
    } else {
        Node* temp = *head;
        while (temp->next) {
            temp = temp->next;  // Traverse to the last node
        }
        temp->next = new_node;  // Insert the new node at the end
    }
}

// Inserts a new node after a given node
void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        fprintf(stderr, "Error: The previous node cannot be NULL\n");
        return;
    }

    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;
}

// Inserts a new node before a given node
void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    if (*head == NULL || next_node == NULL) {
        fprintf(stderr, "Error: The list or the next node is NULL\n");
        return;
    }

    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    new_node->data = data;

    if (*head == next_node) {
        // If the next_node is the head, insert the new node at the beginning
        new_node->next = *head;
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp && temp->next != next_node) {
            temp = temp->next;
        }

        if (temp == NULL) {
            fprintf(stderr, "Error: Next node not found\n");
            return;
        }

        new_node->next = next_node;
        temp->next = new_node;
    }
}

// Deletes a node with the specified data
void list_delete(Node** head, uint16_t data) {
    if (*head == NULL) {
        fprintf(stderr, "Error: The list is empty\n");
        return;
    }

    Node* temp = *head;
    Node* prev = NULL;

    // If the head node itself holds the data to be deleted
    if (temp != NULL && temp->data == data) {
        *head = temp->next;  // Change head
        mem_free(temp);  // Free the old head
        return;
    }

    // Search for the node to be deleted
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    // If the data was not found
    if (temp == NULL) {
        printf("Node with data %d not found\n", data);
        return;
    }

    // Unlink the node from the linked list
    prev->next = temp->next;
    mem_free(temp);
}

// Searches for a node with the specified data
Node* list_search(Node** head, uint16_t data) {
    Node* temp = *head;
    while (temp) {
        if (temp->data == data) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;  // If not found
}

// Displays all nodes in the list
void list_display(Node** head) {
    Node* temp = *head;
    printf("[");
    while (temp) {
        printf("%d", temp->data);
        if (temp->next) {
            printf(", ");
        }
        temp = temp->next;
    }
    printf("]\n");
}

// Displays nodes between two specified nodes
void list_display_range(Node** head, Node* start_node, Node* end_node) {
    Node* temp = start_node ? start_node : *head;
    printf("[");
    while (temp && (end_node == NULL || temp != end_node->next)) {
        printf("%d", temp->data);
        if (temp->next && (end_node == NULL || temp != end_node)) {
            printf(", ");
        }
        temp = temp->next;
    }
    printf("]\n");
}

// Counts the nodes in the list
int list_count_nodes(Node** head) {
    int count = 0;
    Node* temp = *head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Cleans up and frees all nodes in the list
void list_cleanup(Node** head) {
    Node* temp = *head;
    while (temp) {
        Node* next = temp->next;
        mem_free(temp);
        temp = next;
    }
    *head = NULL;  // Set head to NULL after cleaning up
}
