#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum PackageType { Basic, Advanced, Overnight };

typedef struct Package 
{
  char sender_address[100];
  char delivery_address[100];
  float weight;
  char send_date[20];
  enum PackageType package_type;
  float cost;
  struct Package* next;
} Package;

// head of the linked list
Package* head = NULL;

float calculate_cost(struct Package* package);
void load_packages();
void save_packages();
void add_package();
void remove_package();
void view_packages();

int main() 
{
  load_packages();

  int choice;
  do 
  {
    printf("\n1. Add Package\n2. Remove Package\n3. View Packages\n4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) 
    {
    case 1:
      add_package();
      break;
    case 2:
      remove_package();
      break;
    case 3:
      view_packages();
      break;
    case 4:
      printf("Exiting...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 4);

  save_packages();
  return 0;
}

// calculate the cost of a package based on its type and weight
float calculate_cost(struct Package *package)
{
  if (package->package_type == Basic)
  {
    return 2* package->weight;
  }
  else if (package->package_type == Advanced)
  {
    return 3 * package->weight + 2;
  }
  else if (package->package_type == Overnight)
  {
    return 5 * package->weight + 6;
  }
  else
  {
    printf("Error package type! Exiting...");
    exit(1);
  }
}

void load_packages() 
{
  FILE* file = fopen("packages.txt", "r");
  if (file == NULL) 
  {
    perror("Packages file not found");
    return;
  }

  char sender_address[100];
  char delivery_address[100];
  float weight;
  char send_date[20];
  enum PackageType package_type;
  float cost;

  while (fgets(sender_address, sizeof(sender_address), file) != NULL) 
  {
    sender_address[strcspn(sender_address, "\n")] = '\0';
    fgets(delivery_address, sizeof(delivery_address), file);
    delivery_address[strcspn(delivery_address, "\n")] = '\0';
    fscanf(file, "%f\n", &weight);
    fgets(send_date, sizeof(send_date), file);
    send_date[strcspn(send_date, "\n")] = '\0';
    fscanf(file, "%d\n", &package_type);
    fscanf(file, "%f\n", &cost);

    // Create a new package node and add it to the linked list
    Package* new_package = (Package*)malloc(sizeof(Package));
    strcpy(new_package->sender_address, sender_address);
    strcpy(new_package->delivery_address, delivery_address);
    new_package->sender_address[strcspn(new_package->sender_address, "\n")] = '\0';
    new_package->weight = weight;
    strcpy(new_package->send_date, send_date);
    new_package->package_type = package_type;
    new_package->cost = cost;
    new_package->next = head;
    head = new_package;
  }

  fclose(file);
}

void save_packages() 
{
  FILE* file = fopen("packages.txt", "w");
  if (file == NULL) 
  {
    perror("Error opening file");
    exit(1);
  }

  Package* current = head;
  while (current != NULL) 
  {
    fprintf(file, "%s\n%s\n%.2f\n%s\n%d\n%.2f\n", current->sender_address, current->delivery_address,
      current->weight, current->send_date, current->package_type, current->cost);
    current = current->next;
  }

  fclose(file);
}

// add a package to the linked list
void add_package() 
{
  Package* new_package = (Package*)malloc(sizeof(Package));

  printf("Enter sender address: ");
  fgets(new_package->sender_address, sizeof(new_package->sender_address), stdin);
  new_package->sender_address[strcspn(new_package->sender_address, "\n")] = '\0';

  printf("Enter delivery address: ");
  fgets(new_package->delivery_address, sizeof(new_package->delivery_address), stdin);
  new_package->delivery_address[strcspn(new_package->delivery_address, "\n")] = '\0';

  printf("Enter weight: ");
  scanf("%f", &new_package->weight);
  getchar(); 

  printf("Enter send date: ");
  fgets(new_package->send_date, sizeof(new_package->send_date), stdin);
  new_package->send_date[strcspn(new_package->send_date, "\n")] = '\0';

  int option;
  printf("Enter package type (1: Basic, 2: Advanced, 3: Overnight): ");
  scanf("%d", &option);
  getchar();

  switch (option)
  {
  case 1:
    new_package->package_type = Basic;
    break;
  case 2:
    new_package->package_type = Advanced;
    break;
  case 3:
    new_package->package_type = Overnight;
    break;
  default:
    printf("Error package type! Exiting...");
    exit(1);
  }

  new_package->cost = calculate_cost(new_package);

  new_package->next = head;
  head = new_package;

  save_packages();
  printf("Package added successfully.\n");
}

// remove a package from the linked list
void remove_package() 
{
  char search_address[100];
  printf("Enter sender or delivery address of the package to remove: ");
  fgets(search_address, sizeof(search_address), stdin);
  search_address[strcspn(search_address, "\n")] = '\0'; 

  Package* current = head;
  Package* prev = NULL;

  while (current != NULL) 
  {
    // Check if the sender or delivery address matches the search address
    if (strcmp(current->sender_address, search_address) == 0 ||
      strcmp(current->delivery_address, search_address) == 0) 
    {
      if (prev == NULL) 
      {
        // removing the head of the list
        head = current->next;
      }
      else 
      {
        prev->next = current->next;
      }
      free(current);
      save_packages();
      printf("Package removed successfully.\n");
      return;
    }
    prev = current;
    current = current->next;
  }

  printf("Package with address '%s' not found.\n", search_address);
}


// view all packages in the linked list
void view_packages() 
{
  Package* current = head;
  char* package_types_str[3] = { "Basic", "Advanced", "Overnight" };

  printf("List of Packages:\n");
  printf("Sender Address | Delivery Address | Weight | Send Date | Package Type | Cost\n");
  while (current != NULL) 
  {
    printf("%14s | %16s | %6.2f | %9s | %12s | %4.2f\n", current->sender_address, 
      current->delivery_address, current->weight, current->send_date, 
      package_types_str[current->package_type], current->cost);
    current = current->next;
  }
}
