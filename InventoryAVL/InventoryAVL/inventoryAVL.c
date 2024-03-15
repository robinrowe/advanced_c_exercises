#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Product
{
  int id;
  char name[50];
  float price;
  int quantity;
} Product;

typedef struct InventoryNode
{
  Product product;
  struct InventoryNode* left;
  struct InventoryNode* right;
  int height;
} InventoryNode;

int avl_get_height(InventoryNode* node)
{
  if (node == NULL)
    return 0;
  return node->height;
}

int avl_get_balance(InventoryNode* node)
{
  if (node == NULL)
    return 0;
  return avl_get_height(node->left) - avl_get_height(node->right);
}

InventoryNode* avl_new_node(Product product)
{
  InventoryNode* node = (InventoryNode*)malloc(sizeof(InventoryNode));
  if (node != NULL)
  {
    node->product = product;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
  }
  else
  {
    printf("Error allocating memory. Exiting...");
    exit(1);
  }
}

InventoryNode* avl_rotate_right(InventoryNode* y)
{
  printf("right rotate\n");
  InventoryNode* x = y->left;
  InventoryNode* T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(avl_get_height(y->left), avl_get_height(y->right)) + 1;
  x->height = max(avl_get_height(x->left), avl_get_height(x->right)) + 1;

  return x;
}

InventoryNode* avl_rotate_left(InventoryNode* x)
{
  printf("left rotate\n");
  InventoryNode* y = x->right;
  InventoryNode* T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(avl_get_height(x->left), avl_get_height(x->right)) + 1;
  y->height = max(avl_get_height(y->left), avl_get_height(y->right)) + 1;

  return y;
}

InventoryNode* avl_insert_node(InventoryNode* node, Product product)
{
  if (node == NULL)
    return avl_new_node(product);

  if (product.id < node->product.id)
    node->left = avl_insert_node(node->left, product);
  else if (product.id > node->product.id)
    node->right = avl_insert_node(node->right, product);
  else
    return node;

  node->height = 1 + max(avl_get_height(node->left), avl_get_height(node->right));

  int balance = avl_get_balance(node);

  if (balance > 1 && product.id < node->left->product.id)
    return avl_rotate_right(node);

  if (balance < -1 && product.id > node->right->product.id)
    return avl_rotate_left(node);

  if (balance > 1 && product.id > node->left->product.id)
  {
    node->left = avl_rotate_left(node->left);
    return avl_rotate_right(node);
  }

  if (balance < -1 && product.id < node->right->product.id)
  {
    node->right = avl_rotate_right(node->right);
    return avl_rotate_left(node);
  }

  return node;
}

void avl_traverse_tree(InventoryNode* root)
{
  if (root != NULL)
  {
    avl_traverse_tree(root->left);
    printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n", root->product.id, 
      root->product.name, root->product.price, root->product.quantity);
    avl_traverse_tree(root->right);
  }
}

InventoryNode* avl_search_product(InventoryNode* root, int id)
{
  if (root == NULL || root->product.id == id)
  {
    if (root == NULL)
      printf("Product not found.\n");
    else
      printf("Product found:\nID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
        root->product.id, root->product.name,
        root->product.price, root->product.quantity);
    return root;
  }

  printf("Visited product ID: %d\n", root->product.id);

  if (id < root->product.id)
    return avl_search_product(root->left, id);
  else
    return avl_search_product(root->right, id);
}

int main()
{
  InventoryNode* root = NULL;
  Product products[100];

  // Adding 100 random products
  srand((unsigned int)time(NULL));
  for (int i = 0; i < 100; i++)
  {
    products[i].id = i + 1;
    sprintf(products[i].name, "Product %d", products[i].id); 
    products[i].price = (float)(rand() % 1000) / 10.0;
    products[i].quantity = rand() % 100 + 1;
  }

  // Shuffle the array of products
  srand((unsigned int)time(NULL));
  for (int i = 99; i > 0; i--)
  {
    int j = rand() % (i + 1);
    Product temp = products[i];
    products[i] = products[j];
    products[j] = temp;
  }

  for (int i = 0; i < 100; i++)
  {
    root = avl_insert_node(root, products[i]);
  }

  // Print inventory
  printf("Inventory:\n");
  avl_traverse_tree(root);

  // Search for a product
  int productIdToSearch = 35;
  InventoryNode* foundProduct = avl_search_product(root, productIdToSearch);
  if (foundProduct != NULL)
  {
    printf("Product found:\n");
    printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
      foundProduct->product.id, foundProduct->product.name,
      foundProduct->product.price, foundProduct->product.quantity);
  }
  else
  {
    printf("Product with ID %d not found.\n", productIdToSearch);
  }
  return 0;
}
