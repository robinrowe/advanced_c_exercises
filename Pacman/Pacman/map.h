#pragma once
#include "entity.h"

enum BlockType { Wall, Point, Empty };

struct Pair{
  int x;
  int y;
};

struct Block
{
  enum BlockType type;
  struct Entity* entity;
};

struct Game
{
  struct Block map[32][28];
  int sizeX;
  int sizeY;
  int gameActive;
  struct Pair pacmanLocation;
  int pointsLeft;
};

void init_game(struct Game* map);
void print_game(struct Game* map);

