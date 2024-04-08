#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50

typedef struct Node
{
  char name[MAX_NAME_LENGTH];
  struct Node* next;
} Node;

typedef struct
{
  Node* head;
} LinkedList;

typedef struct
{
  char name[MAX_NAME_LENGTH];
  LinkedList friends;
} UserNode;

UserNode users[MAX_USERS];
int num_users = 0;

typedef struct QueueNode
{
  int user_index;
  struct QueueNode* next;
} QueueNode;

typedef struct
{
  QueueNode* front;
  QueueNode* rear;
} Queue;

Queue* queue_create()
{
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  if(!queue)
  {  return 0;
  }
  queue->front = queue->rear = NULL;
  return queue;
}

int queue_is_empty(Queue* queue)
{
  return (queue->front == NULL);
}

void queue_enqueue(Queue* queue, int user_index)
{
  QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
  if(!newNode)
  {  exit(1);
  }
  newNode->user_index = user_index;
  newNode->next = NULL;

  if (queue_is_empty(queue))
  {
    queue->front = queue->rear = newNode;
  }
  else
  {
    queue->rear->next = newNode;
    queue->rear = newNode;
  }
}

int queue_dequeue(Queue* queue)
{
  if (queue_is_empty(queue))
  {
    printf("Queue is empty!\n");
    return -1;
  }

  QueueNode* temp = queue->front;
  int user_index = temp->user_index;
  queue->front = queue->front->next;

  if (queue->front == NULL)
  {
    queue->rear = NULL;
  }

  free(temp);
  return user_index;
}

void graph_add_user(char name[])
{
  if (num_users >= MAX_USERS)
  {
    printf("Max user limit reached!\n");
    return;
  }

  strcpy(users[num_users].name, name);
  users[num_users].friends.head = NULL;
  num_users++;
}

void graph_add_connection(int src, int dest)
{
  if (src < 0 || src >= num_users || dest < 0 || dest >= num_users)
  {
    printf("Invalid user index!\n");
    return;
  }

  Node* new_node_src = (Node*)malloc(sizeof(Node));
  if(!new_node_src)
  {  exit(1);
  }
  strcpy(new_node_src->name, users[dest].name);
  new_node_src->next = users[src].friends.head;
  users[src].friends.head = new_node_src;

  Node* new_node_dest = (Node*)malloc(sizeof(Node));
  if(!new_node_dest)
  {  exit(1);
  }
  strcpy(new_node_dest->name, users[src].name);
  new_node_dest->next = users[dest].friends.head;
  users[dest].friends.head = new_node_dest;
}

void graph_recommend_friends(int user_index)
{
  printf("Recommended friends for %s:\n", users[user_index].name);

  Queue* queue = queue_create();
  int visited[MAX_USERS] = { 0 };

  visited[user_index] = 1;
  queue_enqueue(queue, user_index);

  while (!queue_is_empty(queue))
  {
    int current_user_index = queue_dequeue(queue);
    Node* current = users[current_user_index].friends.head;

    while (current != NULL)
    {
      int friend_index = -1;
      for (int i = 0; i < num_users; i++)
      {
        if (strcmp(current->name, users[i].name) == 0)
        {
          friend_index = i;
          break;
        }
      }

      if (friend_index != -1 && !visited[friend_index])
      {
        printf("- %s\n", current->name);
        visited[friend_index] = 1;
        queue_enqueue(queue, friend_index);
      }

      current = current->next;
    }
  }

  free(queue);
}

int main()
{
  graph_add_user("User A");
  graph_add_user("User B");
  graph_add_user("User C");
  graph_add_user("User D");
  graph_add_user("User E");
  graph_add_user("User F");
  graph_add_user("User G");
  graph_add_user("User H");

  graph_add_connection(0, 1);
  graph_add_connection(1, 2);
  graph_add_connection(2, 3);
  graph_add_connection(4, 5);
  graph_add_connection(5, 7);
  graph_add_connection(3, 6);

  graph_recommend_friends(0);
  graph_recommend_friends(1);
  graph_recommend_friends(7);

  return 0;
}
