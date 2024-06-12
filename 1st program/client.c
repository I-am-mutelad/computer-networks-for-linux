#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t recv_size;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Connect
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("\n\n##---Connected to server---##\n");

    // Send data to server
    char *message = "Hello from client--->";
    send(client_socket, message, strlen(message), 0);
    printf("Message sent to server--->\n");

    // Receive response from server
    recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (recv_size > 0) {
        buffer[recv_size] = '\0'; // Null-terminate the received data
        printf("---> Response from server: %s\n", buffer);
    } else if (recv_size == 0) {
        printf("\n\nConnection closed by server\n");
    } else {
        perror("Receive failed");
    }

    // Close socket
    close(client_socket);

    return 0;
}
