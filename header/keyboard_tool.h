#include <termios.h>
#include <fcntl.h>
#include <time.h>

struct termios save_settings;

void init_keyboard() {
  struct termios settings;
  tcgetattr(0,&save_settings);
  settings = save_settings;

  settings.c_lflag &= ~(ECHO|ICANON);  /* no echo back, buffering */
  settings.c_cc[VTIME] = 0;
  settings.c_cc[VMIN] = 1;
  tcsetattr(0,TCSANOW,&settings);
  fcntl(0,F_SETFL,O_NONBLOCK);  /* No blocking for the input from standard input */
}

void restore_setting_keyboard() {
  tcsetattr(0,TCSANOW,&save_settings);
}
