#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RATINGS 100

struct Ratings
{
  int *ratings;
  int ratingsCount;
};

struct Movie
{
  char *name;
  int year;
  int duration;
  char *director;
  struct Ratings ratings;
};

//////////////////////////////
// struct Ratings functions
//////////////////////////////

void initRatings(struct Ratings* r)
{
  r->ratings = (int*)malloc(MAX_RATINGS * sizeof(int));
  if (r->ratings != NULL) {
    for (int i = 0; i < MAX_RATINGS; i++)
      r->ratings[i] = 0;

    r->ratingsCount = 0;
  }
  else
  {
    printf("array allocation failed!");
    exit(1);
  }
}

void addRating(struct Ratings* r, int score)
{
  if (score >= 0 && score <= 5)
  {
    if (r->ratingsCount < MAX_RATINGS)
    {
      r->ratings[r->ratingsCount] = score;
      r->ratingsCount++;
    }
    else
      printf("Rating space exceeded\n");
  }
  else
    printf("Invalid rating score\n");
}

float getRatingAverage(struct Ratings* r)
{
  float sum = 0.0;
  for (int i = 0; i < r->ratingsCount; i++)
  {
    sum += r->ratings[i];
  }

  if (r->ratingsCount > 0)
    return sum / r->ratingsCount;
  else
    return 0;
}

void display(struct Ratings* r)
{
  for (int i = 0; i < r->ratingsCount; i++)
  {
    printf("%d ", r->ratings[i]);
  }
}

//////////////////////////////
// struct Movie functions
//////////////////////////////

void setName(struct Movie* movie, char* _name)
{
  movie->name = (char*)malloc((strlen(_name) + 1));
  if (movie->name != NULL)
    strcpy_s(movie->name, strlen(_name) + 1, _name);
  else
  {
    printf("string allocation failed!");
    exit(1);
  }
}

void setDirector(struct Movie* movie, char* _director)
{
  movie->director = (char*)malloc((strlen(_director) + 1));
  if(movie->director != NULL)
    strcpy_s(movie->director, strlen(_director) + 1, _director);
  else
  {
    printf("string allocation failed!");
    exit(1);
  }
}

void addRatingToMovie(struct Movie* movie, int score)
{
  addRating(&movie->ratings, score);
}

float getRatingAverageFromMovie(struct Movie* movie)
{
  return getRatingAverage(&movie->ratings);
}

int getAge(struct Movie* movie)
{
  time_t t = time(NULL);
  struct tm tm;
  localtime_s(&tm, &t);
  int current_year = tm.tm_year + 1900;
  return current_year - movie->year;
}

void displayMovie(struct Movie* movie)
{
  printf("------------------------------\n");
  printf("Movie info: \n");
  printf("Name: %s\n", movie->name);
  printf("Release Year: %d (age: %d \n", movie->year, getAge(movie));
  printf("Duration: %d\n", movie->duration);
  printf("Director: %s\n", movie->director);
  printf("Ratings: \n");
  display(&movie->ratings);
  printf("\nRatings average: %.1f\n", getRatingAverageFromMovie(movie));
  printf("------------------------------\n");
}

int main()
{
  struct Movie m;

  setName(&m, "Hunger Games");
  m.year = 2012;
  m.duration = 142;
  setDirector(&m, "Gary Ross");
  initRatings(&m.ratings);

  addRatingToMovie(&m, 3);
  addRatingToMovie(&m, 4);
  addRatingToMovie(&m, 4);
  addRatingToMovie(&m, 5);

  displayMovie(&m);
}
