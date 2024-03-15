#include "patient.h"

int main()
{
  Clinic c1;
  clinic_set_name(&c1, "Surgery");
  clinic_set_director(&c1, "A. Dobbs");

  Clinic c2;
  clinic_set_name(&c2, "Cardiology");
  clinic_set_director(&c2, "B.Smith");

  Patient p1;
  patient_init(&p1);
  patient_set_name(&p1, "John");
  patient_set_surname(&p1, "Doe");
  patient_set_birthyear(&p1, 1970);
  patient_set_clinic(&p1, &c1);
  patient_set_room(&p1, "303");
  patient_insert_measurement(&p1, 37.5, "01/01/2023 00:00");
  patient_insert_measurement(&p1, 38.1, "01/01/2023 06:00");
  patient_insert_measurement(&p1, 37.9, "01/01/2023 09:00");

  Patient p2;
  patient_init(&p2);
  patient_set_name(&p2, "Jane");
  patient_set_surname(&p2, "Doe");
  patient_set_birthyear(&p2, 1985);
  patient_set_clinic(&p2, &c2);
  patient_set_room(&p2, "306");
  patient_insert_measurement(&p2, 36.5, "01/01/2023 00:00");
  patient_insert_measurement(&p2, 38.0, "01/01/2023 06:00");


  patient_set_clinic(&p2, &c1);
  clinic_set_director(&c1, "D. Jones");

  patient_display(&p1);
  patient_display(&p2);
}
