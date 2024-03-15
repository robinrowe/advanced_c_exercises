#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct 
{
  char title[100];
  char author[100];
  int available;
} Book;

typedef struct 
{
  char bookTitle[100];
  char userName[100];
  time_t lendingDate;
  int returned;
} LendingEvent;

void book_add(FILE* file) 
{
  Book book;
  printf("Book title: ");
  fgets(book.title, sizeof(book.title), stdin);
  // Check if the user input was longer that the array size
  if (strlen(book.title) >= sizeof(book.title) - 1 
    && book.title[sizeof(book.title) - 2] != '\n') 
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Discard excess characters
    printf("Book title is too long and has been truncated.\n");
  }
  else
  {
    // Remove trailing newline
    book.title[strcspn(book.title, "\n")] = '\0';
  }

  printf("Author: ");
  fgets(book.author, sizeof(book.author), stdin);
  if (strlen(book.author) >= sizeof(book.author) - 1 
    && book.author[sizeof(book.author) - 2] != '\n')
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Book author is too long and has been truncated.\n");
  }
  else
  {
    book.author[strcspn(book.author, "\n")] = '\0';
  }

  book.available = 1;

  // Seek to the end of the file
  fseek(file, 0, SEEK_END);
  // Add the book entry
  fwrite(&book, sizeof(Book), 1, file);
  printf("Book added successfully.\n");
}

void book_list(FILE* file) 
{
  fseek(file, 0, SEEK_SET);
  Book book;
  printf("Books available in the library:\n");
  while (fread(&book, sizeof(Book), 1, file) == 1) 
  {
    printf("Title: %s\nAuthor: %s\n", book.title, book.author);
    printf("Available: %s\n", (book.available == 1) ? "True" : "False");
    printf("-----------------------------\n");
  }
}

void book_lend(FILE* booksFile, FILE* lendingFile) 
{
  char bookTitle[100];
  char userName[100];
  printf("Enter the title of the book to lend: ");
  fgets(bookTitle, sizeof(bookTitle), stdin);
  if (strlen(bookTitle) >= sizeof(bookTitle) - 1 
    && bookTitle[sizeof(bookTitle) - 2] != '\n')
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    printf("Book title is too long and has been truncated.\n");
  }
  else
  {
    bookTitle[strcspn(bookTitle, "\n")] = '\0';
  }

  fseek(booksFile, 0, SEEK_SET);
  
  Book book;
  int bookFound = 0;
  while (fread(&book, sizeof(Book), 1, booksFile) == 1) 
  {
    if (strcmp(book.title, bookTitle) == 0 && book.available) 
    {
      book.available = 0;
      fseek(booksFile, -((long)sizeof(Book)), SEEK_CUR);
      fwrite(&book, sizeof(Book), 1, booksFile);
      bookFound = 1;

      printf("Enter your name: ");
      fgets(userName, sizeof(userName), stdin);
      userName[strcspn(userName, "\n")] = '\0';
      if (strlen(userName) >= sizeof(userName) - 1 
        && userName[sizeof(userName) - 2] != '\n')
      {
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
        printf("User name is too long and has been truncated.\n");
      }
      else
      {
        userName[strcspn(userName, "\n")] = '\0';
      }

      // Record lending event
      LendingEvent event;
      strcpy(event.bookTitle, bookTitle);
      strcpy(event.userName, userName);
      event.lendingDate = time(NULL);
      event.returned = 0;

      fseek(lendingFile, 0, SEEK_END);
      fwrite(&event, sizeof(LendingEvent), 1, lendingFile);
      printf("Book '%s' has been lent to %s.\n", bookTitle, userName);
      break;
    }
  }
  if (!bookFound) 
  {
    printf("Book '%s' not found or not available.\n", bookTitle);
  }
}

void book_return(FILE* booksFile, FILE* lendingFile) 
{
  char bookTitle[100];
  printf("Enter the title of the book to return: ");
  fgets(bookTitle, sizeof(bookTitle), stdin);
  if (strlen(bookTitle) >= sizeof(bookTitle) - 1 
    && bookTitle[sizeof(bookTitle) - 2] != '\n')
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Book title is too long and has been truncated.\n");
  }
  else
  {
    bookTitle[strcspn(bookTitle, "\n")] = '\0';
  }

  fseek(booksFile, 0, SEEK_SET);

  Book book;
  int bookFound = 0;
  while (fread(&book, sizeof(Book), 1, booksFile) == 1) 
  {
    if (strcmp(book.title, bookTitle) == 0 && !book.available) 
    {
      book.available = 1;
      fseek(booksFile, -((long)sizeof(Book)), SEEK_CUR);
      fwrite(&book, sizeof(Book), 1, booksFile);
      bookFound = 1;

      // Update lending record
      fseek(lendingFile, 0, SEEK_SET);
      LendingEvent event;
      while (fread(&event, sizeof(LendingEvent), 1, lendingFile) == 1) 
      {
        if (strcmp(event.bookTitle, bookTitle) == 0 && !event.returned) 
        {
          event.returned = 1;
          fseek(lendingFile, -((long)sizeof(LendingEvent)), SEEK_CUR);
          fwrite(&event, sizeof(LendingEvent), 1, lendingFile);
          printf("Book '%s' has been returned.\n", bookTitle);
          break;
        }
      }
      break;
    }
  }
  if (!bookFound) 
  {
    printf("Book '%s' not found or already returned.\n", bookTitle);
  }
}

void lending_events_list(FILE* lendingFile) 
{
  fseek(lendingFile, 0, SEEK_SET);
  LendingEvent event;
  printf("Lending events:\n");
  while (fread(&event, sizeof(LendingEvent), 1, lendingFile) == 1) 
  {
    printf("Book Title: %s\n", event.bookTitle);
    printf("User Name: %s\n", event.userName);
    printf("Lending Date: %s", ctime(&event.lendingDate)); 
    printf("Returned: %s\n", (event.returned == 1)? "True": "False");
    printf("-----------------------------\n");
    printf("\n");
  }
}


int main() 
{
  FILE* booksFile = fopen("books.bin", "rb+");
  if (booksFile == NULL) 
  {
    booksFile = fopen("books.bin", "wb+");
    if (booksFile == NULL) 
    {
      printf("Error opening books file.\n");
      return 1;
    }
  }

  FILE* lendingFile = fopen("lending_events.bin", "rb+");
  if (lendingFile == NULL)
  {
    lendingFile = fopen("lending_events.bin", "wb+");
    if (lendingFile == NULL)
    {
      printf("Error opening lending events file.\n");
      return 1;
    }
  }

  int choice;
  do 
  {
    printf("\n1. Add a book\n2. List all books\n3. Lend a book\n4. Return a book\n5. List lending events\n0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline left in the buffer by scanf

    switch (choice) 
    {
    case 1:
      book_add(booksFile);
      break;
    case 2:
      book_list(booksFile);
      break;
    case 3:
      book_lend(booksFile, lendingFile);
      break;
    case 4:
      book_return(booksFile, lendingFile);
      break;
    case 5:
      lending_events_list(lendingFile);
      break;
    case 0:
      printf("Exiting.\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 0);

  fclose(booksFile);
  fclose(lendingFile);
  return 0;
}
