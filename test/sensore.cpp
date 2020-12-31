#include <distance_sensor.h>
#include <switch.h>
#include <keyboard_tool.h>
#include <signal_ctrl_c.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int run_step() {
  int distance_r, distance_rf, distance_lf, distance_l;
  read_distance_from_sensors_3average(distance_r, distance_rf, distance_lf, distance_l);
  //read_distance_from_sensors(distance_r, distance_rf, distance_lf, distance_l);
  printf("distance_lf,l,r,rf = %d, %d, %d, %d\n", distance_lf, distance_l, distance_r, distance_rf);
}

void finalize() {
  finalize_distance_sensor();
  restore_setting_keyboard();
  bcm2835_close();
}

void handler_for_forced_termination( int signo ) {
  finalize();
  printf("You pressed CTRL+C.\n");
  exit(1);
}

void init() {
  if(!bcm2835_init()) {
    exit(1);
  }
  init_distance_sensor();
  init_keyboard();
  init_signal_ctrl_c(&handler_for_forced_termination);
}


int main(int argc, char *argv[]) {
  init();

  printf("To stop, press the key 'q'\n");

  while(true) {
    run_step();
    if(!read_switch0())  break;
    if(getchar() == 'q')  break;
    usleep(100*1000);
  }

  finalize();
}
