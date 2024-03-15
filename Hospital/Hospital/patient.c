#include <stdio.h>
#include <string.h>
#include <time.h>
#include "patient.h"
#include "measurement.h"

void patient_init(Patient* patient)
{
  vector_init(&patient->pm, 10);
}

void patient_set_name(Patient* patient, char* name)
{
  if (strlen(patient->name) < 100)
    strcpy_s(patient->name, strlen(patient->name) + 1, name);
  else
    strcpy_s(patient->name, 100, name);
}

void patient_set_surname(Patient* patient, char* surname)
{
  if (strlen(patient->surname) < 100)
    strcpy_s(patient->surname, strlen(patient->surname) + 1, surname);
  else
    strcpy_s(patient->surname, 100, surname);
}

void patient_set_birthyear(Patient* patient, int yearOfBirth)
{
  patient->yearOfBirth = yearOfBirth;
}

void patient_set_clinic(Patient* patient, Clinic* _clinic)
{
  patient->clinic = _clinic;
}

void patient_set_room(Patient* patient, char* room)
{
  if (strlen(patient->room) < 100)
    strcpy_s(patient->room, strlen(patient->room) + 1, room);
  else
    strcpy_s(patient->room, 100, room);
}

void patient_insert_measurement(Patient* patient, float temp, char* date)
{
  Measurement m;
  measurement_set_date(&m, date);
  m.temp = temp;

  vector_push_back(&patient->pm, m);
}

float patient_max_temp(Patient* patient)
{
  float maxt = 0.0;
  for (int j = 0; j < patient->pm.size; j++)
  {
    if (vector_at(&patient->pm, j).temp > maxt)
      maxt = vector_at(&patient->pm, j).temp;
  }

  return maxt;
}

void patient_display(Patient* patient)
{
  printf("-----Patient info: ------\n");
  printf("Name: %s %s\n", patient->name, patient->surname);
  printf("Birth year: %d\n", patient->yearOfBirth);
  printf("Room: %s\n", patient->room);
  printf("Measurements: \n");

  for (int j = 0; j < patient->pm.size; j++)
  {
    Measurement m = vector_at(&patient->pm, j);
    measurement_print(&m);
  }

  printf("Max temp: %.1f\n", patient_max_temp(patient));

  if (patient->clinic != NULL)
    clinic_display(patient->clinic);
  else
    printf("No clinic! \n");

}

int getAge(Patient* patient)
{
  time_t t = time(NULL);
  struct tm tm;
  localtime_s(&tm, &t);
  int current_year = tm.tm_year + 1900;
  return current_year - patient->yearOfBirth;
}
