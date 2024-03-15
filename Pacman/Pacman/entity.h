#pragma once
#include "map.h"

enum Direction { Up, Right, Down, Left };
enum EntityType { Pacman, Ghost };

struct Entity
{
  int x;
  int y;
  enum Direction direction;
  enum EntityType type;
};

void entity_move(struct Entity* entity, struct Game* map, int newX, int newY);
void play_pacman(struct Entity* pacman, struct Game* map);
void play_ghost(struct Entity* ghost, struct Game* map);
