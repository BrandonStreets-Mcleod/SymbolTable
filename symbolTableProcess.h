/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
A C file information system - this takes an inputted C file and returns all variables and functions plus the number of times they are referenced
*/
#pragma once//includes all relevant and used libraries and uses pragma once to ensure they are only included once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

int countVariables (string line[100], int length);//defines the function to count total variables

int countFunctions (string line[100], int length);//defines the function to count total functions

int countIf (string line[100], int length);//defines the function to count total if statements

int countFor (string line[100], int length);//defines the function to count total for loops

int countWhile (string line[100], int length);//defines the function to count total while loops

void printToFile (string line[100], int length, int x, string stringType);//defines the function to print to file

void funcPrintToFile (string line[100], int length, int x, string stringType, string funcName);//defines the function to print functions to file

string remove_spaces(const string &string);//defines the function to remove spaces from strings

void outputIdentifier (string line[100], int length, string identName, int lineNum, string identType, string identReturnType);//defines the function to output results of other functions to file