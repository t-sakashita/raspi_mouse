#define BUZZER_PIN 19

#include <pwm.h>

void init_buzzer() {
  bcm2835_gpio_fsel(BUZZER_PIN, BCM2835_GPIO_FSEL_OUTP);
}

static void set_buzzer_frequency(int frequency) {
  int range = PWM_BASECLK / frequency;
  bcm2835_pwm_set_range(1, range);  // PWM_CHANNEL=1
  bcm2835_pwm_set_data(1, range >> 1);
  bcm2835_gpio_fsel(BUZZER_PIN, BCM2835_GPIO_FSEL_ALT5);
}

static void stop_buzzer_pwm() {
  bcm2835_gpio_fsel(BUZZER_PIN, BCM2835_GPIO_FSEL_OUTP);
}

static void play_start_signal() {
  int freq = 2*262;
  set_buzzer_frequency(freq);
  bcm2835_delay(200);
  stop_buzzer_pwm();
  bcm2835_delay(600); // pause
  set_buzzer_frequency(freq);
  bcm2835_delay(200);
  stop_buzzer_pwm();
  bcm2835_delay(800); // pause
  set_buzzer_frequency(2*freq);
  bcm2835_delay(1000);
  stop_buzzer_pwm();
}
