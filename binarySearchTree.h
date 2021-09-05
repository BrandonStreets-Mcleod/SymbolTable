/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
This is the binary search tree file, it is used to build binary search trees to find all variables within a C file.
*/

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

// The node of a tree
struct identifier//struct to store all data about each node
{
	string identName;
	int lineNum;
	string identType;
	string identReturnType;
	int count;
};

class node//defines class node that contains all values and functions that can be applied to objects of that type
{
	public:
		node(); //default constructor
        node(identifier ident, node *left, node *right); //constructor taking an initial values
        ~node(); //destructor*
		identifier ident;
		node *left;
		node *right;
		void update(node *tree, identifier structNode);//definition for update function

	private:
		identifier root;
};

void inorder (node *tree);//definition for inorder function

void insert (node **tree, identifier *nodes);//definition for insert function

void fillStruct(string identName, int lineNum, string identType, string identReturnType, int count, node **tree);//definition for fillStruct function

void printToFile(node *tree);

int countVariables (string line[100], int length, node **tree);//defines the function to count total variables

int countFunctions (string line[100], int length, node **tree);//defines the function to count total functions