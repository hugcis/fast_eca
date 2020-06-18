#include <stdlib.h>
#include <stdint.h>

enum InitMode {ONE, RANDOM, RAND_SMALL};

typedef struct rule_s {
  size_t rule_size;
  uint8_t* lookup;
  int radius;
  int states;
} rule_t;

typedef struct autom_s {
  size_t size;
  uint8_t* grid;
  int states;
  uint8_t par;
} autom_t;


/** Update the automatong one step with the corresponding rule.
    (radius is 1 for ECAs)*/
int update_step(autom_t autom[static 1], rule_t rule[static 1]);

/** Format the automaton to a buffer for printing */
int print_autom(autom_t autom[static 1], char* buf);

/** Make a rule from the rule number (neighbors is 3 for ECAs) */
rule_t* rule_from_number(unsigned long rule_number, int states,
                         int neighbors);

/** Find the number corresponding to a given rule */
unsigned long number_from_rule(rule_t rule[static 1]);

/** Init an automaton according to one of the InitMode */
int init_automat(autom_t autom[static 1], enum InitMode mode);
int init_automat_from(size_t size, uint8_t init[size], autom_t autom[static 1]);

autom_t* create_automat(size_t size, int states);

/** Get the element at index `idx` in the grid. */
uint8_t element_at(size_t idx, autom_t autom[static 1]);
