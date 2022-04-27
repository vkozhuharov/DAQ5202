#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

#include "DT5202.h"


void termination_handler (int signum) {
  //Send Acquisition stop signal


  //Wait to flush the buffers


  //Close the output files
  
  printf("Run stopped \n");
  //write all buffers:
  //Close the connection

  printf("Digitizer closed \n");
  exit(0);

}


int main(int argc, char **argv){

  //Signal processing
  signal (SIGINT, termination_handler);  printf("Trap signals: %d, ",SIGINT);
  signal (SIGHUP, termination_handler);  printf("%d, ",SIGHUP);
  signal (SIGTERM, termination_handler); printf("%d, ",SIGTERM);
  signal (SIGKILL, termination_handler); printf("%d, ",SIGKILL);
  signal (SIGUSR2, termination_handler); printf("%d, ",SIGUSR2);
  signal (SIGFPE, termination_handler);  printf("%d\n",SIGFPE);
  
  DT5202 *board = new DT5202();
  
  board->Init("192.168.2.240","");
  //  board->
  

  
  board->Close();
}
