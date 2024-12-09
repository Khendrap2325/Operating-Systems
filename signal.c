/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_obtained = 0;

//signal handler
void handler(int signum)
{ 
  printf("Hello World!\n");
  signal_obtained = 1;
  alarm(5);
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(5); //Schedule a SIGALRM for 5 seconds
  while(1){
    if (signal_obtained){
      printf("Turing was right!\n");
      signal_obtained = 0;
    }
    usleep(100000);
  };
  return 0; //never reached
}