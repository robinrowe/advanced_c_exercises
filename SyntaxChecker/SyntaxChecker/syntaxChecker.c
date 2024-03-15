#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct 
{
  char items[MAX_SIZE];
  int top;
} Stack;

void stack_init(Stack* s) 
{
  s->top = -1;
}

void stack_push(Stack* s, char c) 
{
  if (s->top == MAX_SIZE - 1) 
  {
    printf("Stack is full\n");
    exit(1);
  }
  s->items[++s->top] = c;
}

char stack_pop(Stack* s) 
{
  if (s->top == -1) 
  {
    printf("Stack is empty\n");
    exit(1);
  }
  return s->items[s->top--];
}

int stack_check_empty(Stack* s) 
{
  return s->top == -1;
}

int check_balanced(char* filename) 
{
  FILE* file = fopen(filename, "r");
  if (file == NULL) 
  {
    printf("Error opening file.\n");
    exit(1);
  }

  char c;
  Stack stack;
  stack_init(&stack);

  while ((c = fgetc(file)) != EOF) 
  {
    if (c == '(' || c == '[' || c == '{') 
    {
      stack_push(&stack, c);
    }
    else if (c == ')' || c == ']' || c == '}') 
    {
      // If stack is empty --> Return 'Unbalanced'
      if (stack_check_empty(&stack)) 
      {
        fclose(file);
        return 0; 
      }
      char openingChar = stack_pop(&stack);
      if ((c == ')' && openingChar != '(') ||
        (c == ']' && openingChar != '[') ||
        (c == '}' && openingChar != '{')) 
      {
        fclose(file);
        // If closing character doesn't match top of stack
        // --> return 'Unbalanced' 
        return 0; 
      }
    }
  }

  // If stack is empty, after we have finished 
  // checking the input file
  // --> return 'Balanced' 
  int result = stack_check_empty(&stack); 
  fclose(file);
  return result;
}

int main() 
{
  char filename[100];

  printf("Path to the source file: ");
  scanf("%s", filename);

  if (check_balanced(filename)) 
    printf("The input file is balanced.\n");
  else 
    printf("The input file is not balanced.\n");

  return 0;
}
