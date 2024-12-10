#include <stdio.h>
#include <stdlib.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b != 0) {
        return a / b;
    } else {
        printf("Error: Division by zero.\n");
        return 0; // Handle division by zero
    }
}

void exit_program() {
    printf("Exiting program.\n");
    exit(0);  // Exit the program
}

int main() {
    int (*operations[])(int, int) = {add, subtract, multiply, divide};
    int choice, a, b;

    while (1) {
        printf("Choose an operation:\n");
        printf("0: Add\n");
        printf("1: Subtract\n");
        printf("2: Multiply\n");
        printf("3: Divide\n");
        printf("4: Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            exit_program();  // Exit the program
        }

        printf("Enter two numbers: ");
        scanf("%d %d", &a, &b);

        if (choice >= 0 && choice <= 3) {
            printf("Result: %d\n", operations[choice](a, b));
        } else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
