#include <Windows.h>
#include "entity.h"
#include "map.h"

int main()
{
  int exit = 0;
  struct Game game;
  init_game(&game);

  struct Entity entity[4];

  entity[0].type = Pacman;
  entity[0].x = 23;
  entity[0].y = 13;
  game.map[23][13].entity = &entity[0];
    
  entity[1].type = Ghost;
  entity[1].x = 5;
  entity[1].y = 5;
  game.map[5][5].entity = &entity[1];

  entity[2].type = Ghost;
  entity[2].x = 5;
  entity[2].y = 20;
  game.map[5][20].entity = &entity[2];

  entity[3].type = Ghost;
  entity[3].x = 8;
  entity[3].y = 5;
  game.map[8][5].entity = &entity[3];

  int counter = 0;
  while (game.gameActive == 1 && exit != 1)
  {
    if (GetAsyncKeyState(VK_UP) & 0x8000)
      entity[0].direction = Up;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
      entity[0].direction = Right;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
      entity[0].direction = Down;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
      entity[0].direction = Left;

    if (GetKeyState(VK_SPACE) & 0x8000)
    {
      exit = 1;
    }

    Sleep(20);

    if (counter++ > 10)
    {
      counter = 0;
      play_pacman(&entity[0], &game);
      play_ghost(&entity[1], &game);
      play_ghost(&entity[2], &game);
      play_ghost(&entity[3], &game);

      print_game(&game);
    }
  }
}