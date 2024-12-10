#ifndef LIST_H 
#define LIST_H 

// Define a node structure for the linked list
typedef struct node {
  int value;            // The value stored in the node
  struct node *next;    // Pointer to the next node in the list
} node_t;

// Define the list structure, which holds the head pointer of the list
typedef struct {
  node_t *head;         // Pointer to the first node in the list
} list_t;

/**
 * Allocates memory for a new linked list and initializes it.
 * @return A pointer to the newly allocated linked list.
 */
list_t *list_alloc();

/**
 * Frees the memory used by the linked list, including all its nodes.
 * @param list The linked list to free.
 */
void list_free(list_t *list);

/**
 * Adds a new node with the specified value at the back of the list.
 * @param list The linked list.
 * @param value The value to add to the list.
 */
void list_add_to_back(list_t *list, int value);

/**
 * Adds a new node with the specified value at the front of the list.
 * @param list The linked list.
 * @param value The value to add to the list.
 */
void list_add_to_front(list_t *list, int value);

/**
 * Adds a new node with the specified value at the given index in the list.
 * @param list The linked list.
 * @param index The position at which to insert the new node.
 * @param value The value to add to the list.
 */
void list_add_at_index(list_t *list, int index, int value);

/**
 * Removes and returns the value from the back of the list.
 * @param list The linked list.
 * @return The value removed from the back of the list, or -1 if the list is empty.
 */
int list_remove_from_back(list_t *list);

/**
 * Removes and returns the value from the front of the list.
 * @param list The linked list.
 * @return The value removed from the front of the list, or -1 if the list is empty.
 */
int list_remove_from_front(list_t *list);

/**
 * Removes and returns the value from the node at the specified index.
 * @param list The linked list.
 * @param index The index of the node to remove.
 * @return The value removed, or -1 if the index is invalid or the list is empty.
 */
int list_remove_at_index(list_t *list, int index);

/**
 * Returns the value of the node at the specified index.
 * @param list The linked list.
 * @param index The index of the node.
 * @return The value of the node at the given index, or -1 if the index is out of bounds.
 */
int list_get_elem_at(list_t *list, int index);

/**
 * Returns the length (number of nodes) in the list.
 * @param list The linked list.
 * @return The number of nodes in the list.
 */
int list_length(list_t *list);

/**
 * Converts the linked list to a human-readable string representation.
 * @param list The linked list.
 * @return A string representing the list (e.g., "1 -> 2 -> 3 -> NULL").
 */
char *listToString(list_t *list);

/**
 * Checks if the list is empty.
 * @param list The linked list.
 * @return 1 if the list is empty, 0 otherwise.
 */
int list_is_empty(list_t *list);

void list_add_to_front(list_t *list, int value);

#endif // LIST_H
