#include "db.h"
#include "apartment.h"
#include "reservation.h"
#include "date.h"
#include "person.h"

void add_persons(struct Reservation* resv)
{
  char name[100], surname[100];
  int birth_year;
  char option;

  printf("Give the persons:\n");
  do
  {
    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Surname: ");
    fgets(surname, sizeof(surname), stdin);
    surname[strcspn(surname, "\n")] = '\0';

    printf("Birth Year: ");
    scanf("%d", &birth_year);
    getchar();

    struct Person p;
    person_set(&p, 0, name, surname, birth_year);

    reservation_add_person(resv, p);

    printf("Add another person? (y/n)\n");
    scanf("%c", &option);
    getchar();
  } while (option != 'n' && option != 'N');
}

void add_reservation()
{
  int option;
  char name[100], surname[100], startDateStr[100];
  int duration, day, month, year;

  printf("Name: ");
  fgets(name, sizeof(name), stdin);
  name[strcspn(name, "\n")] = '\0'; 

  printf("Surname: ");
  fgets(surname, sizeof(surname), stdin);
  surname[strcspn(surname, "\n")] = '\0';

  printf("Start date: ");
  fgets(startDateStr, sizeof(startDateStr), stdin);
  startDateStr[strcspn(startDateStr, "\n")] = '\0';

  sscanf(startDateStr, "%2d/%2d/%4d", &day, &month, &year);
  struct Date startDate;
  date_set(&startDate, day, month, year);

  printf("Duration: ");
  scanf("%d", &duration);
  getchar();

  printf("Available apartments:\n");
  struct Apartment* apartments;
  int count;
  db_get_apartments(&apartments, &count);
  for (int i = 0; i < count; i++)
  {
    printf("Apartment no.%d\n", i + 1);
    apartment_print(&apartments[i]);
  }
  printf("Press 0 to cancel:");
  scanf("%d", &option);
  getchar();

  if (option > 0 && option <= sizeof(apartments))
  {
    struct Reservation resv;
    reservation_set(&resv, 0, name, surname, startDate, duration, 
      apartments[option - 1].price);
    
    add_persons(&resv);

    db_insert_reservation(&resv);
  }
}

void search_reservation()
{
  char surname[100];
  printf("Enter surname (also partial): ");
  scanf("%s", surname);
  getchar();

  struct Reservation *resv;
  int count;
  db_get_reservations_by_surname(&resv, &count, surname);

  for (int i = 0; i < count; i++)
  {
    reservation_print(&resv[i]);
    printf("-------------\n");
  }
}

void list_reservations()
{
  struct Reservation* resv;
  int count;
  db_get_all_reservations(&resv, &count);

  for (int i = 0; i < count; i++)
  {
    reservation_print(&resv[i]);
    printf("-------------\n");
  }
}

void menu()
{
  int option;
  do
  {
    printf("Options: \n");
    printf("1) Add reservation \n");
    printf("2) Search reservation \n");
    printf("3) View all reservations \n");
    printf("0) Exit \n");
    printf("Enter your selection:");
    scanf("%d", &option);
    getchar();

    switch (option)
    {
    case 1:
      add_reservation();
      break;
    case 2:
      search_reservation();
      break;
    case 3:
      list_reservations();
      break;
    case 0:
      break;
    default:
      printf("Please enter selection again:");
      break;
    }
  } while (option != 0);
}


int main()
{
  db_init();
  menu();
  db_close();
}

