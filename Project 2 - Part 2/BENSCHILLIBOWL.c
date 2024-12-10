#include "BENSCHILLIBOWL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// Global variables
MenuItem BENSCHILLIBOWLMenu[] = {
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    int random_index = rand() % BENSCHILLIBOWLMenuLength;
    return BENSCHILLIBOWLMenu[random_index];
}

/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */
BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    BENSCHILLIBOWL* bcb = malloc(sizeof(BENSCHILLIBOWL));
    if (!bcb) {
        perror("Failed to allocate memory for restaurant");
        exit(EXIT_FAILURE);
    }

    // Initialize restaurant fields
    bcb->max_size = max_size;
    bcb->expected_num_orders = expected_num_orders;
    bcb->orders = NULL;
    bcb->current_size = 0;
    bcb->orders_received = 0;
    bcb->orders_handled = 0;

    // Initialize mutex and condition variables
    pthread_mutex_init(&(bcb->mutex), NULL);
    pthread_cond_init(&(bcb->can_add_orders), NULL);
    pthread_cond_init(&(bcb->can_get_orders), NULL);

    printf("Restaurant is now open!\n");
    return bcb;
}

/* Check that the number of orders received is equal to the number handled, then deallocate resources */
void CloseRestaurant(BENSCHILLIBOWL* bcb) {
    pthread_mutex_lock(&(bcb->mutex));
    if (bcb->orders_received != bcb->orders_handled) {
        printf("Warning: Not all orders were fulfilled (Received: %d, Handled: %d).\n",
            bcb->orders_received, bcb->orders_handled);
    }
    pthread_mutex_unlock(&(bcb->mutex));

    pthread_mutex_destroy(&(bcb->mutex));
    pthread_cond_destroy(&(bcb->can_add_orders));
    pthread_cond_destroy(&(bcb->can_get_orders));
    free(bcb);

    printf("Restaurant is now closed!\n");
}

/* Add an order to the back of the queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    pthread_mutex_lock(&(bcb->mutex));

    // Wait until there's space to add a new order
    while (IsFull(bcb)) {
        pthread_cond_wait(&(bcb->can_add_orders), &(bcb->mutex));
    }

    AddOrderToBack(&(bcb->orders), order);
    bcb->current_size++;
    bcb->orders_received++;

    pthread_cond_signal(&(bcb->can_get_orders));
    pthread_mutex_unlock(&(bcb->mutex));

    return order->customer_id; // Return customer ID as acknowledgment
}

/* Remove and return an order from the front of the queue */
Order* GetOrder(BENSCHILLIBOWL* bcb) {
    pthread_mutex_lock(&(bcb->mutex));

    // Wait until there are orders available
    while (IsEmpty(bcb)) {
        pthread_cond_wait(&(bcb->can_get_orders), &(bcb->mutex));
    }

    Order* order = bcb->orders;
    if (order) {
        bcb->orders = bcb->orders->next;
        bcb->current_size--;
        bcb->orders_handled++;
    }

    pthread_cond_signal(&(bcb->can_add_orders));
    pthread_mutex_unlock(&(bcb->mutex));

    return order;
}

/* Check if the restaurant is full */
bool IsFull(BENSCHILLIBOWL* bcb) {
    return bcb->current_size == bcb->max_size;
}

/* Check if the restaurant is empty */
bool IsEmpty(BENSCHILLIBOWL* bcb) {
    return bcb->current_size == 0;
}

/* Add an order to the back of the queue */
void AddOrderToBack(Order** orders, Order* order) {
    if (!*orders) {
        *orders = order; // Add as the first order
    } else {
        Order* temp = *orders;
        while (temp->next) {
            temp = temp->next; // Traverse to the end of the queue
        }
        temp->next = order; // Append at the end
    }
    order->next = NULL; // Ensure the new order is the last
}
