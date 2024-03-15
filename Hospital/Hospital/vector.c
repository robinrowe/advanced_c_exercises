#include "vector.h"

void vector_init(Vector* vec, size_t capacity)
{
  vec->data = (Measurement*)malloc(capacity * sizeof(Measurement));
  if (vec->data == NULL) 
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  vec->size = 0;
  vec->capacity = capacity;
}

void vector_push_back(Vector* vec, Measurement value) 
{
  if (vec->size >= vec->capacity) 
  {
    vec->capacity *= 2;
    vec->data = (Measurement*)realloc(vec->data, vec->capacity * sizeof(Measurement));
    if (vec->data == NULL) 
    {
      printf("Memory allocation failed\n");
      exit(1);
    }
  }
  vec->data[vec->size++] = value;
}

Measurement vector_at(const Vector* vec, size_t index) 
{
  if (index >= vec->size) 
  {
    printf("Array index out of bounds\n");
    exit(1);
  }
  return vec->data[index];
}

void vector_free(Vector* vec) 
{
  free(vec->data);
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
}
