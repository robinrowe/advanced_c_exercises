#pragma once
#include "vector.h"
#include "date.h"
#include "person.h"

struct Reservation
{
  int id;
  char name[100];
  char surname[100];
  struct Date start_date;
  int duration;
  double cost;
  struct Vector persons;
};

void reservation_add_person(struct Reservation* r, struct Person p);
void reservation_set(struct Reservation* r,
  int id, char* name, char* surname,
  struct Date start_date, int duration, double cost);
void reservation_print(struct Reservation* resv);

