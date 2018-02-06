#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct node {

	int val;
	struct node* left;
	struct node* right;
};

struct btree {

	struct node* root;
};

struct node* insertNode(struct node* Nd, int val)
{
		if (Nd == NULL)
		{
				Nd = malloc(sizeof(struct node));
				Nd->val = val;
				Nd->left = NULL;
				Nd->right = NULL;
		}
		else
		{
			if (Nd->val > val)
			{
					Nd->left = insertNode(Nd->left, val);
			}
			else
			{
					Nd->left = insertNode(Nd->right, val);
			}
		}
		return Nd;
}

void insert(struct btree* tree, int val) {

		// TODO: Implement insertion.
		tree->root = insertNode(tree->root, val);
}


struct btree* populate(int size) {

	// DO NOT CHANGE OR REUSE THIS LINE!
	srand(time(NULL));

	// TODO: Insert 'size' number of random nodes.
	// Obtain random number using rand()

  struct btree* tree = malloc(sizeof(struct btree));
	tree->root = NULL;

  int idx = 0;
  for (; idx < size; idx++)
  {
      insert(tree, rand()%100);
  }
	return tree;
}

void printTree(struct node* Nd)
{
    if (Nd == NULL)
    {
        return;
    }
    else
    {
				printf(" Val %d\n", Nd->val);
				printf("L ");
				printTree(Nd->left);
				printf("R ");
				printTree(Nd->right);
				printf("\n");
    }
}

void freeTree(struct node* Nd)
{
    if (Nd == NULL)
    {
        return;
    }
    else
    {
        freeTree(Nd->left);
        freeTree(Nd->right);
        printf("Freeing %d\n", Nd->val);
        free(Nd);
    }
}

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Invalid number of args");
		exit(1);
	}

	int size = atoi(argv[1]);

	if (size < 1) {

		printf("Size must be > 0");
		exit(1);
	}

	// Randomly populate a linked list.
	struct btree* tree = populate(size);
  // printTree(tree->root);
  // freeTree(tree->root);
	return 0;
}
