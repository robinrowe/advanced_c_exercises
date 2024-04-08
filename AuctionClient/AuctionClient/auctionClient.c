#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SERVER_IP "127.0.0.1"

SOCKET client_socket;
int is_running = 1;
HANDLE receive_thread;

unsigned __stdcall receive_handler(void* arg);

int main()
{
  WSADATA wsa;
  struct sockaddr_in server_addr;
  int addr_len = sizeof(server_addr);

  // Initialize Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("WSAStartup failed.\n");
    return 1;
  }

  // Create socket
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Socket creation failed.\n");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
  {
    perror("inet_pton");
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
  {
    printf("Connect failed.\n");
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  printf("Connected to server.\n");

  receive_thread = (HANDLE)_beginthreadex(NULL, 0, receive_handler, NULL, 0, NULL);
  if (receive_thread == NULL)
  {
    perror("_beginthreadex");
    closesocket(client_socket);
    WSACleanup();
    return 1;
  }

  char buffer[1024];
  while (is_running)
  {
    printf("\nEnter your bid (or 'q' to quit): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    if (strcmp(buffer, "q") == 0)
    {
      break;
    }

    if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
      printf("Send failed.\n");
      closesocket(client_socket);
      WSACleanup();
      return 1;
    }
  }

  return 0;
}

unsigned __stdcall receive_handler(void* arg)
{
  char buffer[1024] = { 0 };
  while (1)
  {
    int valread = recv(client_socket, buffer, sizeof(buffer), 0);
    if (valread <= 0)
    {
      if (valread == 0)
        printf("\nServer disconnected.\n");
      else
        perror("recv");

      break;
    }

    printf("\nServer: %s\n", buffer);
    if (strncmp(buffer, "Auction", 7) == 0)
    {
      printf("Auction ended. Exiting program.\n");

      CloseHandle(receive_thread);
      closesocket(client_socket);
      WSACleanup();

      exit(EXIT_SUCCESS);
    }

    memset(buffer, 0, sizeof(buffer));
    printf("\nEnter your bid (or 'q' to quit): ");
  }
  return 0;
}