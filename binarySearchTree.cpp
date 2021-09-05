/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
This is the binary search tree file, it is used to build binary search trees to find all variables within a C file.
*/
#include "binarySearchTree.h"//includes all function definitions
#include "symbolTableProcess.h"//includes all function definitions

using namespace std;

node::node() //default constructor
{
	ident;
	root;
	*left;
	*right;
}

node::node(identifier identifierName, node *left, node *right) //constructor taking an initial value
{
	ident = identifierName;
	root = identifierName;
	*left = *left;
	*right = *right;
}

node::~node() //destructor
{
	delete left;
	delete right;
}

//updates a nodes count if there is multiple references to the identifier
void update(node **tree, identifier structNode)//update function that updates count property of the node
{
	if (*tree != nullptr)
	{
		update(&((*tree)->left), structNode);//recursion to go through each left node of the tree
		if (((*tree)->ident).identName == structNode.identName)//checks if the trees identifier value is equal to the nodes identifier value
		{
			((*tree)->ident).count++;//increments counter
		}
		update(&((*tree)->right), structNode);//recursion to go through each right node of the tree
	}
}

//change insert so that it stores pointers to structs with data in
void insert (node **tree , identifier *nodes)
{
	if (*tree == nullptr)
	{
		*tree = new node;//creates new instance of node if it doesnt already exist
		(*tree)->ident = *nodes;//assigns values
		(*tree)->left = nullptr;//assigns values
		(*tree)->right = nullptr;//assigns values
	}
	else 
	{
		if (((nodes)->identName) < ((*tree)->ident).identName)//checks if value is less than the root node
		{
			insert(&((*tree)->left), nodes);//calls insert function with values
		}
		else if ((nodes)->identName > ((*tree)->ident).identName)//checks if value is greater than the root node
		{
			insert(&((*tree)->right), nodes);//calls insert function with values
		}
	}
}

//puts node in order and outputs them to console
void inorder (node *tree)
{
	if (tree != nullptr)//checks if tree isnt a null pointer
	{
		inorder(tree->left);//uses recursion to call itself to organise all left nodes
		cout << (tree->ident).identName;//outputs root node
		inorder(tree->right);//uses recursion to call itself to organise all right nodes
	}
}

//fills up the identifier struct to store all data from each line of symbol table
void fillStruct(string identName, int lineNum, string identType, string identReturnType, int count, node **tree)
{
	identifier nodes;//creates the struct named nodes
	identName = remove_spaces(identName);//assigns values to each property of nodes
	nodes.identName = identName;//assigns values to each property of nodes
	nodes.lineNum = lineNum;//assigns values to each property of nodes
	nodes.identType = identType;//assigns values to each property of nodes
	nodes.identReturnType = identReturnType;//assigns values to each property of nodes
	nodes.count = count;//assigns values to each property of nodes
	insert(&(*tree), &nodes);//inserts data into tree
	update(&(*tree), nodes);//updates the count property of the struct if its not the first occurence of the variable
}

//uses same theory as inorder function so reference through each node and use pointers to get struct and print data in order to file
void printToFile(node *tree)
{
	if (tree != nullptr)//checks if tree isnt a null pointer
	{
		ofstream outAllFile("identifiers.txt",ios_base::app);//opens file stream with the type to append on the end of the file
		printToFile(tree->left);//uses recursion to call itself to organise all left nodes
		outAllFile << (tree->ident).identName << " , " << (tree->ident).lineNum << " , " << (tree->ident).identType << " , " << (tree->ident).identReturnType << ", " << (tree->ident).count << endl;//outputs to file
		//cout << tree->ident;//outputs root node
		printToFile(tree->right);//uses recursion to call itself to organise all right nodes
		outAllFile.close();//closes file
	}
}