#define MOTOR_POWER_PIN 5
#define MOTOR_DIRECTION_R_PIN 6
#define MOTOR_CLOCK_R_PIN 13
#define MOTOR_DIRECTION_L_PIN 16
#define MOTOR_CLOCK_L_PIN 12

#include <pwm.h>

void power_on_motors() {
  bcm2835_gpio_set(MOTOR_POWER_PIN);
}

void power_off_motors() {
  bcm2835_gpio_clr(MOTOR_POWER_PIN);
}

void init_motors() {
  bcm2835_gpio_fsel(MOTOR_POWER_PIN, BCM2835_GPIO_FSEL_OUTP);
  // resets right motor's clock
  bcm2835_gpio_fsel(MOTOR_CLOCK_R_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(MOTOR_CLOCK_R_PIN);
  // resets left motor's clock
  bcm2835_gpio_fsel(MOTOR_CLOCK_L_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_clr(MOTOR_CLOCK_L_PIN);

  //power_on_motors();
}

void stop_left_motor() {
  bcm2835_gpio_fsel(MOTOR_CLOCK_L_PIN, BCM2835_GPIO_FSEL_OUTP);
}

void stop_right_motor() {
  bcm2835_gpio_fsel(MOTOR_CLOCK_R_PIN, BCM2835_GPIO_FSEL_OUTP);
}

void stop_motors() {
  stop_left_motor();
  stop_right_motor();
}

static inline void set_left_motor_frequency(int frequency) {
  int range = PWM_BASECLK / frequency;
  bcm2835_pwm_set_range(0, range);  // PWM_CHANNEL=0
  bcm2835_pwm_set_data(0, range >> 1);
  bcm2835_gpio_fsel(MOTOR_CLOCK_L_PIN, BCM2835_GPIO_FSEL_ALT0);
}

static inline void set_right_motor_frequency(int frequency) {
  int range = PWM_BASECLK / frequency;
  bcm2835_pwm_set_range(1, range);  // PWM_CHANNEL=1
  bcm2835_pwm_set_data(1, range >> 1);
  bcm2835_gpio_fsel(MOTOR_CLOCK_R_PIN, BCM2835_GPIO_FSEL_ALT0);
}

static inline void set_left_motor_forward() {
  bcm2835_gpio_clr(MOTOR_DIRECTION_L_PIN);
  //bcm2835_gpio_write(MOTOR_DIRECTION_L_PIN, LOW);
}

static inline void set_right_motor_forward() {
  bcm2835_gpio_set(MOTOR_DIRECTION_R_PIN);
  //bcm2835_gpio_write(MOTOR_DIRECTION_R_PIN, HIGH);
}

static inline void set_left_motor_backward() {
  bcm2835_gpio_set(MOTOR_DIRECTION_L_PIN);
  //bcm2835_gpio_write(MOTOR_DIRECTION_L_PIN, HIGH);
}

static inline void set_right_motor_backward() {
  bcm2835_gpio_clr(MOTOR_DIRECTION_R_PIN);
  //bcm2835_gpio_write(MOTOR_DIRECTION_R_PIN, LOW);
}

static inline void go_forward(int frequency) {
  set_left_motor_forward();
  set_right_motor_forward();
  set_left_motor_frequency(frequency);
  set_right_motor_frequency(frequency);
}

static inline void go_backward(int frequency) {
  set_left_motor_backward();
  set_right_motor_backward();
  set_left_motor_frequency(frequency);
  set_right_motor_frequency(frequency);
}

static inline void turn_left(int frequency) {
  set_left_motor_backward();
  set_right_motor_forward();
  set_left_motor_frequency(frequency);
  set_right_motor_frequency(frequency);
}

static inline void turn_right(int frequency) {
  set_left_motor_forward();
  set_right_motor_backward();
  set_left_motor_frequency(frequency);
  set_right_motor_frequency(frequency);
}

// Changing velocities of motors while keeping their directions
static inline void change_frequencies(int frequency) {
  set_left_motor_frequency(frequency);
  set_right_motor_frequency(frequency);
}
