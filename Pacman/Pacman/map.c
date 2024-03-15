#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "entity.h"

char* chart[32] =
{
"****************************",
"*............**............*",
"*.****.*****.**.*****.****.*",
"*.****.*****.**.*****.****.*",
"*.****.*****.**.*****.****.*",
"*..........................*",
"*.****.**.********.**.****.*",
"*.****.**.********.**.****.*",
"*......**....**....**......*",
"******.***** ** *****.******",
"******.***** ** *****.******",
"******.**          **.******",
"******.** ******** **.******",
"******.** ******** **.******",
"*     .   ********   .     *",
"******.** ******** **.******",
"******.** ******** **.******",
"******.**          **.******",
"******.**.********.**.******",
"******.**.********.**.******",
"*............**............*",
"*.****.*****.**.*****.****.*",
"*.****.*****.**.*****.****.*",
"*...**................**...*",
"***.**.**.********.**.**.***",
"***.**.**.********.**.**.***",
"*...**.**....**....**.**...*",
"*......**....**....**......*",
"*.**********.**.**********.*",
"*.**********....**********.*",
"*..........................*",
"****************************"
};

void init_game(struct Game* game)
{
  game->sizeX = 32;
  game->sizeY = 28;
  game->gameActive = 1;

  game->pointsLeft = 0;
  for (int i = 0; i < 32; i++)
  {
    char* line = chart[i];
    for (int j = 0; j < 28; j++)
    {
      game->map[i][j].entity = NULL;

      if (line[j] == '.')
      {
        game->map[i][j].type = Point;
        game->pointsLeft++;
      }
      else if (line[j] == '*')
        game->map[i][j].type = Wall;
      if (line[j] == ' ')
        game->map[i][j].type = Empty;
    }
  }
}

void print_game(struct Game* game)
{
  char output[1000];
  memset(output, 0, 1000);
  system("cls");
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 28; j++)
    {
      if (game->map[i][j].entity != NULL)
      {
        if(game->map[i][j].entity->type == Pacman)
          strcat(output, "C");
        else if (game->map[i][j].entity->type == Ghost)
          strcat(output, "A");
      }
      else
      {
        if (game->map[i][j].type == Wall)
          strcat(output, "*");
        else if (game->map[i][j].type == Point)
          strcat(output, ".");
        else if (game->map[i][j].type == Empty)
          strcat(output, " ");
      }
    }
    strcat(output, "\n");
  }
  printf(output);
}

