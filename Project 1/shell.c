#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

void sigint_handler(int sig) {
    printf("\nCaught SIGINT. Use 'exit' to quit the shell.\n");
}

void sigalarm_handler(int sig) {
    printf("Process terminate because of timeout.\n");
}

int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    //Handle response to Ctrl-C 
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);

    while (true) {
      
            // Print the shell prompt.
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL){
                printf("%s> ", cwd);
            }else {
                perror("getcwd() error");
            }
            fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
        
        while(command_line[0] == 0x0A);  // while just ENTER pressed
        command_line[strlen(command_line) - 1] = '\0';
      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  
			
        // 1. Tokenize the command line input (split it on whitespace)
        int argc = 0;
        arguments[argc] = strtok(command_line, delimiters);
        while (arguments[argc] != NULL) {
            argc++;
            arguments[argc] = strtok(NULL, delimiters);
        }

      
        // 2. Implement Built-In Commands
        if (strcmp(arguments[0], "cd")==0){
            if (chdir(arguments[1])!=0){
                perror("cd failed");
            }
            continue;
        } else if (strcmp(arguments[0], "pwd")==0){
            printf("%s\n", cwd);
            continue;
        }else if (strcmp(arguments[0],"echo")==0){
            int i;
            for (i = 1; i < argc; i++){
                printf("%s ", arguments[i]);
            }
            printf("\n");
            continue;
        }else if (strcmp(arguments[0], "exit") ==0){
            printf("Exiting shell...\n");
            exit(0);
        }else if (strcmp(arguments[0], "env") ==0){
            char **env;
            for (env = environ; *env !=0; env++){
                printf("%s\n", *env);
            }
            continue;
        }else if (strcmp(arguments[0], "setenv")==0){
            if (setenv(arguments[1], arguments[2]) !=0){
                perror("setenv failed");
            }
            continue;
        }
        
        //Implement fork
        int bkg =0;
        if (strcmp(arguments[argc-1], "&") == 0){
            bkg = 1;
            arguments[argc-1]=NULL; 
        }

        pid_t pid = fork();
        if (pid ==0){
        // 3. Create a child process which will execute the command line input
            if (execvp(arguments[0], arguments) ==-1){
                perror("execvp failed");
            }
            exit(EXIT_FAILURE);
        }else if (pid <0){
            //if pid negative, forking unsuccessful
            perror("fork failed");
        }else{
        // 4. The parent process should wait for the child to complete unless its a background process
        if (!bkg){
            wait(NULL);
        }
        }
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}