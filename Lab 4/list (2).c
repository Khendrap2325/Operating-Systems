#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include "list.h"

void list_add_to_front(list_t *list, int value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
}

// Allocate a new linked list
list_t *list_alloc() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) {
        perror("Failed to allocate memory for the list");
        exit(1); // Handle memory allocation failure
    }
    list->head = NULL;
    return list;
}

void list_free(list_t *list) {
    node_t *current = list->head;
    while (current != NULL) {
        node_t *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

void list_add_to_back(list_t *list, int value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        return; // Handle memory allocation failure
    }

    new_node->value = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void list_add_at_index(list_t *list, int index, int value) {
    if (index < 0) return;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        return; // Handle memory allocation failure
    }

    new_node->value = value;

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
        return;
    }

    node_t *current = list->head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        free(new_node);
        return;  // Index is out of bounds, so we don't add the node
    }

    new_node->next = current->next;
    current->next = new_node;
}

int list_remove_from_back(list_t *list) {
    if (list->head == NULL) return -1;  // Empty list

    node_t *current = list->head;
    if (current->next == NULL) {
        int value = current->value;
        free(current);
        list->head = NULL;
        return value;
    }

    while (current->next->next != NULL) {
        current = current->next;
    }
    int value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

int list_remove_from_front(list_t *list) {
    if (list->head == NULL) return -1;  // Empty list

    node_t *temp = list->head;
    int value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

int list_remove_at_index(list_t *list, int index) {
    if (index < 0 || list->head == NULL) return -1;  // Invalid index or empty list

    if (index == 0) {
        return list_remove_from_front(list);
    }

    node_t *current = list->head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL || current->next == NULL) return -1;  // Index out of bounds

    node_t *temp = current->next;
    int value = temp->value;
    current->next = temp->next;
    free(temp);
    return value;
}

int list_get_elem_at(list_t *list, int index) {
    if (index < 0) return -1;  // Invalid index

    node_t *current = list->head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return current ? current->value : -1;
}

int list_length(list_t *list) {
    int length = 0;
    node_t *current = list->head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

char *listToString(list_t *list) {
    static char buffer[1024];  // Static to persist across function calls
    buffer[0] = '\0';

    node_t *current = list->head;
    while (current != NULL) {
        char temp[32];
        snprintf(temp, sizeof(temp), "%d -> ", current->value);
        strcat(buffer, temp);
        current = current->next;
    }
    strcat(buffer, "NULL");
    return buffer;
}
