#ifndef ASSIGNMENT1_H
#define ASSIGNMENT1_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <unistd.h>

uint64_t byte_sort(uint64_t arg);

uint64_t nibble_sort(uint64_t arg);

struct elt {
  char val;
  struct elt* link;
};

struct elt* str_to_list(const char* str);

enum format_t {
  OCT = 99, BIN, HEX
};

void convert(enum format_t mode, uint64_t value);

void draw_u(void);

#endif
