#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "person.h"

struct Vector
{
  struct Person* persons;
  size_t size;
  size_t capacity;
} ;

void vector_init(struct Vector* vec, size_t capacity);
void vector_push_back(struct Vector* vec, struct Person value);
struct Person vector_at(const struct Vector* vec, size_t index);
void vector_free(struct Vector* vec);



