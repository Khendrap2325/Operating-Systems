#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define LOOP_COUNT 25

void ParentProcess(int *BankAcc, int *Turn);
void ChildProcess(int *BankAcc, int *Turn);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;


     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     //printf("Server has received a shared memory of 2 integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     //printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0;//Bank Account
     ShmPTR[1] = 0;//Turn
     printf("Shared memory initialized..\n");

     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ChildProcess(&ShmPTR[0], &ShmPTR[1]);
          exit(0);
     }else{
          ParentProcess(&ShmPTR[0], &ShmPTR[1]);

     }

     wait(&status);
     shmdt((void *) ShmPTR);
     shmctl(ShmID, IPC_RMID, NULL);
     printf("shared memory has been detached and removed  ...\n");
     printf("Server exits...\n");
     return 0;
}

void ParentProcess(int *BankAcc, int *Turn){
     srand(time(NULL));
     for (int i=0; i<LOOP_COUNT; i++){
          sleep(rand()%6);
          int account = *BankAcc;
          while (*Turn !=0);

          if (account <= 100){
               int balance = rand() % 101;
               if (balance %2==0){
                    account += balance;
                    printf("Dear old Dad: Deposits %d / Balance = $%d\n", balance, account);    
               } else {
                    printf("Dear old Dad: Doesn't have any money to give\n");

               }
          }else{
               printf("Dear old Dad: Thinks Student has enough cash ($%d)\n", account);
          }
          *BankAcc = account; //Update bank account
          *Turn =1;
     }
}

void  ChildProcess(int *BankAcc, int *Turn)
{
     srand(time(NULL)+1);
     for (int i=0; i<LOOP_COUNT; i++){
          sleep(rand()%6);

          int account = *BankAcc;
          while (*Turn !=1);

          int balance = rand() % 51;
          printf("Poor Student needs $%d\n", balance);

          if (balance <= account){
               account -= balance;
               printf("Poor Student: Withdraws %d / Balance = $%d\n", balance, account);    
               } else {
                    printf("Poor Student : Not enough Cash ($%d)\n", account);
               }
          *BankAcc = account; 
          *Turn =0;
          }
          
}