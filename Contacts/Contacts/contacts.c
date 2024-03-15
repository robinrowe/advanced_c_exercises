#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100

typedef struct Contact 
{
  char name[50];
  char phone[20];
  struct Contact* next;
} Contact;

typedef struct HashMap 
{
  Contact* bucket_table[HASH_SIZE];
} HashMap;

unsigned int hash(const char* name) 
{
  unsigned int hash = 0;
  int c;
  while (c = *name++) 
  {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % HASH_SIZE;
}

void contact_init(HashMap* phonebook)
{
  for (int i = 0; i < HASH_SIZE; i++)
  {
    phonebook->bucket_table[i] = NULL;
  }
}

void contact_add(HashMap* phonebook, const char* name, const char* phone)
{
  unsigned int hash_index = hash(name);
  Contact* new_contact = (Contact*)malloc(sizeof(Contact));
  if (!new_contact) 
  {
    printf("Memory allocation failed.\n");
    return;
  }
  strcpy(new_contact->name, name);
  strcpy(new_contact->phone, phone);
  new_contact->next = phonebook->bucket_table[hash_index];
  phonebook->bucket_table[hash_index] = new_contact;
}

void contact_remove(HashMap* phonebook, const char* name) 
{
  unsigned int index = hash(name);
  Contact* contact = phonebook->bucket_table[index];
  Contact* previous = NULL;

  while (contact != NULL) 
  {
    if (strcmp(contact->name, name) == 0) 
    {
      if (previous == NULL) 
      {
        // Contact to remove is the head of the list
        phonebook->bucket_table[index] = contact->next;
      }
      else 
      {
        // Contact to remove is not the head of the list
        previous->next = contact->next;
      }
      free(contact);
      printf("Contact '%s' removed successfully.\n", name);
      return;
    }
    previous = contact;
    contact = contact->next;
  }
  printf("Contact '%s' not found.\n", name);
}

void contact_search(HashMap* phonebook, const char* name)
{
  unsigned int hash_index = hash(name);
  Contact* contact = phonebook->bucket_table[hash_index];
  while (contact != NULL) 
  {
    if (strcmp(contact->name, name) == 0) 
    {
      printf("Name: %s\nPhone Number: %s\n", contact->name, contact->phone);
      return;
    }
    contact = contact->next;
  }
  printf("Contact '%s' not found.\n", name);
}

int main() 
{
  HashMap phonebook;
  contact_init(&phonebook);

  contact_add(&phonebook, "John", "235454545");
  contact_add(&phonebook, "Jane", "775755454");
  contact_add(&phonebook, "George", "4344343477");

  contact_search(&phonebook, "John");
  contact_search(&phonebook, "Alex");
  contact_search(&phonebook, "George");

  contact_remove(&phonebook, "Jake");
  contact_remove(&phonebook, "Jane");
  contact_search(&phonebook, "Jane");

  return 0;
}
