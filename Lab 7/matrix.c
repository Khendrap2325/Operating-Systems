#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int **matA;
int **matB;

int **matSumResult;
int **matDiffResult;
int **matProductResult;

int MAX;

// Structure to pass row and column information to threads
typedef struct {
    int row;
    int col;
} ThreadArgs;

void fillMatrix(int **matrix) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1; // Fill with random values between 1 and 10
        }
    }
}

void printMatrix(int **matrix) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to compute sum of elements at [row][col]
void* computeSum(void* args) {
    ThreadArgs* data = (ThreadArgs*)args;
    int row = data->row;
    int col = data->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(args); // Free the allocated memory for arguments
    pthread_exit(NULL);
}

// Function to compute difference of elements at [row][col]
void* computeDiff(void* args) {
    ThreadArgs* data = (ThreadArgs*)args;
    int row = data->row;
    int col = data->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(args); // Free the allocated memory for arguments
    pthread_exit(NULL);
}

// Function to compute the product (inner product) of row and column
void* computeProduct(void* args) {
    ThreadArgs* data = (ThreadArgs*)args;
    int row = data->row;
    int col = data->col;
    matProductResult[row][col] = 0;

    for (int k = 0; k < MAX; k++) {
        matProductResult[row][col] += matA[row][k] * matB[k][col]; // Dot product calculation
    }

    free(args); // Free the allocated memory for arguments
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <matrix_size>\n", argv[0]);
        return 1;
    }
    MAX = atoi(argv[1]); // Assign the matrix size from the command line argument

    // Dynamically allocate memory for matrices
    matA = (int**)malloc(MAX * sizeof(int*));
    matB = (int**)malloc(MAX * sizeof(int*));
    matSumResult = (int**)malloc(MAX * sizeof(int*));
    matDiffResult = (int**)malloc(MAX * sizeof(int*));
    matProductResult = (int**)malloc(MAX * sizeof(int*));

    for (int i = 0; i < MAX; i++) {
        matA[i] = (int*)malloc(MAX * sizeof(int));
        matB[i] = (int*)malloc(MAX * sizeof(int));
        matSumResult[i] = (int*)malloc(MAX * sizeof(int));
        matDiffResult[i] = (int*)malloc(MAX * sizeof(int));
        matProductResult[i] = (int*)malloc(MAX * sizeof(int));
    }

    srand(time(0));  // Initialize random number generator

    // 1. Fill the matrices (matA and matB) with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // 3. Create pthread_t objects for our threads
    pthread_t threads[MAX * MAX];

    // 4. Create a thread for each cell in each result matrix (for Sum, Diff, Product)
    int thread_count = 0;

    // Matrix Sum calculation using threads
    printf("Calculating matrix sum...\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&threads[thread_count], NULL, computeSum, (void*)args);
            thread_count++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Reset thread_count for next operation
    thread_count = 0;

    // Matrix Difference calculation using threads
    printf("Calculating matrix difference...\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&threads[thread_count], NULL, computeDiff, (void*)args);
            thread_count++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Reset thread_count for next operation
    thread_count = 0;

    // Matrix Product calculation using threads
    printf("Calculating matrix product...\n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&threads[thread_count], NULL, computeProduct, (void*)args);
            thread_count++;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // 5. Print the results
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);

    printf("Difference:\n");
    printMatrix(matDiffResult);

    printf("Product:\n");
    printMatrix(matProductResult);

    // Free allocated memory
    for (int i = 0; i < MAX; i++) {
        free(matA[i]);
        free(matB[i]);
        free(matSumResult[i]);
        free(matDiffResult[i]);
        free(matProductResult[i]);
    }
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);

    return 0;
}
