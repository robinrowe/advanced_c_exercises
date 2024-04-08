#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define MAX_CLIENTS 5

// Structure to hold client information
typedef struct
{
  SOCKET socket;
  struct sockaddr_in address;
  int addr_len;
  int id;
} Client;

// Global variables for bid information
Client clients[MAX_CLIENTS];
HANDLE timer;
SOCKET server_socket;
int best_bid = 0;
int winning_client = 0;

VOID APIENTRY TimerCompletionRoutine(PVOID lpParam, DWORD TimerOrWaitFired, DWORD ignored);
unsigned __stdcall client_handler(void* data);

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

  // Create server socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Socket creation failed.\n");
    return 1;
  }

  // Prepare the sockaddr_in struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // Bind server socket
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
  {
    printf("Bind failed with error: %d\n", WSAGetLastError());
    return 1;
  }

  // Listen to server socket
  if (listen(server_socket, 5) == SOCKET_ERROR)
  {
    printf("Listen failed with error: %d\n", WSAGetLastError());
    return 1;
  }
  printf("Server listening on port %d\n", PORT);

  // Create a waitable timer
  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  if(!timer)
  {  puts("CreateWaitableTimer failed");
     return 2;
  }
  LARGE_INTEGER dueTime;
  dueTime.QuadPart = -200000000LL; // 20 seconds
  SetWaitableTimer(timer, &dueTime, 0, TimerCompletionRoutine, NULL, 0);

  while (1)
  {
    // Accept incoming connections and handle each client
    SOCKET client_socket;
    Client client;
    client.addr_len = sizeof(client.address);

    // Accept connection from client
    if ((client_socket = accept(server_socket, 
      (struct sockaddr*)&client.address, &client.addr_len)) == INVALID_SOCKET)
    {
      printf("Accept failed with error: %d\n", WSAGetLastError());
      continue;
    }

    // Add client to the clients array
    int c = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (clients[i].socket == 0)
      {
        clients[i].socket = client_socket;
        clients[i].address = client.address;
        clients[i].addr_len = client.addr_len;
        clients[i].id = i + 1;
        c = i;
        printf("Client no.%d connected.\n", i+1);
        break;
      }
    }

    // Handle client in a separate thread
    _beginthreadex(NULL, 0, client_handler, &clients[c], 0, NULL);
  }
  return 0;
}

// Callback function to handle client connection
unsigned __stdcall client_handler(void* data)
{
  Client* client = (Client*)data;
  SOCKET client_socket = client->socket;
  char buffer[1024] = { 0 };
  int bid_amount;

  while (1)
  {
    // Receive bid amount from client
    int valread = recv(client_socket, buffer, sizeof(buffer), 0);
    if (valread <= 0)
    {
      if (valread == 0)
        printf("Client disconnected.\n");
      else
        printf("Recv failed with error: %d\n", WSAGetLastError());

      break;
    }
    bid_amount = atoi(buffer);
    printf("Received bid $%d from client %d\n", bid_amount, client->id);

    // Update best bid if necessary
    if (bid_amount > best_bid)
    {
      best_bid = bid_amount;
      winning_client = client->id;

      // Inform all clients about the new best bid
      char msg[1024];
      sprintf(msg, "New best bid: $%d (Client: %d)", best_bid, winning_client);
      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (clients[i].socket != 0)
        {
          send(clients[i].socket, msg, strlen(msg), 0);
        }
      }

      // Restart the timer
      LARGE_INTEGER dueTime;
      dueTime.QuadPart = -200000000LL; // 10 seconds in 100-nanosecond intervals
      SetWaitableTimer(timer, &dueTime, 0, TimerCompletionRoutine, NULL, 0);
    }
    else
    {
      char msg[1024];
      sprintf(msg, "Received lower bid. Best bid remains at: $%d", best_bid);
      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (clients[i].socket != 0)
        {
          send(clients[i].socket, msg, strlen(msg), 0);
        }
      }
    }
  }
  return 0;
}

/*
VOID
(APIENTRY *PTIMERAPCROUTINE)(
    _In_opt_ LPVOID lpArgToCompletionRoutine,
    _In_     DWORD dwTimerLowValue,
    _In_     DWORD dwTimerHighValue
    );
*/
// Callback function to finish the auction after 20 seconds of inactivity
VOID APIENTRY TimerCompletionRoutine(LPVOID lpParam, DWORD TimerOrWaitFired, DWORD ignored)
{
  printf("Auction finished. Winning bid: $%d, winner: client no. %d\n", best_bid, winning_client);

  // Inform all clients about the end of the auction
  char msg[1024];
  sprintf(msg, "Auction finished. Winning bid: $%d, winner: client no. %d", 
    best_bid, winning_client);
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i].socket != 0)
    {
      send(clients[i].socket, msg, strlen(msg), 0);
    }
  }

  CloseHandle(timer);
  closesocket(server_socket);
  WSACleanup();

  exit(EXIT_SUCCESS);
}

