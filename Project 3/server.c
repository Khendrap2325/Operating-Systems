#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BACKLOG 10

int chat_serv_sock_fd;
int numReaders = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rw_lock = PTHREAD_MUTEX_INITIALIZER;

char const *server_MOTD = "Thanks for connecting to the BisonChat Server.\n\nchat>";

struct node {
    char username[30];
    int socket;
    struct node *next;
};

struct room {
    char name[30];
    struct node *users;
    struct room *next;
};

struct node *head = NULL;
struct room *lobby = NULL;

void sigintHandler(int sig_num);
//struct node *insertFirstU(struct node *head, int socket, char *username);
struct node *findU(struct node *head, char *username);
struct room *create_room(char *room_name);
void add_user_to_room(struct room *room, char *username, int socket);
void list_users(struct node *head);
int get_server_socket();
int start_server(int serv_socket, int backlog);
int accept_client(int serv_sock);
void *client_receive(void *arg);

int main(int argc, char **argv) {
    signal(SIGINT, sigintHandler);

    lobby = create_room("Lobby");

    chat_serv_sock_fd = get_server_socket();
    if (start_server(chat_serv_sock_fd, BACKLOG) == -1) {
        printf("start server error\n");
        exit(1);
    }

    printf("Server Launched! Listening on PORT: %d\n", PORT);

    while (1) {
        int new_client = accept_client(chat_serv_sock_fd);
        if (new_client != -1) {
            pthread_t new_client_thread;
            pthread_create(&new_client_thread, NULL, client_receive, (void *)&new_client);
        }
    }

    close(chat_serv_sock_fd);
    return 0;
}

struct room *create_room(char *room_name) {
    struct room *new_room = (struct room *)malloc(sizeof(struct room));
    if (!new_room) {
        perror("Failed to allocate memory for new room");
        return NULL;
    }
    strncpy(new_room->name, room_name, sizeof(new_room->name) - 1);
    new_room->users = NULL;
    new_room->next = NULL;
    return new_room;
}

void add_user_to_room(struct room *room, char *username, int socket) {
    if (!room) return;
    struct node *new_user = (struct node *)malloc(sizeof(struct node));
    if (!new_user) return;

    strncpy(new_user->username, username, sizeof(new_user->username) - 1);
    new_user->socket = socket;
    new_user->next = room->users;
    room->users = new_user;
}

//struct node *insertFirstU(struct node *head, int socket, char *username) {
    /*struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (!new_node) return NULL;

    strncpy(new_node->username, username, sizeof(new_node->username) - 1);
    new_node->socket = socket;
    new_node->next = head;
    return new_node;*/
//}

void list_users(struct node *head) {
    struct node *current = head;
    printf("Active Users:\n");
    while (current != NULL) {
        printf("- %s\n", current->username);
        current = current->next;
    }
}

int get_server_socket() {
    int opt = 1, master_socket;
    struct sockaddr_in address;

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    return master_socket;
}

int start_server(int serv_socket, int backlog) {
    if (listen(serv_socket, backlog) == -1) {
        printf("socket listen error\n");
        return -1;
    }
    return 0;
}

int accept_client(int serv_sock) {
    int reply_sock_fd = -1;
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    struct sockaddr_storage client_addr;

    if ((reply_sock_fd = accept(serv_sock, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
        printf("socket accept error\n");
    }
    return reply_sock_fd;
}

/*void *client_receive(void *arg) {
    int socket = *(int *)arg;
    char guest_name[30];
    snprintf(guest_name, sizeof(guest_name), "guest%d", socket);

    pthread_mutex_lock(&rw_lock);
    head = insertFirstU(head, socket, guest_name);
    add_user_to_room(lobby, guest_name, socket);
    pthread_mutex_unlock(&rw_lock);

    send(socket, server_MOTD, strlen(server_MOTD), 0);
    pthread_exit(NULL);
}*/

void sigintHandler(int sig_num) {
    printf("\nGracefully shutting down the server...\n");

    pthread_mutex_lock(&rw_lock);
    struct node *current = head;
    while (current != NULL) {
        close(current->socket);
        struct node *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    pthread_mutex_unlock(&rw_lock);

    close(chat_serv_sock_fd);
    printf("Server shut down successfully.\n");
    exit(0);
}
