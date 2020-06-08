# Fast and minimal C ECA implementation

## Methods

## Try it out

Put this C code into a file `main.c`, next to the folder `fast_eca`
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

And run
```
./main 100 110
```
