#pragma once
struct Person
{
  int id;
  char name[100];
  char surname[100];
  int birth_year;
};

void person_set(struct Person* p, 
  int id, char* name, char* surname, int birth_year);
void person_print(struct Person* p);
