#pragma once
#include "date.h"
#include "reservation.h"
#include "apartment.h"

#include <mysql.h>

#define DEFAULT_URI "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "root"
#define EXAMPLE_DB "test"

MYSQL* conn; 

void db_init();
void db_close();
void db_get_apartments(struct Apartment** apartments, int* count);
void db_get_all_reservations(struct Reservation** reservations, int* count);
void db_get_reservations_by_surname(struct Reservation** reservations, 
  int* count, char* surname);
void db_insert_reservation(struct Reservation* resv);

