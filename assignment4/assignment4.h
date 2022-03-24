// DO NOT MODIFY THIS FILE.
//
// Forward declarations for the declarations below.
// You should define these functions however you like in assignment4.c.
// But leave these here so that code that includes assignment4.h
// can compile and link against the functions in assignment4.c

#ifndef ASSIGNMENT4_H
#define ASSIGNMENT4_H

#include <stdatomic.h>
#include <stdbool.h>

// - Part 1 -

typedef struct cas_lock {
  atomic_bool locked;
} cas_lock_t;

void cas_lock_init(cas_lock_t* lock);
void cas_lock_acquire(cas_lock_t* lock);
void cas_lock_release(cas_lock_t* lock);

// - Part 2 -

typedef struct ticket_lock {
  atomic_int turn;
  atomic_int ticket;
} ticket_lock_t;

void ticket_lock_init(ticket_lock_t* lock);
void ticket_lock_acquire(ticket_lock_t* lock);
void ticket_lock_release(ticket_lock_t* lock);

// - Part 3 -

typedef struct node {
  int key;
  struct node* next;
} node_t;

typedef struct list {
  node_t* head;
  cas_lock_t lock;
} list_t;

void list_init(list_t* head);
void list_insert(list_t* head, int key);
bool list_find(list_t* head, int key);

// - Part 4 -

typedef enum animal { TANG, STINGRAY, SHARK, INVALID } animal_t;

struct tank;

struct tank* allocate_and_init_tank(void);
void destroy_and_free_tank(struct tank* tank);
void enter_tank(struct tank* tank, animal_t type);
void leave_tank(struct tank* tank, animal_t type);

#endif
