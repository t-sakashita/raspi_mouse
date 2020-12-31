#include <distance_sensor.h>
#include <pwm.h>
#include <motor.h>
#include <buzzer.h>
#include <switch.h>
#include <keyboard_tool.h>
#include <signal_ctrl_c.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

struct timespec start_time, end_time;

void run_step() {
  const int var_gofw_lim = 200;
  const int var_turn_lim = 20;
  const unsigned int var_gofw_v = 200;
  const unsigned int var_gofw_t = 200;
  const unsigned int var_gobk_v = 100;
  const unsigned int var_gobk_t = 100;

  const unsigned int var_turn_v = 200;
  const unsigned int var_turn_t = 200;

  int distance_r, distance_rf, distance_lf, distance_l;
  read_distance_from_sensors_3average(distance_r, distance_rf, distance_lf, distance_l);
  //read_distance_from_sensors(distance_r, distance_rf, distance_lf, distance_l);

  printf("distance_l,lf,rf,r = %d,%d,%d,%d\n", distance_l, distance_lf, distance_rf, distance_r);
  if (distance_lf + distance_rf <= var_gofw_lim) {
    printf("go_forward\n");
    go_forward(var_gofw_v);
  } else if (distance_r - distance_l > var_turn_lim) {
    printf("turn_left %d\n", distance_r - distance_l);
    turn_left(var_turn_v);
  } else if (distance_l - distance_r > var_turn_lim) {
    printf("turn_right\n");
    turn_right(var_turn_v);
  } else {
    printf("go_backward\n");
    go_backward(var_gobk_v);
  }
}

void finalize() {
  finalize_distance_sensor();
  stop_motors();
  power_off_motors();
  restore_setting_keyboard();
  bcm2835_close();
}

void terminate() {
  clock_gettime(CLOCK_REALTIME, &end_time);
  finalize();
  double time = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) * 1e-9;
  printf("Time = %lf [seconds]\n", time);
}

void handler_for_forced_termination( int signo ) {
  terminate();
  printf("You pressed CTRL+C.\n");
  exit(1);
}

void abort() {
  finalize();
  exit(1);
}

void init() {
  if(!bcm2835_init()) {
    exit(1);
  }
  init_distance_sensor();
  init_pwm();
  init_buzzer();
  init_motors();
  init_keyboard();
  init_signal_ctrl_c(&handler_for_forced_termination);
}

void wait_for_start() {
  printf("Press key 'y'\n");
  while(true) {
    char c = getchar();
    if(c == 'y')  break;
    else if(c == 'q')  abort();
  }
}


int main() {
  init();

  wait_for_start();
  play_start_signal();
  clock_gettime(CLOCK_REALTIME, &start_time);

  while(true) {
    run_step();
    if(!read_switch0())  break;
    if(getchar() == 'q')  break;
    usleep(100*1000);
  }

  terminate();
}
