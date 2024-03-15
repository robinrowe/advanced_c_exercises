#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "entity.h"

struct Vector{
  struct Pair* pair;
  size_t size;
  size_t capacity;
} ;

void vector_init(struct Vector* vec, size_t capacity);
void vector_push_back(struct Vector* vec, struct Pair value);
struct Pair vector_at(const struct Vector* vec, size_t index);
void vector_free(struct Vector* vec);
int vector_contains_pair(struct Vector* vec, int x, int y);


