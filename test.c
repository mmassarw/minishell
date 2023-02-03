#include <stdio.h>

int main(void) {
  int i;
  for (i = 0; i < 20; i++) {
    printf("%lc", 0x2588);
  }
  printf("\n");
  return 0;
}

