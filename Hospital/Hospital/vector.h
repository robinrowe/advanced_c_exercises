#include <stdio.h>
#include <stdlib.h>
#include "measurement.h"

#ifndef VECTOR
#define VECTOR

typedef struct {
  Measurement* data;  
  size_t size;   
  size_t capacity;
} Vector;

void vector_init(Vector* vec, size_t capacity);
void vector_push_back(Vector* vec, Measurement value);
Measurement vector_at(const Vector* vec, size_t index);
void vector_free(Vector* vec);

#endif // !VECTOR

