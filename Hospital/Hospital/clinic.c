#include <stdio.h>
#include <string.h>
#include "clinic.h"

void clinic_display(Clinic* clinic)
{
  printf("----Clinic ---- \n");
  printf("Name: %s\n", clinic->name);
  printf("Director: %s\n", clinic->director);
}

void clinic_set_name(Clinic* clinic, char* name)
{
  if(strlen(clinic->name) < 100)
    strcpy_s(clinic->name, strlen(clinic->name)+1, name);
  else
    strcpy_s(clinic->name, 100, name);
}

void clinic_set_director(Clinic* clinic, char* director)
{
  if (strlen(clinic->director) < 100)
    strcpy_s(clinic->director, strlen(clinic->director) + 1, director);
  else
    strcpy_s(clinic->director, 100, director);
}