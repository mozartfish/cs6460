// A few basic tests for the cas_lock, ticket_lock, and list.
// Extend this code in whatever way you see fit to make sure
// your implementations work correctly.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "assignment4.h"

const bool test_cas = false;
const bool test_ticket = false;
const bool test_list = false;
const bool test_monitor = true;

// You can adjust this if you like. Keep in mind, that if you run with too many
// threads (>8 or so) the spinlocks will start to perform very poorly since
// sometimes the lock holding thread will get scheduled out.
const size_t nthreads = 4;
atomic_bool done;

cas_lock_t cas_lock;
ticket_lock_t ticket_lock;
list_t list;

int cas_counter = 0;
int ticket_counter = 0;

atomic_int iterations;
bool list_failure_detected = false;

// A simple test that runs increments and list inserts in a few threads.
// If your locks provide mutual exclusion the counts should match
// at the end. Each item inserted into the list should also be
// found on future accesses.
void* thread_main(void* _) {
  while (!atomic_load(&done)) {
    if (test_cas) {
      cas_lock_acquire(&cas_lock);
      cas_counter++;
      cas_lock_release(&cas_lock);
    }

    if (test_ticket) {
      ticket_lock_acquire(&ticket_lock);
      ticket_counter++;
      ticket_lock_release(&ticket_lock);
    }

    int next = atomic_fetch_add(&iterations, 1);
    if (test_list) {
      list_insert(&list, next);
      bool found = list_find(&list, next);
      // Consider it a failure if the list element we just put into the list
      // doesn't seem to be there.
      if (!found && !list_failure_detected) {
        fprintf(stderr, "list element missing %d\n", next);
        list_failure_detected = true;
      }
    }
  }

  return NULL;
}

void run_test_monitor();

int main(int argc, char* argv[]) {
  ticket_lock_init(&ticket_lock);
  list_init(&list);

  atomic_store(&done, false);
  pthread_t threads[nthreads];
  // Start up several threads all running the same test loop code.
  for (size_t i = 0; i < nthreads; i++) {
    int r = pthread_create(&threads[i], NULL, thread_main, NULL);
    if (r < 0) {
      perror("pthread_create failed");
      exit(-1);
    }
  }

  sleep(1); // Let the threads run for a little bit.
  atomic_store(&done, true); // Then tell them all to quit.
 
  // Wait for all of the threads to exit and record their results.
  for (size_t i = 0; i < nthreads; i++) {
    int r = pthread_join(threads[i], NULL);
    if (r < 0) {
      perror("pthread_join failed");
      exit(-1);
    }
  }

  int i = atomic_load(&iterations);
  int c = atomic_load(&cas_counter);
  int t = atomic_load(&ticket_counter);

  if (test_cas || test_ticket)
    printf("iterations: %d\n\n", i);

  if (test_cas) {
    printf("cas_counter: %d\n", c);
    if (i == c)
      printf("PASS Part 1 cas_lock counts match\n\n");
    else
      printf("FAIL Part 1 cas_lock counts do not match\n\n");
  }

  if (test_ticket) {
    printf("ticket_counter: %d\n", t);
    if (i == t)
      printf("PASS Part 2 ticket_lock counts match\n\n");
    else
      printf("FAIL Part 2 ticket_lock counts do not match\n\n");
  }

  if (test_list) {
    if (list_failure_detected)
      printf("FAIL Part 3 list was missing some inserted keys\n\n");
    else
      printf("PASS Part 3 list has all inserted keys\n\n");
  }

  if (test_monitor)
    run_test_monitor();

  return 0;
}

typedef struct {
  animal_t type;
  struct tank* tank;
} thread_args_t;

atomic_int entry_counts[INVALID] = { 0, 0, 0 };

void check_tank_invariants(animal_t type) {
  static atomic_int c[INVALID] = {0, 0, 0};
  atomic_fetch_add_explicit(&c[type], 1, memory_order_relaxed);
  const int ntangs = atomic_load_explicit(&c[TANG], memory_order_relaxed);
  const int nstingrays = atomic_load_explicit(&c[STINGRAY], memory_order_relaxed);
  const int nsharks = atomic_load_explicit(&c[SHARK], memory_order_relaxed);
  if (nsharks > 2) {
    fprintf(stderr, "FAILURE: detected %d sharks in the tank.\n", nsharks);
    exit(-1);
  }
  if (nsharks && (ntangs + nstingrays)) {
    fprintf(stderr, "FAILURE: detected %d sharks with %d tangs and %d stingrays in the tank.\n", nsharks, ntangs, nstingrays);
    exit(-1);
  }
  atomic_fetch_add_explicit(&c[type], -1, memory_order_relaxed);
}

void* animal_entry(void* args) {
  thread_args_t* targs = (thread_args_t*)args;
  animal_t type = targs->type;
  struct tank* tank = targs->tank;
  free(args);

  while (!atomic_load(&done)) {
    enter_tank(tank, type);
    atomic_fetch_add(&entry_counts[type], 1);
    check_tank_invariants(type);
    leave_tank(tank, type);
  }

  return NULL;
}

void start_animal_thread(pthread_t* thread, struct tank* tank, animal_t type) {
  thread_args_t* args = malloc(sizeof(*args));
  if (args == NULL) {
    perror("allocation failed");
    exit(-1);
  }
  args->type = type;
  args->tank = tank;
  int r = pthread_create(thread, NULL, animal_entry, args);
  if (r < 0) {
    perror("pthread_create failed");
    exit(-1);
  }
}

void run_test_monitor() {
  atomic_store(&done, false);

  const size_t ntangs = 2;
  const size_t nstingrays = 2;
  const size_t nsharks = 8;
  const size_t nthreads = ntangs + nstingrays + nsharks;

  struct tank* tank = allocate_and_init_tank();
  if (!tank)
    exit(-1);

  int t = 0;
  pthread_t threads[nthreads];
  // Start up several threads all running the same test loop code.
  for (size_t i = 0; i < ntangs; i++)
    start_animal_thread(&threads[t++], tank, TANG);
  for (size_t i = 0; i < nstingrays; i++)
    start_animal_thread(&threads[t++], tank, STINGRAY);
  for (size_t i = 0; i < nsharks; i++)
    start_animal_thread(&threads[t++], tank, SHARK);

  sleep(5); // Let the threads run for a little bit.
  atomic_store(&done, true); // Then tell them all to quit.

  // Wait for all of the threads to exit and record their results.
  for (size_t i = 0; i < nthreads; i++) {
    int r = pthread_join(threads[i], NULL);
    if (r < 0) {
      perror("pthread_join failed");
      exit(-1);
    }
  }

  printf("PASS Part 4\n");
  printf("  Tangs entered the tank %d times\n", entry_counts[TANG]);
  printf("  Stingrays entered the tank %d times\n", entry_counts[STINGRAY]);
  printf("  Sharks entered the tank %d times\n", entry_counts[SHARK]);

  destroy_and_free_tank(tank);
}
