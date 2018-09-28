#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

// Local functions
static int place_bst_node( struct BST_Node *parent, struct BST_Node *node );
static struct BST_Node *make_bst_node( int key, void *data );
static struct BST_Node *min_node( struct BST_Node *root);

// Root's pointer is passed because root can get modified for the first node
int bst_add_node( struct BST_Node **root, int key, void *data )
{
	struct BST_Node *newnode = NULL;
	struct BST_Node *parent = NULL;
	struct BST_Node *retnode = NULL;
	int status = 0;

	newnode = make_bst_node( key, data);
	if( *root == NULL ){
		*root = newnode;
		status = BST_SUCCESS;
	}
	else{
		status = place_bst_node( *root, newnode );
	}
	return status;
}

struct BST_Node *bst_search( struct BST_Node *root, int key )
{
	struct BST_Node *retval = NULL;

	if(root == NULL)
		return	NULL;
	if(root->key == key)
		return root;
	else if(root->key < key)
		return bst_search(root->right_child, key);
	else if(root->key > key)
		return bst_search(root->left_child, key);
	else
		return retval;
	// TO-DO
}
void bst_print( struct BST_Node *root )
{
	if( root == NULL )
		return;
	else{
		printf("%d ", root->key);
		bst_print( root->left_child );
		bst_print( root->right_child );
	}
}

void bst_free( struct BST_Node *root )
{
	if( root == NULL )
		return;
	else{
		bst_free( root->left_child );
		bst_free( root->right_child );
		free(root);
	}
}

static int place_bst_node( struct BST_Node *parent, struct BST_Node *node )
{
	int retstatus;

	if( parent == NULL ){
		return BST_NULL;
	}
	else if( node->key == parent->key ){
		return BST_DUP_KEY;
	}
	else if(parent->left_child == NULL && (node->key < parent->key)){
			parent->left_child = node;
			return	BST_SUCCESS;
	}
	else if( parent->right_child == NULL && (node->key > parent -> key)){
			parent->right_child = node;
			return BST_SUCCESS;
	}
	else if( node->key < parent->key ){
		// TO-DO
		return place_bst_node(parent->left_child, node);
	}
	else if( node->key > parent->key ){
		// TO-DO
		return place_bst_node(parent->right_child, node);
	}
}

static struct BST_Node *make_bst_node( int key, void *data )
{
	struct BST_Node *newnode;
	newnode = (struct BST_Node *) malloc(sizeof(struct BST_Node));
	newnode->key = key;
	newnode->data = data;
	newnode->left_child = NULL;
	newnode->right_child = NULL;

	return newnode;
}

struct BST_Node *bst_delete(struct BST_Node *root, int key)
{
	if (root == NULL)
		return root;

	if (key < root->key)
		root->left_child = bst_delete(root->left_child, key);
	else if (key > root->key)
		root->right_child = bst_delete(root->right_child, key);
	else
	{
		if (root->left_child == NULL)
		{
			struct BST_Node *temp = root->right_child;
			free(root);
			return temp;
		}
		else if (root->right_child == NULL)
		{
			struct BST_Node *temp = root->left_child;
			free(root);
			return temp;
		}

		struct BST_Node *temp = min_node(root->right_child);
		root->key = temp->key;
		root->right_child = bst_delete(root->right_child, temp->key);
	}
	return root;
}

static struct BST_Node *min_node( struct BST_Node *node)
{
	while (node->left_child != NULL)
	{
		node = node->left_child;
	}

	return node;
}
