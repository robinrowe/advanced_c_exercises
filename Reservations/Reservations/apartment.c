#include <stdio.h>
#include <string.h>
#include "apartment.h"

void apartment_set(struct Apartment* a, 
  int id, char* address, int capacity, double price)
{
  a->id = id;
  strcpy(a->address, address);
  a->capacity = capacity;
  a->price = price;
}

void apartment_print(struct Apartment *a)
{
  printf("ID: %d\n", a->id);
  printf("Address: %s\n", a->address) ;
  printf("Capacity: %d\n", a->capacity);
  printf("Price: %f\n", a->price);
}
  