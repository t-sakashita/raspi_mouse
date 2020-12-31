#ifndef PWM_H
#define PWM_H

#include <bcm2835.h>
#include <sys/mman.h>
#include <stdio.h>

#define PWM_BASECLK 9600000

void bcm2835_pwm_kill_clock() {
  if (bcm2835_clk == MAP_FAILED) {
    printf("ERROR: Mapping bcm2835_clk is failed.\n");
    return; /* bcm2835_init() failed or not root */
  }

  /* Kill PWM clock */
  bcm2835_peri_write(bcm2835_clk + BCM2835_PWMCLK_CNTL, BCM2835_PWM_PASSWRD | 1 << 5);
  bcm2835_delay(110); /* Prevents clock going slow */
}

#define FIX_BCM2835_PWMCLK_DIV      56

void init_pwm_clocks() { // for buzzer and motors
  bcm2835_pwm_kill_clock();

  //bcm2835_peri_write(bcm2835_clk + BCM2835_PWMCLK_DIV, BCM2835_PWM_PASSWRD | 2 << 12);
  //bcm2835_peri_write(bcm2835_clk + BCM2835_PWMCLK_CNTL, BCM2835_PWM_PASSWRD | 0x11);
  //bcm2835_delayMicroseconds(1000); /* wait for 1msec */

  bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_2);
}

void init_pwm() { // for buzzer and motors
  init_pwm_clocks();

  if (bcm2835_pwm == MAP_FAILED) {
    printf("ERROR: Mapping bcm2835_pwm is failed.\n");
    return; /* bcm2835_init() failed or not root */
  }

  // Enabling clock generator and setting clock source is oscillator for PWM channel 0&1
  bcm2835_peri_write_nb(bcm2835_pwm + BCM2835_PWM_CONTROL, 0x00000000);
  bcm2835_delayMicroseconds(1000); /* wait for 1msec */
  bcm2835_peri_write_nb(bcm2835_pwm + BCM2835_PWM_CONTROL, 0x00008181);

  // Instead of the above, the following commands do not work.
  /*
  bcm2835_pwm_set_mode(0, 0, 0); // enable=off to reset
  bcm2835_pwm_set_mode(1, 0, 0); // enable=off to reset
  bcm2835_delayMicroseconds(1000);
  bcm2835_pwm_set_mode(0, 1, 1); // for PWM Channel 0 (left motor)
  bcm2835_pwm_set_mode(1, 1, 1); // for PWM Channel 1 (right motor)
  */
}

#endif /* PWM_H */
