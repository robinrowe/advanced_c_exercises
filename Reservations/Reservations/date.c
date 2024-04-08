#include <stdio.h>
#include <time.h>
#include "Date.h"

void date_now(struct Date *d)
{
  time_t t = time(0);
  
  struct tm now;
  localtime_s(&now, &t);
  d->day = now.tm_mday;
  d->month = now.tm_mon + 1;
  d->year = now.tm_year + 1900;
}

void date_set(struct Date *d, int _day, int _month, int _year)
{
  d->day = _day;
  d->month = _month;
  d->year = _year;
}

void addDays(struct Date *d, int days)
{
  struct tm tmp;
  tmp.tm_mday = d->day;
  tmp.tm_mon = d->month - 1;
  tmp.tm_year = d->year - 1900;
  time_t now = mktime(&tmp);

  time_t newSeconds = now + days * (60 * 60 * 24);
  struct tm newDate;
  localtime_s(&newDate, &newSeconds);

  d->day = newDate.tm_mday;
  d->month = newDate.tm_mon + 1;
  d->year = newDate.tm_year + 1900;
}

void date_print(struct Date* d)
{
  printf("%d/%d/%d", d->day, d->month, d->year);
}
