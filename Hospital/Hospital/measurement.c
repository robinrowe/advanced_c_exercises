#include "measurement.h"
#include <stdio.h>
#include <string.h>

void measurement_set_date(Measurement* m, char* date)
{
  if (strlen(m->date) < 20)
    strcpy_s(m->date, strlen(m->date) + 1, date);
  else
    strcpy_s(m->date, 20, date);
}

void measurement_print(Measurement* m)
{
  printf("Date: %s, temperature: %.1f\n", m->date, m->temp);
}
