#include "vector.h"

void vector_init(struct Vector* vec, size_t capacity)
{
  vec->persons = (struct Person*)malloc(capacity * sizeof(struct Person));
  if (vec->persons == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  vec->size = 0;
  vec->capacity = capacity;
}

void vector_push_back(struct Vector* vec, struct Person value)
{
  if (vec->size >= vec->capacity)
  {
    vec->capacity *= 2;
    vec->persons = (struct Person*)realloc(vec->persons, 
      vec->capacity * sizeof(struct Person));
    if (vec->persons == NULL)
    {
      printf("Memory allocation failed\n");
      exit(1);
    }
  }
  vec->persons[vec->size++] = value;
}

struct Person vector_at(const struct Vector* vec, size_t index)
{
  if (index >= vec->size)
  {
    printf("Array index out of bounds\n");
    exit(1);
  }
  return vec->persons[index];
}

void vector_free(struct Vector* vec)
{
  free(vec->persons);
  vec->persons = NULL;
  vec->size = 0;
  vec->capacity = 0;
}
