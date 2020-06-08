# Try it out

``` C
int main(int argc, char** argv) {
  size_t size = atoi(argv[2]);
  int neighbors = 3;
  int radius = (neighbors - 1) / 2;
  int states = 2;

  autom_t* autom = create_automat(size, states);
  char* buf = calloc(size + 1, sizeof(char));
}
```
