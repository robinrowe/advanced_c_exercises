#include <stdio.h>
#include <string.h>
#include "db.h"

void error_exit(MYSQL* con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void db_init()
{
  conn = mysql_init(NULL);
  mysql_autocommit(conn, 0);

  if (conn == NULL)
  {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }

  if (mysql_real_connect(conn, DEFAULT_URI, 
    EXAMPLE_USER, EXAMPLE_PASS, EXAMPLE_DB, 0, NULL, 0) == NULL)
  {
    error_exit(conn);
  }
}

void db_close()
{
  mysql_close(conn);
}

void db_get_apartments(struct Apartment** apartments, int* count)
{
  if (mysql_query(conn, "SELECT id, address, capacity, price from Apartments"))
  {
    error_exit(conn);
  }

  MYSQL_RES* result = mysql_store_result(conn);

  if (result == NULL)
  {
    error_exit(conn);
  }

  int num_fields = mysql_num_fields(result);
  *count = mysql_num_rows(result);
  *apartments = malloc((*count) * sizeof(struct Apartment));
  if (*apartments == NULL)
  {
    printf("Memory allocation error!\n");
    exit(1);
  }

  MYSQL_ROW row;
  int i = 0;
  while ((row = mysql_fetch_row(result)))
  {
    (*apartments)[i].id = atoi(row[0]);
    strcpy((*apartments)[i].address, row[1]);
    (*apartments)[i].capacity = atoi(row[2]);
    (*apartments)[i].price = atof(row[3]);
    i++;
  }

  mysql_free_result(result);
}

void db_get_all_reservations(struct Reservation** reservations, int *count)
{
  if (mysql_query(conn, "SELECT id, name, surname, start_date, duration, cost from Reservations"))
  {
    error_exit(conn);
  }

  MYSQL_RES* result = mysql_store_result(conn);

  if (result == NULL)
  {
    error_exit(conn);
  }

  int num_fields = mysql_num_fields(result);
  *count = mysql_num_rows(result);
  *reservations = malloc((*count) * sizeof(struct Reservation));
  if (*reservations == NULL)
  {
    printf("Memory allocation error!\n");
    exit(1);
  }

  MYSQL_ROW row;
  int i = 0;
  while ((row = mysql_fetch_row(result)))
  {
    (*reservations)[i].id = atoi(row[0]);
    strcpy((*reservations)[i].name, row[1]);
    strcpy((*reservations)[i].surname, row[2]);

    char* ptr = strtok((char*)row[3], "-");
    int year = atoi(ptr);
    ptr = strtok(NULL, "-");
    int month = atoi(ptr);
    ptr = strtok(NULL, "-");
    int day = atoi(ptr);
    date_set(&((*reservations)[i].start_date), day, month, year);

    (*reservations)[i].duration = atoi(row[4]);
    (*reservations)[i].cost = atof(row[5]);
    i++;
  }

  mysql_free_result(result);
}

void db_get_reservations_by_surname(struct Reservation** reservations, int* count, char* surname)
{
  char query[1000];
  sprintf(query, "SELECT id, name, surname, start_date, duration, cost from Reservations where surname like '%%%s%%'", surname);
  if (mysql_query(conn, query))
  {
    error_exit(conn);
  }

  MYSQL_RES* result = mysql_store_result(conn);

  if (result == NULL)
  {
    error_exit(conn);
  }

  int num_fields = mysql_num_fields(result);
  *count = mysql_num_rows(result);
  *reservations = malloc((*count) * sizeof(struct Reservation));
  if (*reservations == NULL)
  {
    printf("Memory allocation error!\n");
    exit(1);
  }

  MYSQL_ROW row;
  int i = 0;
  while ((row = mysql_fetch_row(result)))
  {
    (*reservations)[i].id = atoi(row[0]);
    strcpy((*reservations)[i].name, row[1]);
    strcpy((*reservations)[i].surname, row[2]);

    char* ptr = strtok((char*)row[3], "-");
    int year = atoi(ptr);
    ptr = strtok(NULL, "-");
    int month = atoi(ptr);
    ptr = strtok(NULL, "-");
    int day = atoi(ptr);
    date_set(&((*reservations)[i].start_date), day, month, year);

    (*reservations)[i].duration = atoi(row[4]);
    (*reservations)[i].cost = atof(row[5]);
    i++;
  }

  mysql_free_result(result);
}

void db_insert_reservation(struct Reservation* resv)
{
  char query[1000];
  char date[20];
  sprintf(date, "%d-%d-%d", resv->start_date.year, resv->start_date.month, resv->start_date.day);
  sprintf(query, "insert into Reservations(name, surname, start_date, duration, cost) values('%s','%s','%s',%d,%f)",
    resv->name, resv->surname, date, resv->duration, resv->cost);

  if(mysql_query(conn, query))
  {
    mysql_rollback(conn);
    error_exit(conn);
  }

  int id = mysql_insert_id(conn);

  for (int i = 0; i < resv->persons.size; i++)
  {
    sprintf(query, "insert into Persons(name, surname, birth_year, reservation_id) values('%s', '%s', %d, %d)",
      vector_at(&resv->persons, i).name, vector_at(&resv->persons, i).surname,
      vector_at(&resv->persons, i).birth_year, id);

    if (mysql_query(conn, query))
    {
      mysql_rollback(conn);
      error_exit(conn);
    }
  }

  mysql_commit(conn);
  printf("Reservation inserted successfully!\n\n");
}