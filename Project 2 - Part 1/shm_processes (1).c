#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_BALANCE 500

// Shared variables
typedef struct {
    int BankAccount;
    int localBalance;
} SharedData;

// Semaphore operations
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int createSemaphore(int key, int initialValue) {
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("Semaphore creation failed");
        exit(1);
    }

    union semun arg;
    arg.val = initialValue;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("Semaphore initialization failed");
        exit(1);
    }
    return semid;
}

void waitSemaphore(int semid) {
    struct sembuf sb = {0, -1, 0};  // P operation (wait)
    if (semop(semid, &sb, 1) == -1) {
        perror("Semaphore wait failed");
        exit(1);
    }
}

void signalSemaphore(int semid) {
    struct sembuf sb = {0, 1, 0};  // V operation (signal)
    if (semop(semid, &sb, 1) == -1) {
        perror("Semaphore signal failed");
        exit(1);
    }
}

void parentProcess(SharedData *data, int semid) {
    srand(time(NULL));
    while (1) {
        sleep(rand() % 6);  // Sleep for a random time (0-5 seconds)

        printf("Dear Old Dad: Attempting to Check Balance\n");

        if (rand() % 2 == 0) {  // Random even check
            waitSemaphore(semid);
            printf("Dear Old Dad: Checking balance: $%d\n", data->BankAccount);
            if (data->BankAccount < 100) {
                data->localBalance = data->BankAccount;
                int amount = (rand() % 101) + 100;  // Random deposit between 100 and 200
                data->localBalance += amount;
                printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", amount, data->localBalance);
                data->BankAccount = data->localBalance;
            } else {
                printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", data->BankAccount);
            }
            signalSemaphore(semid);
        } else {  // Random odd check
            printf("Dear Old Dad: Last Checking Balance = $%d\n", data->BankAccount);
        }
        sleep(1);  // Parent sleeps briefly before the next iteration
    }
}

void childProcess(SharedData *data, int semid) {
    srand(time(NULL));
    while (1) {
        sleep(rand() % 6);  // Sleep for a random time (0-5 seconds)

        printf("Poor Student: Attempting to Check Balance\n");

        if (rand() % 2 == 0) {  // Random even check to withdraw money
            waitSemaphore(semid);
            printf("Poor Student: Checking balance: $%d\n", data->BankAccount);
            int need = (rand() % 51) + 50;  // Random need between 50 and 100
            printf("Poor Student needs $%d\n", need);
            if (need <= data->BankAccount) {
                data->localBalance = data->BankAccount;
                data->localBalance -= need;
                printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, data->localBalance);
                data->BankAccount = data->localBalance;
            } else {
                printf("Poor Student: Not Enough Cash ($%d)\n", data->BankAccount);
            }
            signalSemaphore(semid);
        } else {  // Random odd check
            printf("Poor Student: Last Checking Balance = $%d\n", data->BankAccount);
        }
        sleep(1);  // Child sleeps briefly before the next iteration
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_dads> <num_students>\n", argv[0]);
        exit(1);
    }

    int numDads = atoi(argv[1]);
    int numStudents = atoi(argv[2]);

    // Create shared memory for BankAccount
    int shmID = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmID < 0) {
        perror("Shared memory creation failed");
        exit(1);
    }

    SharedData *data = (SharedData *)shmat(shmID, NULL, 0);
    if (data == (void *)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }

    data->BankAccount = 0;
    data->localBalance = 0;

    // Create semaphore
    int semID = createSemaphore(IPC_PRIVATE, 1);  // Initialize semaphore to 1 for mutual exclusion

    // Fork parent (Dear Old Dad) and child (Poor Student) processes
    pid_t pid;
    for (int i = 0; i < numDads; i++) {
        pid = fork();
        if (pid == 0) {
            parentProcess(data, semID);
            exit(0);
        }
    }

    for (int i = 0; i < numStudents; i++) {
        pid = fork();
        if (pid == 0) {
            childProcess(data, semID);
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < numDads + numStudents; i++) {
        wait(NULL);
    }

    // Clean up
    shmdt(data);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(semID, 0, IPC_RMID);

    return 0;
}
