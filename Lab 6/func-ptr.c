#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "process.h"
#include "util.h"

#define DEBUG 0  // change this to 1 to enable verbose output

// Signature for a function pointer that can compare
// You need to cast the input into its actual type and then compare them according to your custom logic
typedef int (*Comparer)(const void *a, const void *b);

// Compare 2 processes
// You can assume:
// - Process ids will be unique
// - No 2 processes will have the same arrival time
int my_comparer(const void *this, const void *that)
{
    const Process *p1 = (const Process *)this;
    const Process *p2 = (const Process *)that;

    // Compare based on arrival time (ascending)
    if (p1->arrival_time != p2->arrival_time) {
        return p1->arrival_time - p2->arrival_time;
    }
    return p1->pid - p2->pid;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
        return 1;
    }

    /*******************/
    /* Parse the input */
    /*******************/
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        return 1;
    }

    // Assuming the parse_file function returns a pointer to an array of Process
    // and sets P_SIZE correctly
    Process *processes = parse_file(input_file);
    if (!processes) {
        fprintf(stderr, "Error: Failed to parse file\n");
        fclose(input_file);
        return 1;
    }

    /*******************/
    /* Sort the input  */
    /*******************/
    Comparer process_comparer = &my_comparer;

    if (DEBUG) {
        printf("Unsorted Processes:\n");
        for (int i = 0; i < P_SIZE; i++) {
            printf("%d (%d, %d)\n", processes[i].pid, processes[i].priority, processes[i].arrival_time);
        }
        printf("\n");
    }

    qsort(processes, P_SIZE, sizeof(Process), process_comparer);

    /**************************/
    /* Print the sorted data  */
    /**************************/
    printf("Sorted Processes:\n");
    for (int i = 0; i < P_SIZE; i++) {
        printf("%d (%d, %d)\n", processes[i].pid, processes[i].priority, processes[i].arrival_time);
    }

    /************/
    /* Clean up */
    /************/
    free(processes);
    fclose(input_file);
    return 0;
}
