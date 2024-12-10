#ifndef BENSCHILLIBOWL_H
#define BENSCHILLIBOWL_H

#include <stdbool.h>
#include <pthread.h>

// Menu Item type definition
typedef const char* MenuItem;

// Struct to represent an order
typedef struct Order {
    MenuItem menu_item;
    int customer_id;
    struct Order* next;
} Order;

// Struct to represent the restaurant
typedef struct BENSCHILLIBOWL {
    int max_size;                // Maximum orders the restaurant can handle at once
    int current_size;            // Current number of orders in the queue
    int orders_received;         // Total number of orders received
    int orders_handled;          // Total number of orders handled
    int expected_num_orders;     // Number of orders the restaurant expects to handle

    Order* orders;               // Pointer to the head of the order queue

    pthread_mutex_t mutex;       // Mutex to protect shared resources
    pthread_cond_t can_add_orders; // Condition variable for adding orders
    pthread_cond_t can_get_orders; // Condition variable for getting orders
} BENSCHILLIBOWL;

// Constants
#define MENU_ITEMS 10
extern MenuItem BENSCHILLIBOWLMenu[MENU_ITEMS];

// Function declarations
MenuItem PickRandomMenuItem();
BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders);
void CloseRestaurant(BENSCHILLIBOWL* bcb);
int AddOrder(BENSCHILLIBOWL* bcb, Order* order);
Order* GetOrder(BENSCHILLIBOWL* bcb);

// Optional helper function declarations
bool IsFull(BENSCHILLIBOWL* bcb);
bool IsEmpty(BENSCHILLIBOWL* bcb);
void AddOrderToBack(Order** orders, Order* order);

#endif // BENSCHILLIBOWL_H
