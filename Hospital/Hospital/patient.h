#include "measurement.h"
#include "clinic.h"
#include "vector.h"

#ifndef PATIENT

typedef struct
{
  char name[100];
  char surname[100];
  int yearOfBirth;
  Clinic* clinic;
  char room[100];
  Vector pm;
} Patient;

void patient_init(Patient*);
void patient_set_name(Patient*, char* name);
void patient_set_surname(Patient*, char* surname);
void patient_set_birthyear(Patient*, int yearOfBirth);
void patient_set_clinic(Patient*, Clinic* clinic);
void patient_set_room(Patient*, char* room);
void patient_insert_measurement(Patient*, float temp, char* date);
float patient_max_temp(Patient*);
void patient_display(Patient*);
int getAge(Patient*);

#endif // !PATIENT
