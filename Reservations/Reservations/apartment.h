#pragma once
struct Apartment
{
  int id;
  char address[100];
  int capacity;
  double price;
};

void apartment_set(struct Apartment* a,
  int id, char* address, int capacity, double price);
void apartment_print(struct Apartment* a);
