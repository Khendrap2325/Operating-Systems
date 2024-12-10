#include "list.h"

// Insert a new node at the beginning of the list
struct node* insertFirstU(struct node *head, int socket, char *username) {
    // Allocate memory for a new node
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    if (!newNode) {
        perror("Failed to allocate memory for new node");
        return head;
    }

    // Initialize the new node
    strncpy(newNode->username, username, sizeof(newNode->username) - 1);
    newNode->username[sizeof(newNode->username) - 1] = '\0'; // Ensure null-termination
    newNode->socket = socket;
    newNode->next = head; // Point to the current head

    return newNode; // Return the new head
}

// Find a node by username
struct node* findU(struct node *head, char* username) {
    struct node *current = head;

    // Traverse the list to find the node
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return current; // Found the node
        }
        current = current->next; // Move to the next node
    }

    return NULL; // Node not found
}