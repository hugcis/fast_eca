# Fast and minimal C ECA implementation

## Methods

The following methods are available in the header
`"fast_eca/wolfram_automaton.h"`:

- `autom_t* create_automat(size_t size, int states)`: This creates a pointer to
  an `autom_t` structure, representing an automaton of size `size` with `states`
  states.
- `int init_automat(autom_t* autom, enum InitMode mode)`: Initializes the
  automaton with:
  1. One cell in state `1` in the middle of the grid and the rest to `0` if `mode == ONE`
  2. Random  states if `mode == RANDOM`
- `uint8_t* rule_from_number(unsigned long rule_number, int states, int
  neighbors)`: Creates an ECA rule (array of uint8_t) from its number, number of
  states and neighborhood size.
- `int update_step(autom_t* autom, uint8_t* rule, int radius)`: Updates the
  automaton one step at a time with the rule and radius (radius is `neighbords/2 - 1`).
- `int print_autom(autom_t* autom, char* buf)`: Format the automaton to a
  buffer. The buffer needs to be allocated to at least the size of `(size + 1) char`.

## Try it out

Clone this folder in your project. 
```
git clone https://github.com/hugcis/fast_eca.git
```

Put this C code into a file `main.c`, next to the folder `fast_eca`:
``` C
#include <stdio.h>
#include <inttypes.h>
#include "fast_eca/wolfram_automaton.h"

int main(int argc, char** argv) {
  size_t size = atoi(argv[1]);
  int neighbors = 3;
  int radius = (neighbors - 1) / 2;
  int states = 2;

  uint8_t* rule = rule_from_number(atoi(argv[2]), states, neighbors);
  autom_t* autom = create_automat(size, states);
  char* buf = calloc(size + 1, sizeof(char));
  init_automat(autom, ONE);

  for (int i = 0; i < 100; ++i) {
    update_step(autom, rule, radius);
    print_autom(autom, buf);
    fprintf(stdout, "%s\n", buf);
  }
}
```

Compile with the following command:
```
gcc -O3 -o main main.c -I. fast_eca/wolfram_automaton.c
```

And run (100 steps, rule 110)
```
./main 100 110
```
