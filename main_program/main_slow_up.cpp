#include <distance_sensor.h>
#include <motor.h>
#include <buzzer.h>
#include <pwm.h>
#include <switch.h>
#include <keyboard_tool.h>
#include <signal_ctrl_c.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

// original parameters

int forward_lim = 400;
int turn_lim = 10;

int forward_min_freq = 200;
int forward_max_freq = 1200;
int forward_step_freq = 40;
int turn_min_freq = 200;
int turn_max_freq = 300;
int turn_step_freq = 20;

struct timespec start_time, end_time;

int current_freq;
char current_status = ' ';

static inline void accelerate_forward() { // accelerate up to max_freq
  if (current_freq + forward_step_freq <= forward_max_freq) {
    current_freq += forward_step_freq;
    change_frequencies(current_freq);
    printf("前進を%d[Hz]に加速\n", current_freq);
  } else {
    printf("前進は%d[Hz]で上限に達した。\n", current_freq);
  }
}

static inline void change_to_forward() {
  current_freq = forward_min_freq;
  go_forward(current_freq);
  current_status = 'F';
}

static inline void change_to_backward() {
  current_freq = forward_min_freq;
  go_backward(current_freq);
  current_status = 'B';
}

static inline void accelerate_to_turn_left() { // accelerate up to max_freq
  if (current_freq + turn_step_freq <= turn_max_freq) {
    current_freq += turn_step_freq;
    change_frequencies(current_freq);
    printf("左折を%d[Hz]に加速\n", current_freq);
  } else {
    printf("左折は%d[Hz]で上限に達した。\n", current_freq);
  }
}

static inline void change_to_turn_left() {
  current_freq = turn_min_freq;
  turn_left(current_freq);
  current_status = 'L';
}

static inline void accelerate_to_turn_right() { // accelerate up to max_freq
  if (current_freq + turn_step_freq <= turn_max_freq) {
    current_freq += turn_step_freq;
    change_frequencies(current_freq);
    printf("右折を%d[Hz]に加速\n", current_freq);
  } else {
    printf("右折は%d[Hz]で上限に達した。\n", current_freq);
  }
}

static inline void change_to_turn_right() {
  current_freq = turn_min_freq;
  turn_right(current_freq);
  current_status = 'R';
}

static inline void check_changing(int distance_r, int distance_rf, int distance_lf, int distance_l) {
  if (distance_lf + distance_rf <= forward_lim) {
    printf("前進に変更\n");
    change_to_forward();
  } else if (distance_r - distance_l > turn_lim) {
    printf("左折に変更\n");
    change_to_turn_left();
  } else if (distance_l - distance_r > turn_lim) {
    printf("右折に変更\n");
    change_to_turn_right();
  } else {
    printf("後退に変更\n");
    change_to_backward();
  }
}

static inline void run_step() {
  int distance_r, distance_rf, distance_lf, distance_l;
  read_distance_from_sensors_3average(distance_r, distance_rf, distance_lf, distance_l);
  //read_distance_from_sensors(distance_r, distance_rf, distance_lf, distance_l);
  printf("距離センサの値（左直，左斜，右斜、右直）= %d, %d, %d, %d\n", distance_lf, distance_l, distance_r, distance_rf);

  if (current_status == 'F') { // whether continuing to forward or not.
    if (distance_lf + distance_rf <= forward_lim) {
      accelerate_forward();
    } else {
      check_changing(distance_r, distance_rf, distance_lf, distance_l);
    }
  } else if (current_status == 'L') { // whether continuing to left or not.
    if (distance_r - distance_l > turn_lim) {
      accelerate_to_turn_left();
    } else {
      check_changing(distance_r, distance_rf, distance_lf, distance_l);
    }
  } else if (current_status == 'R') { // whether continuing to right or not.
    if (distance_l - distance_r > turn_lim) {
      accelerate_to_turn_right();
    } else {
      check_changing(distance_r, distance_rf, distance_lf, distance_l);
    }
  } else { // whether continuing to back or not. The cases 'B' or ' '
    check_changing(distance_r, distance_rf, distance_lf, distance_l);
    //go_backward_accelerate();
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
  if (start_time.tv_sec != 0 && start_time.tv_nsec != 0) {
    double time = end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("\n走行時間 = %lf [秒]\n", time);
  }
}

void handler_for_forced_termination( int signo ) {
  terminate();
  printf("CTRL+Cが押されたため、終了しました。\n");
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
  // Zero-clear timer, to detemoine displaying elapsed time or not when aboring program.
  start_time.tv_sec = 0;
  start_time.tv_nsec = 0;
}

void wait_for_start() {
  printf("走行させるには、キーボードのyを押してください。\n");
  printf("終了するには、キーボードのqを押してください。\n");
  while(true) {
    char c = getchar();
    if(c == 'y')  break;
    else if(c == 'q') {
      terminate();
      exit(0);
    }
  }
}

void parse_arugument(int argc, char *argv[], int i, int& var, const char name[]) {
  char *err_str;

  if (argc > i) {
    var = strtol(argv[i], &err_str, 10);
    if (*err_str != '\0') {
      printf("  エラー: %d番目の引数%s = %sが不正です。数字だけを指定してください。\n", i, name, argv[i]);
      exit(1);
    } else if (var < 0) {
      printf("  エラー: %d番目の引数%s = %sが負です。正の整数を指定してください。\n", i, name, argv[i]);
      exit(1);
    }
    printf("  %d番目の引数%s = %d\n", i, name, var);
  } else {
    printf("  %d番目の引数%s = %d (デフォルト)\n", i, name, var);
  }
}

void set_variables_from_command_line(int argc, char *argv[]) {
  printf("コマンドライン引数を抽出しています：\n");
  parse_arugument(argc, argv, 1, forward_lim, "forward_lim");
  parse_arugument(argc, argv, 2, turn_lim, "turn_lim");

  parse_arugument(argc, argv, 3, forward_min_freq, "forward_min_freq");
  parse_arugument(argc, argv, 4, forward_max_freq, "forward_max_freq");
  parse_arugument(argc, argv, 5, forward_step_freq, "forward_step_freq");

  parse_arugument(argc, argv, 6, turn_min_freq, "turn_min_freq");
  parse_arugument(argc, argv, 7, turn_max_freq, "turn_max_freq");
  parse_arugument(argc, argv, 8, turn_step_freq, "turn_step_freq");
}

int main(int argc, char *argv[]) {
  set_variables_from_command_line(argc, argv);

  init();

  wait_for_start();

  power_on_motors();
  play_start_signal();
  clock_gettime(CLOCK_REALTIME, &start_time);

  while(true) {
    run_step();
    if(!read_switch0())  break;
    if(getchar() == 'q')  break;
    usleep(10*1000);
  }

  terminate();
}
