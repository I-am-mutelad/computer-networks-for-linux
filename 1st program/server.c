#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    ssize_t recv_size;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_socket, SOMAXCONN) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("\n\nServer listening on port %d....\n", PORT);

    // Accept connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("\n##---Connection accepted from %s:%d---##\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Receive data from client and echo back
    while ((recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[recv_size] = '\0'; // Null-terminate the received data
        printf("---> Received from client: %s\n", buffer);
        send(client_socket, buffer, recv_size, 0);
    }

    if (recv_size == 0) {
        printf("\n\nConnection closed by client\n");
    } else {
        perror("Receive failed");
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
