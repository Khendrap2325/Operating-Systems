// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  mylist->head = NULL;
}

void list_free(list_t *l) {
  node_t *current = l->head;
  node_t *next;

  while (current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  free(l);
}

void list_print(list_t *l) {
  node_t *current = l->head;
  while (current != NULL){
    printf("%d -> ", current->value);
    current = current->next; //iterate to the next node
  }
  printf("NULL\n"); //reached the last element in list
}

char * listToString(list_t *l) {
  if (l == NULL || l->head == NULL) {
        return strdup("NULL");
    }

    char *buf = (char *)malloc(sizeof(char) * 1024); // Allocate a buffer
    buf[0] = '\0';  // Ensure buffer is empty

    node_t *current = l->head;
    char temp[20];  // Temporary buffer to hold each node's value

    while (current != NULL) {
        sprintf(temp, "%d->", current->value);
        strcat(buf, temp);  // Append the node value to the result buffer
        current = current->next;
    }

    strcat(buf, "NULL");  // Add "NULL" to the end of the string
    return buf;
}

void list_add_to_back(list_t *l, elem value) {
  node_t *cur_node = getNode(value);

  if (l->head == NULL){
    l->head = cur_node;
    return;
  }

  node_t *temp = l->head;
  while (temp->next != NULL){
    temp = temp->next;
  }
  temp->next = cur_node;
}

void list_add_to_front(list_t *l, elem value) {
     node_t *cur_node = (node_t *) getNode(value);

     /* Insert to front */
     node_t *head = l->head;  // get head of list

     cur_node->next = head;
     l->head = cur_node;
}

node_t * getNode(elem value) {
  node_t *mynode;

  mynode = (node_t *) malloc(sizeof(node_t));
  mynode->value = value;
  mynode->next = NULL;

  return mynode;
}

void list_add_at_index(list_t *l, elem value, int index) {
  node_t *cur_node = (node_t *) getNode(value);

  if (index == 0){
    cur_node->next = l->head;
    l->head = cur_node;
    return;
  }

  node_t *temp = l->head;
  int currentIndex = 0;

  while (temp != NULL && currentIndex < index -1 ){
    temp = temp->next;
    currentIndex++;
  }

  if (temp == NULL){
    printf("Index out of bounds\n");
    free(cur_node);
    return;
  }

  cur_node->next = temp->next;
  temp->next = cur_node;
}

elem list_remove_from_back(list_t *l) { 
  if (l->head == NULL){
    return -1; //list is empty
  }

  node_t *temp = l->head;
  node_t *prev = NULL;

  while (temp->next != NULL){
    prev = temp;
    temp = temp->next;
  }

  if (prev ==NULL){
    l->head = NULL;
  } else{
    prev->next =NULL; //remove the last node
  }

  elem value = temp->value;
  free(temp);
  return value; // return the removed node's value
}
 
elem list_remove_from_front(list_t *l) {
  if (l->head == NULL){
    return -1; //list is empty
  }

  node_t *temp = l->head; 
  l->head = l->head->next; //make head the next node

  elem value = temp->value;
  free(temp);
  return value;
}

elem list_remove_at_index(list_t *l, int index) {
   if (l == NULL || l->head == NULL) {
        printf("List is empty or null\n");
        return -1;
    }

    // Handle the case of removing the first node
    if (index == 0) {
        printf("Removing the first element\n");
        return list_remove_from_front(l);
    }

    node_t *temp = l->head;
    node_t *prev = NULL;
    int currentIndex = 0;

    // Traverse to the node at the specified index
    while (temp != NULL && currentIndex < index) {
        prev = temp;
        temp = temp->next;
        currentIndex++;
    }

    if (temp == NULL) {
        printf("Index out of bounds\n");
        return -1;
    }

    // Bypass the node to be removed
    printf("Removing value: %d at index: %d\n", temp->value, index);
    prev->next = temp->next;

    elem value = temp->value;
    free(temp);  // Free the node
    return value;
}

bool list_is_in(list_t *l, elem value) {
  node_t *current = l->head;

  while (current != NULL){
    if (current->value == value){
      return true;
    }
    current = current->next;
  }
  return false;
}
elem list_get_elem_at(list_t *l, int index) {
  node_t *current =l->head;
  int currentIndex = 0;

  while (current != NULL){
    if (currentIndex == index){
      return current->value;
    }
    current = current->next;
    currentIndex++;
  }
  return -1;
}
int list_get_index_of(list_t *l, elem value) {
  node_t *current = l->head;
  int currentIndex = 0;
  
  while (current != NULL) {
    if (current->value == value){
      return currentIndex;
    }
    current = current->next;
    currentIndex++;
  }
  return -1; 
}

