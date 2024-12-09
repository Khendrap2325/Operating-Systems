/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t alarm_counter = 0;
time_t start_time;

void handler(int signum)
{ //signal handler
  alarm_counter++;
  alarm(1); 
}

void sigint_handler(int signum) 
{
  time_t end_time = time(NULL); 
  printf("Number of alarms received: %d\n", alarm_counter);
  printf("Total execution time: %ld seconds\n", end_time - start_time);
  exit(0); // Exit the program
}



int main(int argc, char *argv[]) 
{
  // Record the start time
  start_time = time(NULL);

  // Register the alarm handler
  signal(SIGALRM, handler);

  // Register the SIGINT handler
  signal(SIGINT, sigint_handler);

  // Schedule the first alarm
  alarm(1);

  // Main loop
  while (1) {
    // Busy wait, nothing else to do
  }

  return 0; // Never reached
}