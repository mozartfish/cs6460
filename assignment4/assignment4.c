#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <inttypes.h>
#include <malloc.h>
#include <assert.h>
#include <pthread.h>

#include "assignment4.h"

// --- Problem 1: Compare-and-swap Spinlock  ---

/**
 * Initialize a newly allocated cas_lock_t to the unlocked state. After
 * allocating a cas_lock_t it must be initialized before calling
 * cas_lock_acquire() and cas_lock_release() with that lock is legal.
 *
 * This function performs no allocations itself, and it never fails.
 * Hence, no special function needs to be called before freeing the lock later.
 *
 * This function may not be thread-safe, and only one thread should call it.
 *
 * Arguments:
 *  - lock: a pointer to a newly allocated cas_lock_t whose fields are to be
 *          initialized; passing any other pointer is illegal.
 */
void cas_lock_init(cas_lock_t *lock)
{
  // Boolean atomic case - set to false
  atomic_store(&(lock->locked), false);
}

/**
 * Spins until ownership of the lock is acquired; this ensures that only thread
 * has ownership of the lock at a time. It is illegal to call this function if
 *  - the lock has never been initialized (see cas_lock_init()),
 *  - or this thread already owns the passed lock.
 *
 * This function never fails.
 *
 * Arguments:
 *  - lock: a pointer to an initalized cas_lock_t not currently owned by the
 *          calling thread; passing any other pointer is illegal.
 */
void cas_lock_acquire(cas_lock_t *lock)
{
  bool expected = false;
  while (!atomic_compare_exchange_strong(&(lock->locked), &expected, true))
  {
    expected = false;
  };
}

/**
 * Releases ownership of the lock. It is illegal to call this function
 * if this thread does not own the passed lock.
 *
 * This function never fails.
 *
 * Arguments:
 * - lock: a pointer to a cas_lock_t currently owned by the calling
 *         thread; passing any other pointer is illegal.
 */
void cas_lock_release(cas_lock_t *lock)
{
  atomic_store(&(lock->locked), false);
}

// --- Problem 2: Ticket Spinlock  ---

/**
 * Initialize a newly allocated ticket_lock_t to the unlocked state. After
 * allocating a ticket_lock_t it must be initialized before calling
 * ticket_lock_acquire() and ticket_lock_release() with that lock is legal.
 *
 * This function performs no allocations itself, and it never fails.
 * Hence, no special function needs to be called before freeing the lock later.
 *
 * This function may not be thread-safe, and only one thread should call it.
 *
 * Arguments:
 *  - lock: a pointer to a newly allocated ticket_lock_t whose fields are to be
 *          initialized; passing any other pointer is illegal.
 */
void ticket_lock_init(ticket_lock_t *lock)
{
  atomic_store(&(lock->turn), 0);
  atomic_store(&(lock->ticket), 0);
}

/**
 * Spins until ownership of the lock is acquired; this ensures that only thread
 * has ownership of the lock at a time. It is illegal to call this function if
 *  - the lock has never been initialized (see ticket_lock_init()),
 *  - or this thread already owns the passed lock.
 *
 * This function never fails.
 *
 * Arguments:
 *  - lock: a pointer to an initalized ticket_lock_t not currently owned by the
 *          calling thread; passing any other pointer is illegal.
 */
void ticket_lock_acquire(ticket_lock_t *lock)
{
  int me;
  me = atomic_fetch_add(&(lock->ticket), 1);
  while (me != atomic_load(&(lock->turn)))
    ;
}

/**
 * Releases ownership of the lock. It is illegal to call this function
 * if this thread does not own the passed lock.
 *
 * This function never fails.
 *
 * Arguments:
 * - lock: a pointer to a ticket_lock_t currently owned by the calling
 *         thread; passing any other pointer is illegal.
 */
void ticket_lock_release(ticket_lock_t *lock)
{
  atomic_store(&(lock->turn),
               atomic_load(&(lock->turn)) + 1);
}

// --- Problem 3: Linked List ---

/**
 * Initialize a newly allocated list_t to the empty list state. After
 * allocating a list_t it must be initialized before calling
 * list_insert() and list_find() with that list is legal.
 *
 * This function performs no allocations itself, and it never fails.
 * Lists can never be freed once a call to list_insert() is made, since
 * deallocation/removal of elements isn't supported.
 *
 * This function may not be thread-safe, and only one thread should call it.
 *
 * Arguments:
 *  - list: a pointer to a newly allocated list_t whose fields are to be
 *          initialized.
 */
void list_init(list_t *list)
{
  list->head = NULL;
  cas_lock_init(&(list->lock));
}

/**
 * Insert key into list. After list_insert(list, key) returns, a call to
 * list_find(list, key) will henceforth be guaranteed to return true.
 *
 * This function is thread-safe; multiple threads can call list_insert() and
 * list_find() concurrently and safely.
 *
 * This function must perform memory allocation which can fail. Memory
 * allocation failures log a message and cause the current process to exit.
 *
 * Arguments:
 *  - list: a pointer to an initialized list_t; passing any other pointer
 *          is illegal.
 *  - key: a value which is guaranteed to be found by future calls to
 *         list_find() on the same list once this call returns.
 */
void list_insert(list_t *list, int key)
{
  cas_lock_acquire(&(list->lock));
  node_t *new = malloc(sizeof(node_t));
  if (new == NULL)
  {
    perror("malloc");
    cas_lock_release(&(list->lock));
  }
  new->key = key;
  new->next = list->head;
  list->head = new;
  cas_lock_release(&(list->lock));
}

/**
 * Find key in list. Returns true if key exists in list, otherwise returns
 * false.
 *
 * This function is thread-safe; multiple threads can call list_insert() and
 * list_find() concurrently and safely.
 *
 * This function never fails.
 *
 * Arguments:
 *  - list: a pointer to an initialized list_t; passing any other pointer
 *          is illegal.
 *  - key: the value to search for in list; if key exists (due to a prior
 *         list_insert()) the function returns true, otherwise it returns false.
 */
bool list_find(list_t *list, int key)
{
  cas_lock_acquire(&(list->lock));
  node_t *curr = list->head;
  while (curr)
  {
    if (curr->key == key)
    {
      cas_lock_release(&(list->lock));
      return true;
    }
    curr = curr->next;
  }
  cas_lock_release(&(list->lock));
  return false;
}

// --- Problem 4: Monitors and Condition Variables ---

void panic_on_failure(int e, const char *file, int line)
{
  if (e)
  {
    fprintf(stderr, "Failure at %s:%d\n", file, line);
    exit(-1);
  }
}

// You may use this macro to handle return values from pthread_mutex_init,
// pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock,
// pthread_cond_init, pthread_cond_destroy, pthread_cond_wait,
// pthread_cond_signal, and pthread_cond_broadcast.
#define try(expr) panic_on_failure((expr), __FILE__, __LINE__)

/**
 * Implements the monitor. All state fields related to the tank must be tracked
 * as part of an instance of this structure. The tank methods
 * (allocate_and_init_tank, destroy_and_free_tank, enter_tank, and leave tank
 * should not depend on other outside or global state).  The fields of the
 * monitor should include a mutex that ensures  all operations on an initalized
 * tank instance happen under  mutual exclusion.
 */
typedef struct tank
{
  // ADD YOUR OWN FIELDS HERE.
  pthread_cond_t tank_changed;
  pthread_mutex_t mutex;
  int tang_count;
  int stingray_count;
  int shark_count;
} tank_t;

/**
 * Heap allocate an instance of struct tank and return a pointer to it.
 * Must properly initalize all fields within the returned instance. Note
 * that the pthread primitives (mutex, cond vars) require special
 * initialization. If any step in initalization fails this function can
 * return NULL or trigger process exit with an error code.
 *
 * Return value:
 *  An initalized instance of struct tank.
 */
tank_t *allocate_and_init_tank(void)
{
  tank_t *tank = (tank_t *)malloc(sizeof(tank_t));
  tank->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  tank->tank_changed = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
  tank->tang_count = 0;
  tank->stingray_count = 0;
  tank->shark_count = 0;
  return tank;
}

/**
 * Destroy and free all resources that are part of a tank instance.
 * The caller must ensure that no other concurrent operations are ongoing
 * for this tank (that is, a tank can only be destroyed if all animal threads
 * have left the tank and will not make further method calls on the tank).
 *
 * Arguments:
 *   - tank: an instance of a tank created by allocate_and_init_tank().
 */
void destroy_and_free_tank(tank_t *tank)
{
  free(tank);
}

/**
 * Let the calling animal thread into the tank so long is it safe to do so,
 * otherwise block the calling animal thread until it may enter the tank.
 * A calling animal thread must not currently be "in" the tank, otherwise
 * this method make no guarantees.
 *
 * Arguments:
 *   - tank: the tank that the calling animal thread wants to enter; this
 *           must be a valid tank instance created with allocate_and_init_tank,
 *           otherwise, this method make no guarantees.
 *   - type: the type of the calling animal thread (either TANG, STINGRAY,
 *           or SHARK); this method make no guarantees if any other value is
 *           passed.
 */
void enter_tank(tank_t *tank, animal_t type)
{
  // Open Lock
  try(pthread_mutex_lock(&(tank->mutex)));

  // Tang
  if (type == TANG && tank->shark_count == 0)
  {
    printf("%s\n", "add in tangs");
    // Update tang count
    tank->tang_count++;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }
  // Stingray
  if (type == STINGRAY && tank->shark_count == 0)
  {
    printf("%s\n", "add in rays");
    // Update stingray count
    tank->stingray_count++;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }
  // Shark
  while(tank->tang_count > 0 || tank->stingray_count > 0 || tank->shark_count == 2)
  {
    try(pthread_cond_wait(&(tank->tank_changed), &(tank->mutex)));
  }
  if (type == SHARK && tank->tang_count == 0 && tank->stingray_count == 0 && tank->shark_count < 2)
  {
    // Update shark count
    printf("%s\n", "add in sharks");
    tank->shark_count++;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }

  // Close Lock
  try(pthread_mutex_unlock(&(tank->mutex)));
}

/**
 * Let the calling animal thread leave the tank, waking other blocked animals
 * when it is appropriate to do so. A calling animal thread must currently
 * be "in" the tank, otherwise this method make no guarantees.
 *
 * Arguments:
 *   - tank: the tank that the calling animal thread wants to enter; this must
 *           be a valid tank instance created with allocate_and_init_tank,
 *           otherwise, this method make no guarantees.
 *   - type: the type of the calling animal thread (either TANG, STINGRAY,
 *           or SHARK); this method make no guarantees if any other value is
 *           passed.
 */
void leave_tank(tank_t *tank, animal_t type)
{
  // Open Lock
  try(pthread_mutex_lock(&(tank->mutex)));

  // Tang
  if (type == TANG && tank->tang_count > 0 && tank->shark_count == 0)
  {
    // Update tang count
    tank->tang_count--;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }
  // Stingray
  if (type == STINGRAY && tank->stingray_count > 0 && tank->shark_count == 0)
  {
    // Update stingray count
    tank->stingray_count--;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }
  // Shark
  while(tank->tang_count > 0 || tank->stingray_count > 0 || tank->shark_count == 2)
  {
    try(pthread_cond_wait(&(tank->tank_changed), &(tank->mutex)));
  }
  if (type == SHARK && tank->tang_count == 0 && tank->stingray_count == 0 && tank->shark_count > 0)
  {
    // Update shark count
    tank->shark_count--;

    // Broadcast and wake up all threads
    try(pthread_cond_broadcast(&(tank->tank_changed)));
  }

  // Close Lock
  try(pthread_mutex_unlock(&(tank->mutex)));
}
