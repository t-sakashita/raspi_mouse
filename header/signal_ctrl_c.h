#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void init_signal_ctrl_c(void (*handler)(int)) {
  struct sigaction sa;
  memset( &sa, 0, sizeof(struct sigaction) ); // zero-clear

  sa.sa_handler = handler;
  sa.sa_flags |= SA_RESTART;

  if( sigaction( SIGINT, &sa, NULL ) < 0 ) {
    perror("ERROR: registering sigaction");
  }
}
