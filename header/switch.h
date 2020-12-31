#include <bcm2835.h>

#define SWITCH_CENTER_PIN 26
#define SWITCH_LEFT_PIN 21
#define SWITCH_RIGHT_PIN 20

void init_switch() {
  bcm2835_gpio_fsel(SWITCH_LEFT_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SWITCH_CENTER_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(SWITCH_RIGHT_PIN, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(SWITCH_LEFT_PIN, BCM2835_GPIO_PUD_UP); // with pull-up
  bcm2835_gpio_set_pud(SWITCH_CENTER_PIN, BCM2835_GPIO_PUD_UP); // with pull-up
  bcm2835_gpio_set_pud(SWITCH_RIGHT_PIN, BCM2835_GPIO_PUD_UP); // with pull-up
}

uint8_t read_switch0() {
  return bcm2835_gpio_lev(SWITCH_RIGHT_PIN);
}

uint8_t read_switch1() {
  return bcm2835_gpio_lev(SWITCH_CENTER_PIN);
}

uint8_t read_switch2() {
  return bcm2835_gpio_lev(SWITCH_LEFT_PIN);
}
