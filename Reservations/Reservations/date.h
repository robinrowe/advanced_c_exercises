#pragma once

struct Date
{
  int day;
  int month;
  int year;
};

void date_now(struct Date* d);
void date_set(struct Date* d, int _day, int _month, int _year);
void addDays(struct Date* d, int days);
char* date_serialize();
void deserialize(struct Date* d, char* s);
char* toMysqlDate(struct Date* d);
