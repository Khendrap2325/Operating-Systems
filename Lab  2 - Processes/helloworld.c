//helloWorld.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    //output Hello World!
    fork();
    fork();
    printf("Hello World! \n");
    return 0; // what does the 0 represent? is it just needed cuz it's a function
}