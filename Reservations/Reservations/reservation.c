#include "reservation.h"

void reservation_add_person(struct Reservation* r, struct Person p)
{
  vector_push_back(&r->persons, p);
}

void reservation_set(struct Reservation* r, 
  int id, char* name, char* surname,
  struct Date start_date, int duration, double cost)
{
  r->id = id;
  strcpy(r->name, name);
  strcpy(r->surname, surname);
  r->start_date = start_date;
  r->duration = duration;
  r->cost = cost;
  vector_init(&r->persons, 5);
}

void reservation_print(struct Reservation *resv)
{
  printf("ID: %d\n", resv->id);
  printf("Name: %s\n", resv->name);
  printf("Surname: %s\n", resv->surname);
  printf("Start Date: ");
  date_print(&resv->start_date);
  printf("\nDuration: %d\n", resv->duration);
  printf("Cost: %.2f\n", resv->cost);
}
