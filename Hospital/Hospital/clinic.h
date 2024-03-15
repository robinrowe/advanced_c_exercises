#ifndef CLINIC
#define CLINIC

typedef struct
{
  char name[100];
  char director[100];
} Clinic;

void clinic_set_name(Clinic* clinic, char* name);
void clinic_set_director(Clinic* clinic, char* director);
void clinic_display(Clinic* clinic);

#endif // !CLINIC
