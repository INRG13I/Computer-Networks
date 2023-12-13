#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    char* server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int client_socket;
    struct sockaddr_in server_addr;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to the server failed");
        close(client_socket);
        exit(1);
    }

    char message[1024];
    printf("Enter a string: ");
    scanf("%99[^\n]", message);

    send(client_socket, message, strlen(message), 0);

    int space_count;
    if (recv(client_socket, &space_count, sizeof(space_count), 0) > 0) {
        printf("Number of spaces in the string: %d\n", space_count);
    }

    close(client_socket);

    return 0;
}
