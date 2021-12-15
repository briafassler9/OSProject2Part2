#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "BENSCHILLIBOWL.h"

// Feel free to play with these numbers! This is a great way to
// test your implementation.
#define BENSCHILLIBOWL_SIZE 100
#define NUM_CUSTOMERS 90
#define NUM_COOKS 10
#define ORDERS_PER_CUSTOMER 3
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ORDERS_PER_CUSTOMER

// Global variable for the restaurant.
BENSCHILLIBOWL *bcb;

/**
 * Thread funtion that represents a customer. A customer should:
 *  - allocate space (memory) for an order.
 *  - select a menu item.
 *  - populate the order with their menu item and their customer ID.
 *  - add their order to the restaurant.
 */
void* BENSCHILLIBOWLCustomer(void* tid) {
    int customer_id = (int)(long) tid;
  
    for (int i = 0; i < ORDERS_PER_CUSTOMER; i++){
      Order *order = (Order*)malloc(sizeof(Order));
      MenuItem menu_item = PickRandomMenuItem();
      order->menu_item = menu_item;
      order->customer_id = customer_id;
      order->next = NULL;
      int order_num = AddOrder(bcb, order);
    }
	return NULL;
}

/**
 * Thread function that represents a cook in the restaurant. A cook should:
 *  - get an order from the restaurant.
 *  - if the order is valid, it should fulfill the order, and then
 *    free the space taken by the order.
 * The cook should take orders from the restaurants until it does not
 * receive an order.
 */
void* BENSCHILLIBOWLCook(void* tid) {
    int cook_id = (int)(long) tid;
	int orders_fulfilled = 0;
	printf("Cook #%d fulfilled %d orders\n", cook_id, orders_fulfilled);
	return NULL;
}

/**
 * Runs when the program begins executing. This program should:
 *  - open the restaurant
 *  - create customers and cooks
 *  - wait for all customers and cooks to be done
 *  - close the restaurant.
 */
int main() {
  bcb = OpenRestaurant(BENSCHILLIBOWL_SIZE, EXPECTED_NUM_ORDERS);
  pthread_t cooks[NUM_COOKS];
  int cook_id[NUM_COOKS];
  pthread_t customers[NUM_CUSTOMERS];
  int customer_id[NUM_CUSTOMERS];
  
  for (int i = 0; i < NUM_CUSTOMERS; i++){
    customer_id[i] = i + 1;
    pthread_create(&(customers[i]), NULL, BENSCHILLIBOWLCustomer, &(customer_id[i]));
  }
  
  for (int j = 0; j < NUM_COOKS; j++){
  cook_id[j] = j + 1;
  pthread_create(&(cooks[j]), NULL, BENSCHILLIBOWLCook, &(cook_id[j]));
  }
  
  for (int k = 0; k < NUM_CUSTOMERS; k++){
    pthread_join(customers[k], NULL);
  }
  
  for (int l = 0; l < NUM_COOKS; l++){
    pthread_join(cooks[l], NULL);
  }
  CloseRestaurant(bcb);
  return 0;
}