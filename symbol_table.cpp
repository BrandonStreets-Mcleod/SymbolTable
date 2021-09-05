/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
A C file information system - this takes an inputted C file and returns all variables and functions plus the number of times they are referenced
*/
#include "symbolTableProcess.h"//includes all function definitions

using namespace std;

int main (int argc, char **argv)
{
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
	int funcCounter = countFunctions(line, x);//calls function and assigns value to variable
	int varCounter = countVariables(line, x);//calls function and assigns value to variable
	cout << "The number of variables is " << varCounter << endl;//outputs to console
	cout << "The number of functions is " << funcCounter << endl;//outputs to console
	int ifCounter = countIf(line, x);//calls function and assigns value to variable
	cout << "The number of if statements is " << ifCounter << endl;//outputs to console
	int forCounter = countFor(line, x);//calls function and assigns value to variable
	cout << "The number of for loops is " << forCounter << endl;//outputs to console
	int whileCounter = countWhile(line, x);//calls function and assigns value to variable
	cout << "The number of while loops is " << whileCounter << endl;//outputs to console
}