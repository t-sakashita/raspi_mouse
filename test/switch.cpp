#include <switch.h>
#include <stdio.h>

int main() {
  if (!bcm2835_init())
    return 1;

  init_switch();

  while (1) {
    printf("read switch0,1,2: %d, %d, %d\n", read_switch0(), read_switch1(), read_switch2());
    delay(500);
  }

  bcm2835_close();
  return 0;
}
