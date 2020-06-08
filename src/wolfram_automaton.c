#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include "wolfram_automaton.h"
#include "utils/compress.h"
#include "utils/utils.h"


static int print_bits_spaced(int size, uint8_t a[2 * size], int par,
                      char buf[size + 1])
{
  for (int i = 0; i < size; i++) {
    buf[i] = a[2 * i + par] + '0';
  }
  buf[size] = '\0';
  return 0;
}

int print_autom(autom_t* autom, char* buf)
{
  return print_bits_spaced(autom->size, autom->grid, autom->par, buf);
}

int update_step(size_t size, autom_t* autom,
                uint8_t* rule, int states, int radius)
{
  for (size_t r = radius; r < size - radius; r++) {
    size_t c = 0;
    int mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[2 * (r + w) + autom->par] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule[c];
  }
  for (size_t r = 0; r < (size_t)radius; r++) {
    size_t c = 0;
    int mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[(2 * (r + w + size) + autom->par) %
                       (2 * size)] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule[c];
  }
  for (size_t r = size - radius; r < size; r++) {
    size_t c = 0;
    int mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[(2 * (r + w) + autom->par) %
                       (2 * size)] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule[c];
  }
  autom->par = 1 - autom->par;

  return 0;
}

autom_t* create_automat(size_t size, int states)
{
  autom_t* autom = calloc(1, sizeof(autom_t));

  autom->states = states;
  autom->size = size;
  autom->grid = calloc(size, sizeof(uint8_t));
  autom->par = 0;

  return autom;
}

int init_automat(autom_t* autom, enum InitMode mode)
{
  size_t size = autom->size;
  int states = autom->states;
  if (mode == ONE) {
    for (size_t i = 0; i < size; i++) {
      autom->grid[2 * i + autom->par] = 0;
    }
    autom->grid[size + autom->par] = 1;
  }
  else if (mode == RANDOM) {
    for (size_t i = 0; i < size; i++) {
      autom->grid[2 * i + autom->par] = rand() % states;
    }
  } else if (mode == RAND_SMALL) {
    for (size_t i = size / 2 - size / 5; i < size / 2 + size / 5; i++) {
      autom->grid[2 * i + autom->par] = rand() % states;
    }
  }
  return 0;
}

uint8_t* rule_from_number(unsigned long rule_number, int states,
                          int neighbors)
{
  int rule_size = ipow(states, neighbors);
  uint8_t* rule = calloc(rule_size, sizeof(uint8_t));
  for (int i = 0; i < rule_size; ++i) {
    rule[i] = (rule_number / (ipow(states, i))) % states;
  }
  return rule;
}


unsigned long number_from_rule(int states, size_t rule_size,
                               uint8_t* rule)
{
  unsigned long count = 0;
  unsigned long mult = 1;
  for (size_t i = 0; i < rule_size; ++i) {
    count += rule[i] * mult;
    mult *= states;
  }
  return count;
}

static void write_step(size_t size, size_t rule_size, uint8_t A[size],
                       uint8_t rule[rule_size], int step, int states)
{
  FILE* steps_file;
  char* steps_fname;
  asprintf(&steps_fname, "steps/out%lu_%i.step",
           number_from_rule(states, rule_size, rule), step);

  steps_file = fopen(steps_fname, "w+");

  char output_string[size + 1];
  /* print_bits_spaced(size, A, output_string); */
  fputs(output_string, steps_file);
  fclose(steps_file);
}

