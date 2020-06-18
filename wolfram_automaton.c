#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "wolfram_automaton.h"

static uint32_t ipow(int base, int exp)
{
  uint32_t result = 1;

  for (;;) {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    if (!exp)
      break;
    base *= base;
  }

  return result;
}

static int print_bits_spaced(int size, uint8_t a[2 * size], int par,
                             char buf[size + 1])
{
  for (int i = 0; i < size; i++) {
    buf[i] = a[2 * i + par] + '0';
  }
  buf[size] = '\0';

  return 0;
}

uint8_t element_at(size_t idx, autom_t autom[static 1])
{
  return autom->grid[2 * idx + autom->par];
}

int print_autom(autom_t autom[static 1], char* buf)
{
  if (buf)
    return print_bits_spaced(autom->size, autom->grid, autom->par, buf);
  return 1;
}

int update_step(autom_t autom[static 1], rule_t* rule)
{
  size_t size = autom->size;
  int radius = rule->radius;
  int states = autom->states, mult;
  size_t c;

  if (!rule)
    return 1;
  for (size_t r = radius; r < size - radius; r++) {
    c = 0;
    mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[2 * (r + w) + autom->par] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule->lookup[c];
  }
  for (size_t r = 0; r < (size_t)radius; r++) {
    c = 0;
    mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[(2 * (r + w + size) + autom->par) %
                       (2 * size)] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule->lookup[c];
  }
  for (size_t r = size - radius; r < size; r++) {
    c = 0;
    mult = 1;
    for (int w = radius; w >= -radius; w--) {
      c += autom->grid[(2 * (r + w) + autom->par) %
                       (2 * size)] * mult;
      mult *= states;
    }
    autom->grid[!autom->par + 2 * r] = rule->lookup[c];
  }
  autom->par = 1 - autom->par;

  return 0;
}

autom_t* create_automat(size_t size, int states)
{
  autom_t* autom = calloc(1, sizeof(autom_t));

  autom->states = states;
  autom->size = size;
  autom->grid = calloc(2 * size, sizeof(uint8_t));
  autom->par = 0;

  return autom;
}

int init_automat(autom_t autom[static 1], enum InitMode mode)
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


int init_automat_from(size_t size, uint8_t init[size], autom_t autom[static 1])
{
  for (size_t i = 0; i < size; i++) {
    autom->grid[2 * i + autom->par] = init[i];
  }

  return 0;
}

rule_t* rule_from_number(unsigned long rule_number, int states,
                          int neighbors)
{
  int rule_size = ipow(states, neighbors);
  rule_t* rule = calloc(1, sizeof(rule_t));

  rule->states = states;
  rule->rule_size = rule_size;
  rule->radius = (neighbors - 1) / 2;
  rule->lookup = calloc(rule_size, sizeof(uint8_t));
  for (int i = 0; i < rule_size; ++i) {
    rule->lookup[i] = (rule_number / (ipow(states, i))) % states;
  }

  return rule;
}


unsigned long number_from_rule(rule_t rule[static 1])
{
  unsigned long count = 0;
  unsigned long mult = 1;

  for (size_t i = 0; i < rule->rule_size; ++i) {
    count += rule->lookup[i] * mult;
    mult *= rule->states;
  }

  return count;
}
