#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "BENSCHILLIBOWL.h"

BENSCHILLIBOWL *bcb;

void* BENSCHILLIBOWLCustomer(void* tid) {
    int customer_id = *(int*)tid;
    printf("Customer %d is placing an order.\n", customer_id);

    // Allocate memory for a new order
    Order* new_order = (Order*)malloc(sizeof(Order));
    if (!new_order) {
        perror("Failed to allocate memory for order.");
        pthread_exit(NULL);
    }

    // Populate the order
    new_order->menu_item = PickRandomMenuItem();
    new_order->customer_id = customer_id;
    new_order->next = NULL;

    // Add the order to the restaurant
    AddOrder(bcb, new_order);

    printf("Customer %d placed an order for %s.\n", customer_id, new_order->menu_item);
    pthread_exit(NULL);
}

void* BENSCHILLIBOWLCook(void* tid) {
    int cook_id = *(int*)tid;
    printf("Cook %d is ready to fulfill orders.\n", cook_id);

    while (1) {
        Order* order = GetOrder(bcb);

        if (!order) { // No more orders
            break;
        }

        // Process the order
        printf("Cook %d is fulfilling order for Customer %d (%s).\n", 
               cook_id, order->customer_id, order->menu_item);

        // Simulate cooking time
        sleep(rand() % 2 + 1);

        // Free order memory
        free(order);
    }

    printf("Cook %d has no more orders to fulfill.\n", cook_id);
    pthread_exit(NULL);
}

int main() {
    int num_customers = 5;
    int num_cooks = 2;
    int max_orders = 10;
    int expected_orders = num_customers;

    // Open the restaurant
    bcb = OpenRestaurant(max_orders, expected_orders);

    // Create customer threads
    pthread_t customers[num_customers];
    int customer_ids[num_customers];
    for (int i = 0; i < num_customers; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, BENSCHILLIBOWLCustomer, &customer_ids[i]);
    }

    // Create cook threads
    pthread_t cooks[num_cooks];
    int cook_ids[num_cooks];
    for (int i = 0; i < num_cooks; i++) {
        cook_ids[i] = i + 1;
        pthread_create(&cooks[i], NULL, BENSCHILLIBOWLCook, &cook_ids[i]);
    }

    // Wait for all customers to finish
    for (int i = 0; i < num_customers; i++) {
        pthread_join(customers[i], NULL);
    }

    // Signal cooks that no more orders are coming
    for (int i = 0; i < num_cooks; i++) {
        AddOrder(bcb, NULL);
    }

    // Wait for all cooks to finish
    for (int i = 0; i < num_cooks; i++) {
        pthread_join(cooks[i], NULL);
    }

    // Close the restaurant
    CloseRestaurant(bcb);

    return 0;
}
