#include <bcm2835.h>

#define R_AD_CH 3
#define L_AD_CH 0
#define RF_AD_CH 2
#define LF_AD_CH 1

#define R_LED_SENSOR_PIN 22
#define L_LED_SENSOR_PIN 4
#define RF_LED_SENSOR_PIN 27
#define LF_LED_SENSOR_PIN 17

static inline unsigned int read_from_sensor(int ch) {
  char out_ch[][3] = { { 0b00000110, 0b00000000, 0b00000000 },
		       { 0b00000110, 0b01000000, 0b00000000 },
		       { 0b00000110, 0b10000000, 0b00000000 },
		       { 0b00000110, 0b11000000, 0b00000000 } };

  char ch_data[] = { 0x00, 0x00, 0x00 };

  bcm2835_spi_transfernb(out_ch[ch], ch_data, 3);
  return ((ch_data[1] & 0xf) << 8) | ch_data[2];
}

static inline int read_distance_from_sensor(int ch, int pin) {
  unsigned int distance0 = read_from_sensor(ch);
  bcm2835_gpio_set(pin);
  bcm2835_delayMicroseconds(100);//30);
  unsigned int distance = read_from_sensor(ch);
  bcm2835_gpio_clr(pin);
  //bcm2835_delayMicroseconds(100);//30);
  return distance - distance0;
}

static inline void read_distance_from_sensors(int& distance_r, int& distance_rf, int& distance_lf, int& distance_l) {
  distance_l  = read_distance_from_sensor(0,  L_LED_SENSOR_PIN);
  distance_lf = read_distance_from_sensor(1, LF_LED_SENSOR_PIN);
  distance_rf = read_distance_from_sensor(2, RF_LED_SENSOR_PIN);
  distance_r  = read_distance_from_sensor(3,  R_LED_SENSOR_PIN);
}

static inline void read_distance_from_sensors_3average(int& distance_r, int& distance_rf, int& distance_lf, int& distance_l) {
  int distance_r_tmp, distance_rf_tmp, distance_lf_tmp, distance_l_tmp;

  distance_r = 0;  distance_rf = 0;  distance_lf = 0;  distance_l = 0;
  for (int i=0; i<3; ++i) {
    read_distance_from_sensors(distance_r_tmp, distance_rf_tmp, distance_lf_tmp, distance_l_tmp);
    distance_r += distance_r_tmp;
    distance_rf += distance_rf_tmp;
    distance_lf += distance_lf_tmp;
    distance_l += distance_l_tmp;
  }
  distance_r /= 3;  distance_rf /= 3;  distance_lf /= 3;  distance_l /= 3;
}

static inline void read_distance_from_sensors_5average(int& distance_r, int& distance_rf, int& distance_lf, int& distance_l) {
  int distance_r_tmp, distance_rf_tmp, distance_lf_tmp, distance_l_tmp;

  distance_r = 0;  distance_rf = 0;  distance_lf = 0;  distance_l = 0;
  for (int i=0; i<5; ++i) {
    read_distance_from_sensors(distance_r_tmp, distance_rf_tmp, distance_lf_tmp, distance_l_tmp);
    distance_r += distance_r_tmp;
    distance_rf += distance_rf_tmp;
    distance_lf += distance_lf_tmp;
    distance_l += distance_l_tmp;
  }
  distance_r /= 5;  distance_rf /= 5;  distance_lf /= 5;  distance_l /= 5;
}

void init_distance_sensor() {
  bcm2835_spi_begin();
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW); // In mode3, Polarity is LOW.
}

void finalize_distance_sensor() {
  bcm2835_spi_end();
}
