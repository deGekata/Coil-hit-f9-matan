

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
typedef int Data;
 
struct Node
{
	Data val;
	struct Node* left;
	struct Node* right;
};
 
struct Node * tree_add (struct Node * tree, Data x);
 
void tree_print (struct Node * tree);
 
void tree_destroy (struct Node * tree);
 
 
 
 
struct Node * tree_add (struct Node * tree, Data x)
{
	if(!tree)
	{                                                      
         struct Node* t = (struct Node*)calloc(1, sizeof(struct Node));  
         t->right = NULL;                         
         t->left = NULL;  
		 t->val = x;
	     return t;
	}
 
	if (x > tree->val)
   	{
		tree->right = tree_add(tree->right, x);
	}
	if (x < tree->val)
	{
		tree->left = tree_add(tree->left, x);
	}
 
	return tree;
}
 
 
 
void tree_print (struct Node * tree)
{
	if (tree->left)
	{
		tree_print(tree->left);
		printf(" ");
	}
	printf("%d", tree->val);
	if (tree->right)
	{
		printf(" ");
		tree_print(tree->right);
	}
}
 
void tree_destroy (struct Node* tree)
{
	if (tree)
	{
		tree_destroy(tree->right);
		tree_destroy(tree->left);
		free(tree);
	}
}	
 
 
int main(void)
{
	struct Node * tree=NULL;
	tree = tree_add(tree, 7);
	tree = tree_add(tree, 3);
	tree = tree_add(tree, 2);
	tree = tree_add(tree, 1);
	tree = tree_add(tree, 9);
	tree = tree_add(tree, 5);
	tree = tree_add(tree, 4);
	tree = tree_add(tree, 6);
	tree = tree_add(tree, 8);
	tree_print(tree);
	tree_destroy(tree);
	return 0;
}
// #include <signal.h>
// #include "stdio.h"
// #include "stdlib.h"

// void recursion(int p) {
//     if (p == 0) {
//         free((int*)15);
//     }
//     printf("%d\n", p);
//     recursion(p - 1);

// }


// void termination_handler (int signum) {
//   struct temp_file *p;

// //   for (p = temp_file_list; p; p = p->next)
// //     unlink (p->name);
//     printf("lol");
//     exit(0);
// }

// int main (void) {


//     if (({int a = 10;}) == 10) {
//         printf("Yes %d ", a);
//     } else {
//         printf("NO %d", a);
//     }

//     // struct sigaction new_action, old_action; dd

//     /* Set up the structure to specify the new action. */
//     // new_action.sa_handler = termination_handler; dd
//     // sigemptyset (&new_action.sa_mask); dd
//     // new_action.sa_flags = 0;dd

//     // sigaction (SIGINT, NULL, &old_action);dd

//     // sigaction (SIGSEGV, &new_action, &old_action);dd
//     // free((int*)15);
//     // recursion(20); dd

//     // if (old_action.sa_handler != SIG_IGN)
//     // sigaction (SIGINT, &new_action, NULL);
//     // sigaction (SIGHUP, NULL, &old_action);
//     // if (old_action.sa_handler != SIG_IGN)
//     // sigaction (SIGHUP, &new_action, NULL);
//     // sigaction (SIGTERM, NULL, &old_action);
//     // if (old_action.sa_handler != SIG_IGN)
//     // sigaction (SIGTERM, &new_action, NULL);
//     return 0;
// }


