#include <stdio.h>
#include <stdlib.h>

#define ROWS 15
#define COLS 15

int maze[ROWS][COLS] = {
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
  {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0},
  {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
  {0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
  {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
  {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
};

struct Point 
{
  int row, col;
};

struct Stack
{
  struct Point* points;
  int top;
};

void stack_init(struct Stack* stack, int capacity) 
{
  stack->points = (struct Point*)malloc(capacity * sizeof(struct Point));
  stack->top = -1;
}

int stack_is_empty(struct Stack* stack) 
{
  return stack->top == -1;
}

void stack_push(struct Stack* stack, int x, int y) 
{
  stack->top++;
  stack->points[stack->top].row = x;
  stack->points[stack->top].col = y;
}

struct Point stack_pop(struct Stack* stack) 
{
  return stack->points[stack->top--];
}

// Check if we can move to this cell
int canMove(int row, int col) 
{
  return (row >= 0 && row < ROWS && col >= 0 && col < COLS && maze[row][col] == 0);
}

void printMaze(int maze[ROWS][COLS])
{
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) 
    {
      printf("%d ", maze[i][j]);
    }
    printf("\n");
  }
}

// Solve the maze using backtracking
int solveMaze(struct Stack *stack, int row, int col)
{
  if (row == ROWS - 1 && col == COLS - 1) 
  { 
    // destination reached
    stack_push(stack, row, col);
    return 1;
  }

  if (canMove(row, col)) 
  {
    stack_push(stack, row, col);
    maze[row][col] = 2; // Marking visited

    // Move right
    if (solveMaze(stack, row, col + 1))
      return 1;

    // Move down
    if (solveMaze(stack, row + 1, col))
      return 1;

    // Move left
    if (solveMaze(stack, row, col - 1))
      return 1;

    // Move up
    if (solveMaze(stack, row - 1, col))
      return 1;

    // If none of the above movements work, backtrack
    stack_pop(stack);
    return 0;
  }

  return 0;
}

int main() 
{
  struct Stack stack;
  stack_init(&stack, ROWS*COLS);

  printf("This is the maze:\n");
  printMaze(maze);

  if (solveMaze(&stack, 0, 0)) 
  {
    printf("\n\n This is the path found:\n");
    while(!stack_is_empty(&stack)){
      struct Point p = stack_pop(&stack);
      printf("(%d, %d) ", p.row, p.col);
    }
    printf("\n\nThis is the maze with all the points crossed:\n");
    printMaze(maze);
  }
  else 
  {
    printf("No path found\n");
  }
  return 0;
}
