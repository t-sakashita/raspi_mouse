#define LED0_PIN 25
#define LED1_PIN 24
#define LED2_PIN 23
#define LED3_PIN 18

#include <bcm2835.h>

void init_led() {
  bcm2835_gpio_fsel(LED0_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LED1_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LED2_PIN, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(LED3_PIN, BCM2835_GPIO_FSEL_OUTP);
}

// LED0
static inline void turn_on_led0() {
  bcm2835_gpio_set(LED0_PIN);
}

static inline void turn_off_led0() {
  bcm2835_gpio_clr(LED0_PIN);
}

// LED1
static inline void turn_on_led1() {
  bcm2835_gpio_set(LED1_PIN);
}

static inline void turn_off_led1() {
  bcm2835_gpio_clr(LED1_PIN);
}

// LED2
static inline void turn_on_led2() {
  bcm2835_gpio_set(LED2_PIN);
}

static inline void turn_off_led2() {
  bcm2835_gpio_clr(LED2_PIN);
}

// LED3
static inline void turn_on_led3() {
  bcm2835_gpio_set(LED3_PIN);
}

static inline void turn_off_led3() {
  bcm2835_gpio_clr(LED3_PIN);
}
