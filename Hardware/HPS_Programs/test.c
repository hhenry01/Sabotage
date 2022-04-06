#include <stdio.h>
#include "address_map.h"

#define TOUCH_ADDR  (volatile int*) 0xFF2030e0

int main(void) {
  while (1)
    printf("%d\n", *TOUCH_ADDR);
  return 0;
}
