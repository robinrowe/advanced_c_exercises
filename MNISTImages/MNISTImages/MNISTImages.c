#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define IMAGE_SIZE 784    // Size of each image (28x28)
#define META_DATA_SIZE 15 // Size of meta data at the beginning of the file

// Structure to represent an image
typedef struct Image
{
  unsigned char pixels[IMAGE_SIZE]; // Array to store pixel values
  struct Image* next;               // Pointer to the next image in the linked list
} Image;

// Calculate Euclidean distance between two images
double euclideanDistance(Image* img1, Image* img2)
{
  double distance = 0.0;
  for (int i = 0; i < IMAGE_SIZE; i++)
  {
    distance += sqrt(pow((img1->pixels[i] - img2->pixels[i]), 2));
  }
  return sqrt(distance);
}

// Insert a new image into the linked list
void insertImage(Image** head, unsigned char pixels[])
{
  Image* newImage = (Image*)malloc(sizeof(Image));
  if (newImage == NULL)
  {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < IMAGE_SIZE; i++)
  {
    newImage->pixels[i] = pixels[i];
  }
  newImage->next = *head;
  *head = newImage;
}

// Free memory allocated for the linked list
void freeList(Image* head)
{
  Image* temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}

// Get a random image from the linked list
Image* getRandomImage(Image* head)
{
  int length = 0;
  Image* current = head;
  while (current != NULL)
  {
    length++;
    current = current->next;
  }
  if (length == 0)
  {
    printf("Empty list.\n");
    return NULL;
  }

  // Seed the random number generator
  srand(time(NULL));

  // Generate a random index within the range of the list length
  int randomIndex = rand() % length;
  printf("Random index: %d\n", randomIndex);

  // Traverse the list to the randomly chosen index
  current = head;
  for (int i = 0; i < randomIndex; i++)
  {
    current = current->next;
  }

  return current;
}

int main()
{
  FILE* file;
  char pixels[IMAGE_SIZE];
  Image* head = NULL;
  Image* tail = NULL;

  // Open the file
  file = fopen("input.dat", "rb"); // Open in binary mode to read exact bytes
  if (file == NULL)
  {
    printf("Error opening file.\n");
    exit(EXIT_FAILURE);
  }

  // Skip the meta data at the beginning of the file
  fseek(file, META_DATA_SIZE, SEEK_SET);

  int count = 0;
  // Read data from the file and insert images into the linked list
  while (fread(pixels, sizeof(unsigned char), IMAGE_SIZE, file) == IMAGE_SIZE)
  {
    insertImage(&head, pixels);
    if (ftell(file) % (IMAGE_SIZE * IMAGE_SIZE * sizeof(int)) == 0)
    {
      tail = head;
      while (tail->next != NULL)
      {
        tail = tail->next;
      }
    }
    count++;
  }

  printf("Total images: %d\n", count);

  fclose(file);

  // Example: Find the closest image to a randomly selected image
  Image* randomImage = getRandomImage(head); // Implement this function accordingly

  // Print selected image
  for (int i = 0; i < 28; i++)
  {
    for (int j = 0; j < 28; j++)
    {
      if (randomImage->pixels[28 * i + j] == 0)
        printf(" ");
      else
        printf("*");
    }
    printf("\n");
  }

  Image* closestImage = NULL;
  double minDistance = INFINITY;
  Image* current = head;
  int index = 0, minIndex = 0;

  while (current != NULL)
  {
    double distance = euclideanDistance(randomImage, current);
    if (distance != 0 && distance < minDistance)
    {
      minDistance = distance;
      minIndex = index;
      closestImage = current;
    }
    current = current->next;
    index++;
  }
  if(!closestImage)
  {  puts("closestImage failed");
     return 2;
  }
  // Output the label of the closest image
  printf("Closest image (distance=%f, index=%d)\n", minDistance, minIndex);

  // Print closest image
  for (int i = 0; i < 28; i++)
  {
    for (int j = 0; j < 28; j++)
    {
      if (closestImage->pixels[28 * i + j] == 0)
        printf(" ");
      else
        printf("*");
    }
    printf("\n");
  }

  // Free memory allocated for the linked list
  freeList(head);

  return 0;
}
