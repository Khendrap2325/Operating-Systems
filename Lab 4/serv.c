#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "list.h"
#include <signal.h>
#include <unistd.h>

#define PORT 9001
#define ACK "ACK"

list_t *mylist;  

void signal_handler(int sig) {
    (void)sig;  // Mark sig as unused
    printf("\nServer shutting down..\n");
    if (mylist) {
        list_free(mylist);
        printf("Linked list memory freed.\n");
    }
    exit(0);
}

int main(int argc, char const* argv[]) {
    (void)argc;  // Mark argc as unused
    (void)argv;  // Mark argv as unused
    signal(SIGINT, signal_handler);

    int n, val, idx;
    char buf[1024];
    char sbuf[1024];
    char* token;

    // create server socket 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    if (servSockD < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 

    // bind socket to the specified IP and port 
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        close(servSockD);
        exit(1);
    }

    // listen for connections 
    listen(servSockD, 1); 

    // accept client connection
    int clientSocket = accept(servSockD, NULL, NULL); 
    if (clientSocket < 0) {
        perror("Client connection failed");
        close(servSockD);
        exit(1);
    }

    mylist = list_alloc();  // create the list

    while(1) {
        // Receive messages from client
        n = recv(clientSocket, buf, sizeof(buf), 0);
        buf[n] = '\0';  // Null-terminate the received string

        if (n > 0) {
            token = strtok(buf, " ");
            if (strcmp(token, "exit") == 0) {
                list_free(mylist);
                close(clientSocket);
                exit(0);
            } else if (strcmp(token, "get_length") == 0) {
                val = list_length(mylist);
                sprintf(sbuf, "Length = %d", val);
            } else if (strcmp(token, "add_front") == 0) {
                token = strtok(NULL, " ");  // get next token (value)
                if (!token) {
                    sprintf(sbuf, "Error: missing value for add_front");
                } else {
                    val = atoi(token);
                    list_add_to_front(mylist, val);
                    sprintf(sbuf,"%s%d", ACK, val); // Response
                }
            } else if (strcmp(token, "add_position") == 0) {
                token = strtok(NULL, " ");
                if (!token) {
                    sprintf(sbuf, "Error: missing index for add_position");
                } else {
                    idx = atoi(token);
                    token = strtok(NULL, " ");
                    if (!token) {
                        sprintf(sbuf, "Error: missing value for add_position");
                    } else {
                        val = atoi(token);
                        list_add_at_index(mylist, idx, val); // Add element at specific index
                        sprintf(sbuf, "%s %d", ACK, val);
                    }
                }
            } else if (strcmp(token, "remove_front") == 0) {
                val = list_remove_from_front(mylist);
                sprintf(sbuf, "Removed %d", val);
            } else if (strcmp(token, "remove_back") == 0) {
                val = list_remove_from_back(mylist);
                sprintf(sbuf, "Removed %d", val);
            } else if (strcmp(token, "remove_position") == 0) {
                token = strtok(NULL, " ");
                if (!token) {
                    sprintf(sbuf, "Error: Missing index for remove_position");
                } else {
                    idx = atoi(token);
                    val = list_remove_at_index(mylist, idx);
                    sprintf(sbuf, "Removed %d", val);
                }
            } else if (strcmp(token, "get") == 0) {
                token = strtok(NULL, " ");
                if (!token) {
                    sprintf(sbuf, "Error: Missing index for get");
                } else {
                    idx = atoi(token);
                    val = list_get_elem_at(mylist, idx);
                    sprintf(sbuf, "Element at index %d = %d", idx, val);
                }
            } else if (strcmp(token, "print") == 0) {
                sprintf(sbuf, "%s", listToString(mylist));
            } else {
                sprintf(sbuf, "Error: Unknown command");
            }

            // Send response to client
            send(clientSocket, sbuf, strlen(sbuf), 0);
        }
        memset(buf, '\0', sizeof(buf));
        memset(sbuf, '\0', sizeof(sbuf));
    }

    close(clientSocket);
    close(servSockD);
    return 0; 
}
