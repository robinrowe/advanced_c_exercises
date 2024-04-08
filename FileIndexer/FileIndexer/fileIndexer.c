#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef _MSC_VER
#define strdup(p) _strdup(p)
#endif

typedef struct TreeNode
{
  char* filename;
  char* location;
  struct TreeNode* left;
  struct TreeNode* right;
}TreeNode;

TreeNode* TreeNode_create(const char* filename,const char *location) 
{
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  if(node == NULL)
  {
    printf("Memory allocation error. Exiting..\n");
    exit(1);
  }
  node->filename = strdup(filename);
  node->location = strdup(location);
  node->left = node->right = NULL;
  return node;
}

TreeNode* TreeNode_insert(TreeNode* root,const char* filename,const char* location) 
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

void TreeNode_print(TreeNode* root)
{
  if (root != NULL)
  {
    TreeNode_print(root->left);
    printf("%s - (%s)\n", root->filename, root->location);
    TreeNode_print(root->right);
  }
}

void index_file_system(TreeNode** root, const char* path)
{
  WIN32_FIND_DATAA findData;
  HANDLE hFind;
  char searchPath[MAX_PATH];
  char subDirPath[MAX_PATH];

  // Create a search pattern --> get all files in the folder
  snprintf(searchPath, MAX_PATH, "%s\\*", path);
  // Find the first file in the directory
  hFind = FindFirstFileA(searchPath, &findData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    printf("Error: Could not open directory.\n");
    return;
  }

  // Iterate over files in the directory
  do
  {
    if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      *root = TreeNode_insert(*root, findData.cFileName, path);
    }
    else if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
    {
      snprintf(subDirPath, MAX_PATH, "%s\\%s", path, findData.cFileName);
      // Recursively index subdirectory
      index_file_system(root, subDirPath); 
    }
  } while (FindNextFileA(hFind, &findData) != 0);

  // Close the search handle
  FindClose(hFind);
}

// Search for a file in the BST
char* search_file_location(struct TreeNode* root, const char* filename) 
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

int main(int argc, char* argv[])
{
  char path[200];
  printf("Path to index recursively: ");
  fgets(path, 200, stdin);
  path[strcspn(path, "\n")] = '\0';

  TreeNode* root = NULL;

  // Index the file system
  index_file_system(&root, path);

  // Print indexed files
  printf("Indexed Files:\n");
  TreeNode_print(root);

  char filenameToSearch[200];
  printf("Let's search for a file's location. Give the file name: ");
  fgets(filenameToSearch, 200, stdin);
  filenameToSearch[strcspn(filenameToSearch, "\n")] = '\0';

  char* location = search_file_location(root, filenameToSearch);
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
