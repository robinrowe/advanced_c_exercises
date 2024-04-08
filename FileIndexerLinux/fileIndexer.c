#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#if 0
#ifdef _MSC_VER
#define strdup(p) _strdup(p)
#endif
#endif

typedef struct TreeNode
{
  char *filename;
  char *location;
  struct TreeNode *left;
  struct TreeNode *right;
} TreeNode;

TreeNode *TreeNode_create(const char *filename,const char *location)
{
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
  if (node == NULL)
  {
    printf("Memory allocation error. Exiting..\n");
    exit(1);
  }
  node->filename = strdup(filename);
  node->location = strdup(location);
  node->left = node->right = NULL;
  return node;
}

TreeNode *TreeNode_insert(TreeNode *root,const char *filename,const char *location)
{
  // If the tree is empty, return a new node
  if (root == NULL)
    return TreeNode_create(filename, location);

  // If not empty, then go down the tree
  if (strcmp(filename, root->filename) < 0)
    root->left = TreeNode_insert(root->left, filename, location);
  else if (strcmp(filename, root->filename) > 0)
    root->right = TreeNode_insert(root->right, filename, location);

  // Return the root pointer, it is used
  // by the lines above
  return root;
}

void TreeNode_print(TreeNode *root)
{
  if (root != NULL)
  {
    TreeNode_print(root->left);
    printf("%s - (%s)\n", root->filename, root->location);
    TreeNode_print(root->right);
  }
}

void index_file_system(struct TreeNode **root, const char *path)
{
  DIR *dir;
  struct dirent *entry;
  struct stat fileStat;

  // Open the directory
  dir = opendir(path);
  if (dir == NULL)
  {
    perror("opendir");
    return;
  }

  // Read directory entries
  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    {
      char fullPath[PATH_MAX];
      snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);

      // Check if it's a regular file
      if (stat(fullPath, &fileStat) == 0 && S_ISREG(fileStat.st_mode))
      {
        *root = TreeNode_insert(*root, entry->d_name, path);
      }
      else if (S_ISDIR(fileStat.st_mode))
      {
        index_file_system(root, fullPath); // Recursively index subdirectory
      }
    }
  }

  closedir(dir);
}

// Search for a file in the BST
char *search_file_location(struct TreeNode *root, const char *filename)
{
  // Traverse the tree until a match is found or the tree is exhausted
  while (root != NULL)
  {
    int cmp = strcmp(filename, root->filename);
    if (cmp == 0)
    {
      return root->location; // File found
    }
    else if (cmp < 0)
    {
      root = root->left; // Search in the left subtree
    }
    else
    {
      root = root->right; // Search in the right subtree
    }
  }
  return NULL; // File not found
}

int main(int argc, char *argv[])
{
  char path[200];
  printf("Path to index recursively: ");
  fgets(path, 200, stdin);
  path[strcspn(path, "\n")] = '\0';

  TreeNode *root = NULL;

  // Index the file system
  index_file_system(&root, path);

  // Print indexed files
  printf("Indexed Files:\n");
  TreeNode_print(root);

  char filenameToSearch[200];
  printf("Let's search for a file's location. Give the file name: ");
  fgets(filenameToSearch, 200, stdin);
  filenameToSearch[strcspn(filenameToSearch, "\n")] = '\0';

  char *location = search_file_location(root, filenameToSearch);
  if (location != NULL)
  {
    printf("File '%s' found. Location: %s\n", filenameToSearch, location);
  }
  else
  {
    printf("File '%s' not found.\n", filenameToSearch);
  }
  return 0;
}
