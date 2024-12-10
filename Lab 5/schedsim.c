#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <stdbool.h>

// Function to find the waiting time for all processes using Round Robin
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) {
    int rem_bt[n];
    for (int i = 0; i < n; i++) 
        rem_bt[i] = plist[i].bt; // Initialize remaining burst time array

    int t = 0; // Current time

    while (1) {
        bool done = true;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = false; // Process is still pending

                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt;
                    rem_bt[i] = 0;
                }
            }
        }

        if (done) 
            break;
    }
}

// Function to find the waiting time for all processes using SJF
void findWaitingTimeSJF(ProcessType plist[], int n) {
    int rem_bt[n], completed = 0, t = 0, min_bt = INT_MAX, shortest = 0;
    bool found = false;

    for (int i = 0; i < n; i++) 
        rem_bt[i] = plist[i].bt; // Initialize remaining burst time array

    while (completed != n) {
        found = false;
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= t && rem_bt[i] > 0 && rem_bt[i] < min_bt) {
                shortest = i;
                min_bt = rem_bt[i];
                found = true;
            }
        }

        if (!found) {
            t++;
            continue;
        }

        rem_bt[shortest]--;
        min_bt = rem_bt[shortest];

        if (rem_bt[shortest] == 0) {
            completed++;
            min_bt = INT_MAX;
            int finish_time = t + 1;
            plist[shortest].wt = finish_time - plist[shortest].bt - plist[shortest].art;
        }

        t++;
    }
}

// Function to find the waiting time for all processes using FCFS
void findWaitingTime(ProcessType plist[], int n) { 
    plist[0].wt = 0 + plist[0].art; 

    for (int i = 1; i < n; i++) 
        plist[i].wt = plist[i-1].bt + plist[i-1].wt; 
}

// Function to calculate turn around time
void findTurnAroundTime(ProcessType plist[], int n) { 
    for (int i = 0; i < n; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
}

// Function to sort the processes by priority
int my_comparer(const void *this, const void *that) {
    ProcessType *p1 = (ProcessType *)this;
    ProcessType *p2 = (ProcessType *)that;
    return p1->pri - p2->pri; // Sort by priority (ascending)
}

// Function to calculate average time for FCFS
void findavgTimeFCFS(ProcessType plist[], int n) {
    findWaitingTime(plist, n);
    findTurnAroundTime(plist, n);
    printf("\n*********\nFCFS\n");
}

// Function to calculate average time for SJF
void findavgTimeSJF(ProcessType plist[], int n) {
    findWaitingTimeSJF(plist, n);
    findTurnAroundTime(plist, n);
    printf("\n*********\nSJF\n");
}

// Function to calculate average time for Priority scheduling
void findavgTimePriority(ProcessType plist[], int n) {
    qsort(plist, n, sizeof(ProcessType), my_comparer); // Sort processes by priority
    findWaitingTime(plist, n); // Apply FCFS after sorting
    findTurnAroundTime(plist, n);
    printf("\n*********\nPriority\n");
}

// Function to calculate average time for Round Robin
void findavgTimeRR(ProcessType plist[], int n, int quantum) {
    findWaitingTimeRR(plist, n, quantum);
    findTurnAroundTime(plist, n);
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

// Function to print metrics
void printMetrics(ProcessType plist[], int n) {
    int total_wt = 0, total_tat = 0;
    float awt, att;

    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n");

    for (int i = 0; i < n; i++) {
        total_wt = total_wt + plist[i].wt;
        total_tat = total_tat + plist[i].tat;
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat);
    }

    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);

    printf("\nAverage waiting time = %.2f", awt);
    printf("\nAverage turn around time = %.2f\n", att);
}

// Function to initialize process list from file
ProcessType *initProc(char *filename, int *n) {
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);

    fclose(input_file);
    return plist;
}

// Driver code
int main(int argc, char *argv[]) {
    int n;
    int quantum = 2;

    ProcessType *proc_list;

    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeFCFS(proc_list, n);
    printMetrics(proc_list, n);

    // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeSJF(proc_list, n);
    printMetrics(proc_list, n);

    // Priority
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimePriority(proc_list, n);
    printMetrics(proc_list, n);

    // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    findavgTimeRR(proc_list, n, quantum);
    printMetrics(proc_list, n);

    return 0;
}