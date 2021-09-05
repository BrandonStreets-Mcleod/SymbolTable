/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
A C file information system - this takes an inputted C file and returns all variables and functions to be used within a binary search tree
*/
#include "symbolTableProcess.h"//includes all function definitions
#include "binarySearchTree.h"//includes all function definitions

using namespace std;

int main (int argc, char **argv)
{
	node *tree = nullptr; // Declare the tree - nullptr to a node
	char* file = argv[1];
	string line[100];
	int x = 0;
	ifstream openFile(file);//opens the in file stream
	while (getline(openFile, line[x]))//loops through the file until it runs out of lines
	{
		x++;//increments the value
	}
	openFile.close();//closes file stream
	ofstream outFile("identifiers.txt");//opens file stream to "identifiers.txt"
	outFile << "Identifier Name , " << "Line number , " << "var/arr/func , " << "Identifier type , " << "Number of times referenced" << endl;//writes to file
	outFile.close();//closes file stream
	int funcCounter = countFunctions(line, x, &tree);//calls function and assigns value to variable
	int varCounter = countVariables(line, x, &tree);//calls function and assigns value to variable
	inorder (&(*tree));//sorts tree so all nodes are in order
	printToFile(&(*tree));
	cout << endl;//prints an end line
	return 0;
}