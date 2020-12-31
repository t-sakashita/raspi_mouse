#include <distance_sensor.h>
#include <stdio.h>

int main() {
  if (!bcm2835_init())
    return 1;
  init_distance_sensor();

  int distance_r, distance_rf, distance_lf, distance_l;
  read_distance_from_sensors_5average(distance_r, distance_rf, distance_lf, distance_l);
  printf("distance_l,lf,rf,r = %d,%d,%d,%d\n", distance_r, distance_rf, distance_lf, distance_l);

  finalize_distance_sensor();
  bcm2835_close();
  return 0;
}
