#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


volatile sig_atomic_t alarm_counter = 0;
 time_t start_time;

//signal handler
void handler(int signum){
  alarm_counter++;
  alarm(1);
}

void sigint_handler(int signum){
  time_t end_time = time(NULL);
  printf("Timer started\n");
  printf("Number of alarms recieved: %d\n", alarm_counter);
  printf("Total time: %ld seconds\n", end_time - start_time);
  exit(0);
}


int main( int argc, char* argv[]){
  start_time = time(NULL);
  signal(SIGALRM, handler); // register handler
  signal(SIGINT, sigint_handler);
  alarm(1);
  while(1){
    // Busy wait
    }
  
  return 0; //never reached

}