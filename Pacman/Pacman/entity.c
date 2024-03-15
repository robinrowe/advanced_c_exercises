#include <Math.h>
#include "entity.h"
#include "vector.h"

int find_min_distance_index(struct Vector* vec, struct Pair* pacmanCoords)
{
  int index = 0;
  double min_dist = 
    sqrt(pow(vec->pair[0].x - pacmanCoords->x, 2) + pow(vec->pair[0].y - pacmanCoords->y, 2));
  double temp_dist = 0;

  for (int i = 0; i < vec->size; i++)
  {
    temp_dist = 
      sqrt(pow(vec->pair[i].x - pacmanCoords->x, 2) + pow(vec->pair[i].y - pacmanCoords->y, 2));
    if (temp_dist < min_dist)
      index = i;
  }
  return index;
}

void entity_move(struct Entity* entity, struct Game* game, int newX, int newY)
{
  if (newX == entity->x && newY == entity->y)
    printf("error");
  
  game->map[newX][newY].entity 
    = game->map[entity->x][entity->y].entity;
  
  game->map[entity->x][entity->y].entity = NULL;
  entity->x = newX;
  entity->y = newY;
}

void play_pacman(struct Entity* pacman, struct Game *game)
{
  struct Vector candidateBlocks;
  int x = pacman->x;
  int y = pacman->y;

  vector_init(&candidateBlocks, 10);

  for (int i = x - 1; i <= x + 1; i++)
    for (int j = y - 1; j <= y + 1; j++)
    {
      if (i >= 0 && i < game->sizeX
        && j >= 0 && j < game->sizeY
        && !(i == x && j == y)
        && game->map[i][j].type != Wall)
      {
        struct Pair newPair;
        newPair.x = i;
        newPair.y = j;
        vector_push_back(&candidateBlocks, newPair);
      }
    }

  if (pacman->direction == Up
    && vector_contains_pair(&candidateBlocks, x - 1, y))
  {
    entity_move(pacman, game, x - 1, y);
  }
  else if (pacman->direction == Right
    && vector_contains_pair(&candidateBlocks, x, y + 1))
  {
    entity_move(pacman, game, x, y + 1);
  }
  else if (pacman->direction == Down
    && vector_contains_pair(&candidateBlocks, x + 1, y))
  {
    entity_move(pacman, game, x + 1, y);
  }
  else if (pacman->direction == Left
    && vector_contains_pair(&candidateBlocks, x, y - 1))
  {
    entity_move(pacman, game, x, y - 1);
  }
  game->pacmanLocation.x = x;
  game->pacmanLocation.y = y;
  
  if (game->map[x][y].type == Point)
  {
    game->map[x][y].type = Empty;
    game->pointsLeft--;
    if (game->pointsLeft == 0)
      game->gameActive = 0;
  }
}


void play_ghost(struct Entity* ghost, struct  Game* game)
{
  struct Vector candidateBlocks;
  int x = ghost->x;
  int y = ghost->y;

  vector_init(&candidateBlocks, 10);

  for (int i = x - 1; i <= x + 1; i++)
    for (int j = y - 1; j <= y + 1; j++)
    {
      if (i >= 0 && i < game->sizeX
        && j >= 0 && j < game->sizeY
        && !(i == x && j == y)
        && game->map[i][j].type != Wall)
      {
        struct Pair newPair;
        newPair.x = i;
        newPair.y = j;
        vector_push_back(&candidateBlocks, newPair);
      }
    }

  int index = find_min_distance_index(&candidateBlocks, &game->pacmanLocation);

  if (game->map[vector_at(&candidateBlocks, index).x][vector_at(&candidateBlocks, index).y].entity != NULL)
  {
    //move only if pacman is there
    if (game->map[vector_at(&candidateBlocks, index).x][vector_at(&candidateBlocks, index).y].entity->type == Pacman)
    {
      //eat pacman
      game->map[vector_at(&candidateBlocks, index).x][vector_at(&candidateBlocks, index).y].entity = NULL;
      game->gameActive = 0;
      entity_move(ghost, game, vector_at(&candidateBlocks, index).x, vector_at(&candidateBlocks, index).y);
    }
  }
  else
  {
    entity_move(ghost, game, vector_at(&candidateBlocks, index).x, vector_at(&candidateBlocks, index).y);
  }
}


