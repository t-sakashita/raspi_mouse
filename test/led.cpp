#include <led.h>
#include <stdlib.h>
#include <unistd.h>

void init() {
  if(!bcm2835_init()) {
    exit(1);
  }

  init_led();
}

void finalize() {
  bcm2835_close();
}

int main() {
  init();

  for (int i=0; i<10; ++i) {
    turn_on_led0();
    usleep(40*1000);
    turn_off_led0();

    turn_on_led1();
    usleep(40*1000);
    turn_off_led1();

    turn_on_led2();
    usleep(40*1000);
    turn_off_led2();

    turn_on_led3();
    usleep(40*1000);
    turn_off_led3();
  }

  finalize();
}
