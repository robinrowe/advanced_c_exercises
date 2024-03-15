#include "vector.h"

void vector_init(struct Vector* vec, size_t capacity)
{
  vec->pair = (struct Pair*)malloc(capacity * sizeof(struct Pair));
  if (vec->pair == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  vec->size = 0;
  vec->capacity = capacity;
}

void vector_push_back(struct Vector* vec, struct Pair value)
{
  if (vec->size >= vec->capacity)
  {
    vec->capacity *= 2;
    vec->pair = (struct Pair*)realloc(vec->pair, vec->capacity * sizeof(struct Pair));
    if (vec->pair == NULL)
    {
      printf("Memory allocation failed\n");
      exit(1);
    }
  }
  vec->pair[vec->size++] = value;
}

struct Pair vector_at(const struct Vector* vec, size_t index)
{
  if (index >= vec->size)
  {
    printf("Array index out of bounds\n");
    exit(1);
  }
  return vec->pair[index];
}

void vector_free(struct Vector* vec)
{
  free(vec->pair);
  vec->pair = NULL;
  vec->size = 0;
  vec->capacity = 0;
}

int vector_contains_pair(struct Vector* vec, int x, int y)
{
  for (int i = 0; i < vec->size; i++)
  {
    struct Pair pair = vec->pair[i];
    if (pair.x == x && pair.y == y)
      return 1;
  }

  return 0;
}
