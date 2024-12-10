#include "server.h"

#define DEFAULT_ROOM "Lobby"

// USE THESE LOCKS AND COUNTER TO SYNCHRONIZE
extern int numReaders;
extern pthread_mutex_t rw_lock;
extern pthread_mutex_t mutex;

extern struct node *head;

extern char *server_MOTD;

// Helper function to trim whitespace
char *trimwhitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

// Thread for handling each client
void *client_receive(void *ptr) {
    int client = *(int *)ptr;
    int received, i;
    char buffer[MAXBUFF], sbuffer[MAXBUFF];
    char tmpbuf[MAXBUFF];
    char cmd[MAXBUFF], username[30];
    char *arguments[80];

    struct node *currentUser;

    send(client, server_MOTD, strlen(server_MOTD), 0);

    // Assign default guest username
    sprintf(username, "guest%d", client);
    pthread_mutex_lock(&rw_lock);
    head = insertFirstU(head, client, username);
    pthread_mutex_unlock(&rw_lock);

    // Add the GUEST to the default room (not implemented in the skeleton)
    // Example: Add logic to place a user in a default "Lobby" room.

    while (1) {
        if ((received = read(client, buffer, MAXBUFF)) > 0) {
            buffer[received] = '\0';
            strcpy(cmd, buffer);
            strcpy(sbuffer, buffer);

            arguments[0] = strtok(cmd, delimiters);
            i = 0;
            while (arguments[i] != NULL) {
                arguments[++i] = strtok(NULL, delimiters);
                if (arguments[i - 1])
                    strcpy(arguments[i - 1], trimwhitespace(arguments[i - 1]));
            }

            if (strcmp(arguments[0], "create") == 0) {
                printf("Creating room: %s\n", arguments[1]);
                // Logic to create room
                sprintf(buffer, "Room '%s' created.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "join") == 0) {
                printf("Joining room: %s\n", arguments[1]);
                // Logic to join room
                sprintf(buffer, "You joined '%s'.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "leave") == 0) {
                printf("Leaving room: %s\n", arguments[1]);
                // Logic to leave room
                sprintf(buffer, "You left '%s'.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "connect") == 0) {
                printf("Connecting to user: %s\n", arguments[1]);
                // Logic to connect to another user
                sprintf(buffer, "You are now connected to '%s'.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "disconnect") == 0) {
                printf("Disconnecting from user: %s\n", arguments[1]);
                // Logic to disconnect
                sprintf(buffer, "You disconnected from '%s'.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "rooms") == 0) {
                printf("Listing all rooms\n");
                // Logic to list all rooms
                sprintf(buffer, "Available rooms: Lobby, Room1, Room2.\nchat>");
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "users") == 0) {
                printf("Listing all users\n");
                pthread_mutex_lock(&rw_lock);
                currentUser = head;
                while (currentUser != NULL) {
                    strcat(buffer, currentUser->username);
                    strcat(buffer, "\n");
                    currentUser = currentUser->next;
                }
                pthread_mutex_unlock(&rw_lock);
                strcat(buffer, "chat>");
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "login") == 0) {
                printf("User logging in as: %s\n", arguments[1]);
                pthread_mutex_lock(&rw_lock);
                currentUser = findU(head, username);
                if (currentUser) strcpy(currentUser->username, arguments[1]);
                pthread_mutex_unlock(&rw_lock);
                sprintf(buffer, "Logged in as '%s'.\nchat>", arguments[1]);
                send(client, buffer, strlen(buffer), 0);
            } else if (strcmp(arguments[0], "exit") == 0 || strcmp(arguments[0], "logout") == 0) {
                printf("User exiting\n");
                pthread_mutex_lock(&rw_lock);
                // Logic to remove user and cleanup
                pthread_mutex_unlock(&rw_lock);
                close(client);
                break;
            } else if (strcmp(arguments[0], "help") == 0) {
                sprintf(buffer, "Commands:\n"
                                "login <username> - Login with username\n"
                                "create <room> - Create a room\n"
                                "join <room> - Join a room\n"
                                "leave <room> - Leave a room\n"
                                "users - List all users\n"
                                "rooms - List all rooms\n"
                                "connect <user> - Connect to user\n"
                                "exit - Exit chat\nchat>");
                send(client, buffer, strlen(buffer), 0);
            } else {
                sprintf(tmpbuf, "::%s> %s\nchat>", username, sbuffer);
                pthread_mutex_lock(&rw_lock);
                currentUser = head;
                while (currentUser != NULL) {
                    if (currentUser->socket != client)
                        send(currentUser->socket, tmpbuf, strlen(tmpbuf), 0);
                    currentUser = currentUser->next;
                }
                pthread_mutex_unlock(&rw_lock);
            }
        }
    }
    return NULL;
}