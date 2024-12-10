#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define the node structure
struct node {
   char username[30];  // Username of the user
   int socket;         // Socket descriptor for the user
   struct node *next;  // Pointer to the next node
};

// Insert node at the beginning of the list
struct node* insertFirstU(struct node *head, int socket, char *username);

// Find a node with the given username
struct node* findU(struct node *head, char* username);

#endif // LIST_H