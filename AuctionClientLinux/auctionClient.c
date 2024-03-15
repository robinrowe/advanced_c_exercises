#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int client_socket;

void *receive_handler(void *arg);

int main()
{
  struct sockaddr_in server_addr;
  pthread_t receive_thread;

  // Create client socket
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
  {
    perror("Invalid address");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  // Connect to server
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Connect failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server.\n");

  // Create thread to handle receiving messages from server
  if (pthread_create(&receive_thread, NULL, receive_handler, NULL) != 0)
  {
    perror("Thread creation failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  // Main thread to send messages to the server
  char buffer[1024];
  while (1)
  {
    printf("Enter your bid (or 'q' to quit): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    if (strcmp(buffer, "q") == 0)
    {
      break;
    }

    if (send(client_socket, buffer, strlen(buffer), 0) == -1)
    {
      perror("Send failed");
      close(client_socket);
      exit(EXIT_FAILURE);
    }
  }

  // Wait for the receive thread to finish
  // pthread_join(receive_thread, NULL);

  // close(client_socket);

  return 0;
}

void *receive_handler(void *arg)
{
  char buffer[1024] = {0};

  while (1)
  {
    // Receive message from server
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1)
    {
      perror("Receive failed");
      break;
    }
    else if (bytes_received == 0)
    {
      printf("Server disconnected.\n");
      break;
    }

    buffer[bytes_received] = '\0';
    printf("\nServer: %s\n", buffer);

    if (strncmp(buffer, "Auction", 7) == 0)
    {
      printf("Auction ended. Exiting program.\n");

      close(client_socket);

      exit(0);
    }
    memset(buffer, 0, sizeof(buffer));
    printf("\nEnter your bid (or 'q' to quit): ");

  }

  close(client_socket);
  pthread_exit(NULL);
}
