#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char title[100];
  char artist[100];
  char album[100];
  int release_year;
} Song;

// Compare songs based on artist
int compareByArtist(const Song* a, const Song* b)
{
  return strcmp(a->artist, b->artist);
}

// Compare songs based on album
int compareByAlbum(const Song* a, const Song* b)
{
  return strcmp(a->album, b->album);
}

// Compare songs based on release date
int compareByReleaseDate(const Song* a, const Song* b)
{
  return (a->release_year - b->release_year);
}

void insertionSort(Song arr[], int n, int (*compare)(const Song*, const Song*))
{
  int i, j;
  Song key;
  for (i = 1; i < n; i++)
  {
    key = arr[i];
    j = i - 1;

    // Move elements of arr[0..i-1], that are greater than key, 
    // to one position ahead of their current position
    while (j >= 0 && compare(&arr[j], &key) > 0)
    {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

int main()
{
  Song songs[] = {
      {"Song1", "Artist2", "Album1", 2010},
      {"Song2", "Artist1", "Album2", 2005},
      {"Song3", "Artist3", "Album1", 2015},
      {"Song4", "Artist4", "Album3", 2008},
      {"Song5", "Artist1", "Album2", 2003},
      {"Song6", "Artist3", "Album4", 2019},
      {"Song7", "Artist2", "Album3", 2012},
      {"Song8", "Artist4", "Album4", 2017},
      {"Song9", "Artist5", "Album5", 2014},
      {"Song10", "Artist5", "Album5", 2011} };

  int num_songs = sizeof(songs) / sizeof(songs[0]);

  // Sort by artist
  insertionSort(songs, num_songs, compareByArtist);
  printf("Sorted by Artist:\n");
  for (int i = 0; i < num_songs; i++)
  {
    printf("%s by %s\n", songs[i].title, songs[i].artist);
  }
  printf("\n");

  // Sort by album
  insertionSort(songs, num_songs, compareByAlbum);
  printf("Sorted by Album:\n");
  for (int i = 0; i < num_songs; i++)
  {
    printf("%s from %s\n", songs[i].title, songs[i].album);
  }
  printf("\n");

  // Sort by release date
  insertionSort(songs, num_songs, compareByReleaseDate);
  printf("Sorted by Release Date:\n");
  for (int i = 0; i < num_songs; i++)
  {
    printf("%s released in %d\n", songs[i].title, songs[i].release_year);
  }

  return 0;
}
