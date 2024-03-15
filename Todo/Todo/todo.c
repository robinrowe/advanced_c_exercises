#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task 
{
  char description[100];
  int priority;
  struct Task* next;
} Task;

typedef struct LinkedList 
{
  Task* head;
  int size;
} LinkedList;

void init_tasks(LinkedList* list);
void add_task(LinkedList* list, const char* description, int priority);
void remove_task(LinkedList* list, int index);
void display_tasks(LinkedList* list);
void sort_tasks(LinkedList* list);

int main() 
{
  LinkedList list;
  init_tasks(&list);

  int choice;
  char description[100];
  int priority;
  int index;

  do 
  {
    printf("\nTo-Do List Manager\n");
    printf("1. Add Task\n");
    printf("2. Remove Task\n");
    printf("3. Display Tasks\n");
    printf("4. Sort Tasks by Priority\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) 
    {
    case 1:
      printf("Enter task description: ");
      getchar(); // Clear input buffer
      fgets(description, sizeof(description), stdin);
      description[strcspn(description, "\n")] = '\0'; // Remove trailing newline
      printf("Enter priority: ");
      scanf("%d", &priority);
      add_task(&list, description, priority);
      printf("Task added successfully.\n");
      break;
    case 2:
      printf("Enter number of task to remove: ");
      scanf("%d", &index);
      remove_task(&list, index-1);
      printf("Task removed successfully.\n");
      break;
    case 3:
      printf("List of tasks:\n");
      display_tasks(&list);
      break;
    case 4:
      sort_tasks(&list);
      printf("Tasks sorted by priority.\n");
      break;
    case 5:
      printf("Exiting...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 0);

  return 0;
}

void init_tasks(LinkedList* list)
{
  list->head = NULL;
  list->size = 0;
}

void add_task(LinkedList* list, const char* description, int priority) 
{
  Task* task = (Task*)malloc(sizeof(Task));
  if (!task) 
  {
    printf("Memory allocation failed.\n");
    return;
  }
  strcpy(task->description, description);
  task->priority = priority;
  task->next = NULL;

  if (list->head == NULL) 
  {
    // List is empty
    list->head = task;
  }
  else 
  {
    Task* temp = list->head;
    // Find the last node
    while (temp->next != NULL) 
    {
      temp = temp->next;
    }
    // Insert the new task after the last node
    temp->next = task;
  }
  list->size++;
}

void remove_task(LinkedList* list, int index) 
{
  if (list->head == NULL) 
  {
    printf("List is empty.\n");
    return;
  }

  if (index == 0) 
  {
    // If we remove the first item in the list
    Task* temp = list->head;
    list->head = list->head->next;
    free(temp);
    list->size--;
    return;
  }

  Task* previous = NULL;
  Task* current = list->head;
  int i = 0;
  // Go to the selected index
  while (current != NULL && i < index) 
  {
    previous = current;
    current = current->next;
    i++;
  }

  if (current == NULL) 
  {
    printf("Index out of bounds.\n");
    return;
  }

  previous->next = current->next;
  free(current);
  list->size--;
}

void display_tasks(LinkedList* list) 
{
  Task* temp = list->head;
  int i = 1;
  while (temp != NULL) 
  {
    printf("%d) Description: %s, Priority: %d\n", 
      i++, temp->description, temp->priority);
    temp = temp->next;
  }
}

void sort_tasks(LinkedList* list) 
{
  int swapped;
  Task* ptr1;
  Task* ptr2 = NULL;

  if (list->head == NULL)
    return;

  do 
  {
    swapped = 0; // will change if swapping happens
    ptr1 = list->head;

    while (ptr1->next != ptr2)
    {
      if (ptr1->priority > ptr1->next->priority) 
      {
        // Swap data of adjacent nodes
        int tempPriority = ptr1->priority;
        ptr1->priority = ptr1->next->priority;
        ptr1->next->priority = tempPriority;

        char tempDescription[100];
        strcpy(tempDescription, ptr1->description);
        strcpy(ptr1->description, ptr1->next->description);
        strcpy(ptr1->next->description, tempDescription);

        swapped = 1; // swap happened in this loop pass; don't stop yet
      }
      ptr1 = ptr1->next;
    }
    ptr2 = ptr1;
  } while (swapped); // quit loop when no swap happened
}
