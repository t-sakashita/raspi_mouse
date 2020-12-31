#include <pwm.h>
#include <motor.h>
#include <stdlib.h>
#include <unistd.h>

void init() {
  if(!bcm2835_init()) {
    exit(1);
  }
  init_pwm();
  init_motors();
}

void finalize() {
  power_off_motors();
  bcm2835_close();
}

int main() {
  init();

  go_forward(100);
  sleep(1);
  stop_motors();

  finalize();
}
