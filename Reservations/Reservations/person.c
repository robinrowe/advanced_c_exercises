#include <stdio.h>
#include <string.h>
#include "person.h"

void person_set(struct Person *p, 
  int id, char* name, char* surname, int birth_year)
{
  p->id = id;
  strcpy(p->name, name);
  strcpy(p->surname, surname);
  p->birth_year = birth_year;
}

void person_print(struct Person* p)
{
  printf("ID: %d\n", p->id);
  printf("Name: %s\n", p->name);
  printf("Surname: %s\n", p->surname);
  printf("Birth Year: %d\n", p->birth_year);
}