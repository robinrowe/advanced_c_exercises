#ifndef MEAS
#define MEAS

typedef struct
{
  float temp;
  char date[20];
} Measurement;

void measurement_set_date(Measurement*, char*);
void measurement_print(Measurement*);

#endif // !MEAS