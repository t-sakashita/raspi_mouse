#include <pwm.h>
#include <buzzer.h>
#include <stdlib.h>
#include <unistd.h>

void init() {
  if(!bcm2835_init()) {
    exit(1);
  }
  init_pwm();
  init_buzzer();
}

void finalize() {
  bcm2835_close();
}

int main() {
  init();

  play_start_signal();
}
