#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 

int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 

    int fd1[2];
    int fd2[2];  // Used to store two ends of the second pipe 

    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 

    // Create the pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1; 
    }

    // Create a child process
    p = fork(); 
    if (p < 0) { 
        fprintf(stderr, "fork Failed");
        return 1; 
    } 

    // Parent process
    else if (p > 0) { 
        if (p < 0) { 
        fprintf(stderr, "fork Failed");
        return 1; 
    } 

    // Parent process
    else if (p > 0) { 
        printf("Enter a string to concatenate: ");
        scanf("%s", input_str); 
         // Wait for the child to finish
        wait(NULL);

        // Read concatenated string from the second pipe
        char output_str[100];
        read(fd2[0], output_str, 100); 

        // Close the reading end of the second pipe
        close(fd2[0]);

        // Print the concatenated result
        printf("Concatenated string: %s\n", output_str);
    } 

    // Child process
    else { 
        close(fd1[1]);  // Close writing end of the first pipe 
        close(fd2[0]);  // Close reading end of the second pipe 

        // Read the string from the first pipe
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 

        // Concatenate the fixed string with the read string
        int k = strlen(concat_str); 
        int i; 
        for (i = 0; i < strlen(fixed_str); i++) {
            concat_str[k++] = fixed_str[i]; 
        }

        concat_str[k] = '\0';  // Ensure the string ends with '\0'

        // Print the concatenated string
        printf("Child process: Concatenated string: %s\n", concat_str);
         // Write the concatenated string to the second pipe
        write(fd2[1], concat_str, strlen(concat_str) + 1);

        // Close both pipes
        close(fd1[0]); 
        close(fd2[1]); 

        exit(0); 
    } 
}
}