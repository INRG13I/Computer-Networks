#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *client_handler(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[1024];
    int recv_size;

    while ((recv_size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        int space_count = 0;
        for (int i = 0; i < recv_size; i++) {
            if (buffer[i] == ' ') {
                space_count++;
            }
        }
        send(client_socket, &space_count, sizeof(space_count), 0);
    }

    close(client_socket);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread;

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }

    // Listen for clients
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        close(server_socket);  // Close the socket before exiting
        exit(1);
    }

    printf("Server is listening on port 8888...\n");

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        int *new_client = malloc(sizeof(int));
        *new_client = client_socket;

        if (pthread_create(&thread, NULL, client_handler, (void *)new_client) < 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    close(server_socket);
    return 0;
}
