#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define MAX_REQUEST_SIZE 1024
#define CACHE_SIZE 3

#pragma comment(lib, "ws2_32.lib")

// Node structure for doubly linked list
typedef struct Node
{
  char* url;
  char* content;
  struct Node* prev;
  struct Node* next;
} Node;

// Cache structure
typedef struct LRUCache
{
  int capacity;
  int size;
  Node* head;
  Node* tail;
} LRUCache;

LRUCache* cache;

Node* createNode(char* url, char* content);
void deleteNode(Node* node);
void insertAtHead(Node* node);
void moveToHead(Node* node);
char* getContent(char* url);
void putContent(char* url, char* content);
void handleRequest(SOCKET clientSocket);

int main()
{
  WSADATA wsaData;
  SOCKET listenSocket, clientSocket;
  struct sockaddr_in serverAddr, clientAddr;
  int addrLen = sizeof(struct sockaddr_in);

  // Initialize Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    perror("WSAStartup");
    return 1;
  }

  // Create socket
  if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    perror("socket");
    return 1;
  }

  // Initialize server address
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  // Bind socket to address
  if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
  {
    perror("bind");
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  // Listen for connections
  if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
    perror("listen");
    closesocket(listenSocket);
    WSACleanup();
    return 1;
  }

  // Initialize cache
  cache = (LRUCache*)malloc(sizeof(LRUCache));
  if(!cache)
  {  puts("malloc failed");
     return 2;
  }
  cache->capacity = CACHE_SIZE;
  cache->size = 0;
  cache->head = NULL;
  cache->tail = NULL;

  printf("Server started on port %d...\n", PORT);

  while (1)
  {
    // Accept connections
    if ((clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen)) == INVALID_SOCKET)
    {
      perror("accept");
      continue;
    }

    printf("Connection from %s\n", inet_ntoa(clientAddr.sin_addr));

    // Handle client request
    handleRequest(clientSocket);

    // Close client socket
    closesocket(clientSocket);
  }

  // Free cache memory
  Node* current = cache->head;
  while (current != NULL)
  {
    Node* temp = current;
    current = current->next;
    deleteNode(temp);
  }
  free(cache);

  // Close server socket
  closesocket(listenSocket);

  // Cleanup Winsock
  WSACleanup();

  return 0;
}

void handleRequest(SOCKET clientSocket)
{
  char request[MAX_REQUEST_SIZE];
  int bytesReceived = recv(clientSocket, request, sizeof(request)-1, 0);
  if (bytesReceived <= 0)
  {
    perror("recv");
    return;
  }
  request[bytesReceived] = '\0';

  char* url = strtok(request, " ");
  if (strcmp(url, "GET") != 0)
  {
    perror("Only GET requests are supported.");
    return;
  }

  url = strtok(NULL, " ");
  if (url == NULL)
  {
    perror("Invalid request format.");
    return;
  }

  char* content = getContent(url);
  if (content == NULL)
  {
    // Serve the page from disk
    FILE* file = fopen(url + 1, "r");
    if (file == NULL)
    {
      // File not found, return 404 response
      char response[] = "HTTP/1.1 404 Not Found\n\n";
      printf("File not found: %s\n", url + 1);
      send(clientSocket, response, strlen(response), 0);
      return;
    }

    char response[MAX_REQUEST_SIZE];
    snprintf(response, sizeof(response), 
      "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    send(clientSocket, response, strlen(response), 0);

    char buffer[MAX_REQUEST_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
      send(clientSocket, buffer, strlen(buffer), 0);
    }
    fclose(file);
    printf("Got content from file: %s\n", buffer);

    // Cache the page content
    putContent(url, buffer);
  }
  else
  {
    // Serve the page from cache
    char response[MAX_REQUEST_SIZE];
    snprintf(response, sizeof(response), 
      "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    send(clientSocket, response, strlen(response), 0);

    send(clientSocket, content, strlen(content), 0);
  }
}

// Get the content associated with a URL from the cache
char* getContent(char* url)
{
  Node* current = cache->head;
  while (current != NULL)
  {
    if (strcmp(current->url, url) == 0)
    {
      moveToHead(current);
      printf("Got content from cache: %s\n", current->content);

      return current->content;
    }
    current = current->next;
  }
  // Return NULL if the URL is not found in cache
  return NULL;
}

// Put a URL-content pair into the cache
void putContent(char* url, char* content)
{
  if (cache->size == CACHE_SIZE)
  {
    deleteNode(cache->tail);
    cache->size--;
  }
  Node* newNode = createNode(url, content);
  insertAtHead(newNode);
  cache->size++;
}

// Create a new node
Node* createNode(char* url, char* content)
{
  Node* newNode = (Node*)malloc(sizeof(Node));
  if(!newNode)
  {  return 0;
  }
  newNode->url = _strdup(url);
  newNode->content = _strdup(content);
  newNode->prev = NULL;
  newNode->next = NULL;
  printf("New node created: %s\n", content);
  return newNode;
}

// Insert a new node at the head of the cache
void insertAtHead(Node* node)
{
  node->next = cache->head;
  node->prev = NULL;
  if (cache->head != NULL)
  {
    cache->head->prev = node;
  }
  cache->head = node;
  if (cache->tail == NULL)
  {
    cache->tail = node;
  }
  printf("Node inserted at head: %s\n", node->content);
}

// Move a node to the head of the cache
void moveToHead(Node* node)
{
  if (node == cache->head)
  {
    // Node is already at the head, no need to move
    return;
  }

  // Adjust pointers to remove node from its current position
  if (node->prev != NULL)
  {
    node->prev->next = node->next;
  }
  if (node->next != NULL)
  {
    node->next->prev = node->prev;
  }

  // Update pointers to insert node at the head
  node->prev = NULL;
  node->next = cache->head;
  if (cache->head != NULL)
  {
    cache->head->prev = node;
  }
  cache->head = node;
  if (cache->tail == NULL)
  {
    cache->tail = node;
  }
  printf("Node moved to head: %s\n", node->content);
}

// Delete a node from the cache
void deleteNode(Node* node)
{
  if (node == NULL)
    return;

  // If the node is the head of the list
  if (node == cache->head)
  {
    cache->head = node->next;
  }

  // If the node is the tail of the list
  if (node == cache->tail)
  {
    cache->tail = node->prev;
  }

  // Adjust pointers of neighboring nodes
  if (node->prev != NULL)
  {
    node->prev->next = node->next;
  }
  if (node->next != NULL)
  {
    node->next->prev = node->prev;
  }
  printf("Node deleted: %s\n", node->content);

  free(node->url);
  free(node->content);
  free(node);
}
