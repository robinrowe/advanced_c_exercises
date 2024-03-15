#include <stdio.h>
#include <time.h>

struct Movie
{
  char name[100];
  int year;
  int duration;
  char director[100];
  int ratings[100];
  int ratingsCount;
};

void addRating(struct Movie* movie, int score)
{
  if (score >= 0 && score <= 5)
  {
    if (movie->ratingsCount < 100)
    {
      movie->ratings[movie->ratingsCount] = score;
      movie->ratingsCount++;
    }
    else
      printf("Rating space exceeded\n");
  }
  else
    printf("Invalid rating score\n");
}

float getRatingAverage(struct Movie* movie)
{
  float sum = 0.0;
  for (int i = 0; i < movie->ratingsCount; i++)
  {
    sum += movie->ratings[i];
  }

  if (movie->ratingsCount > 0)
    return sum / movie->ratingsCount;
  else
    return 0;
}

int getAge(struct Movie* movie)
{
  time_t t = time(NULL);
  struct tm tm;
  localtime_s(&tm, &t);
  int current_year = tm.tm_year + 1900;
  return current_year - movie->year;
}

void display(struct Movie* movie)
{
  printf("------------------------------\n");
  printf("Movie info: \n");
  printf("Name: %s\n", movie->name);
  printf("Release Year: %d (age: %d \n", movie->year, getAge(movie));
  printf("Duration: %d\n", movie->duration);
  printf("Director: %s\n", movie->director);
  printf("Ratings: \n");
  for (int i = 0; i < movie->ratingsCount; i++)
  {
    printf("%d ", movie->ratings[i]);
  }
  printf("\nRatings average: %.1f\n", getRatingAverage(movie));
  printf("------------------------------\n");
}

int main()
{
  struct Movie m;

  strcpy(m.name, "Hunger Games");
  m.year = 2012;
  m.duration = 142;
  strcpy(m.director, "Gary Ross");
  m.ratingsCount = 0;

  addRating(&m, 3);
  addRating(&m, 4);
  addRating(&m, 4);
  addRating(&m, 5);

  display(&m);
}
