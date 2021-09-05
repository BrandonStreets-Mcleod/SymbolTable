/*
Author - Brandon Streets-McLeod
Last modified - 03/05/2021
A C file information system - this takes an inputted C file and returns all variables and functions plus the number of times they are referenced
*/
#include "symbolTableProcess.h"//includes the function definitions of all functions

using namespace std;

//countIf is used to count all if statements
int countIf (string line[100], int length)//function to find all occurences of If statements
{
	int ifCount = 0;
	int lineLength = length;
	for (int x = 0; x < lineLength; x++)//loops through each line
	{
		int found = line[x].find("if");//checks if the line has "if" on it
		if (found != string::npos)//if the line does have if then it breaks in
		{
			ifCount++;//increments the counter
		}
	}
	return ifCount;//returns counter
}

//countFor is used to count all for statements
int countFor (string line[100], int length)
{
	int forCount = 0;
	int lineLength = length;
	string identName;
	static const string stringArr[] = {"int ","short ","long long","long ","double ","char ","float ","void ", "char*"};//array of all possible prefixes for variables and functions
	int arrLength = 8, forFlag = 5;
	for (int x = 0; x < lineLength; x++)//loops through each line of file
	{
		int found = line[x].find("for");//checks for "for" occurences
		if (found != string::npos)//breaks in if true
		{
			forCount++;//increments counter
		}			
		for (int y = 0;y < arrLength;y++)//loops through each array reference
		{
			istringstream iss{line[x]};//creates a string stream with the array value x
			vector<string> stringSplit//vector to store each part of the line - "this is a test" would be stored as "this" "is" "a" "test"
			{
				istream_iterator<std::string>(iss),
				istream_iterator<std::string>()//iterates through the string, inserting each value into the string stream 'iss'
			};
			for (auto sepWord : stringSplit)//assigns the value of stringSplit to sepWord
			{
				if (sepWord.compare(")") == 0)//uses .compare to see if the variable is equal to the values ASCII value
				{
					forFlag = 5;
				}
				else if (sepWord.compare("for") == 0)
				{
					forFlag = 0;
				}
				else if (sepWord.compare("(") == 0 && forFlag == 0)
				{
					forFlag = 1;
				}
				else if (forFlag == 1)
				{
					sepWord = sepWord + " ";
					if (sepWord.compare(stringArr[y]) == 0)
					{
						forFlag = 2;
					}
				}
				else if (forFlag == 2)
				{
					int semiCPos = sepWord.find(";");//checks for semi-colons on the end of words
					string name = sepWord.substr(0,semiCPos);//removes the semi-colon if there
					identName.assign(name);
					forFlag = 0;
					outputIdentifier(line, lineLength, identName, x+1, "var", stringArr[y]);//calls function to output to file
				}
			}
		}
	}
	return forCount;//returns counter
}

//countWhile is used to count all while statements
int countWhile (string line[100], int length)
{
	int whileCount = 0;
	int lineLength = length;
	for (int x = 0; x < lineLength; x++)//loops through each line
	{
		int found = line[x].find("while"); 
		if (found != string::npos) 
		{
			whileCount++;//increments counter
		}
	}
	return whileCount;//returns counter
}

//remove_spaces is used to remove spaces from the end of variables
string remove_spaces(const string &string)
{
  int last = string.size() - 1;
  while (last >= 0 && string[last] == ' ')//loops through each character in the string looking for a space on the end
    --last;
  return string.substr(0, last + 1);//returns a new string with all the spaces on the end removed
}

//outputIdentifier is used to output identifiers to the identifiers.txt file
void outputIdentifier (string line[100], int length, string identName, int lineNum, string identType, string identReturnType)
{
	int fileLength = length;
	int numOfRef = 0, jump = 0, funcFound = 0, oBrackCount, returnJump = 0;
	string funcName;
	string name, arrName;
	string blackList;
	if (identType.compare("func") != 0)//checks if the identifier type is not a function
	{
		for (int x = lineNum-1; x < fileLength; x++)//loops through file
		{
			if (line[x].find("return ") != string::npos)//checks if the line contains a return statement
			{
				if (identName.find(funcName) == string::npos)//checks if the identifier name doesnt equal the function name
				{
					x = fileLength-1;//assigns the last line value to x 
				}
			}
			else if (identName.find("(") != string::npos && identName.find(")") != string::npos)//checks for brackets in the identifier name
			{
				int bracketPos = identName.find("(");
				name = identName.substr(0,bracketPos);//gets the name of the variable from the identifier name
				int endBracketPos = identName.length();
				funcName = identName.substr(bracketPos+1,endBracketPos-bracketPos-2);//gets function name from the identifier name
			}
			else if (identName.find("(") == string::npos && identName.find(")") == string::npos)//checks there isnt any brackets in the identifier name
			{
				int bracketPos = identName.find("[");
				name = identName.substr(0,bracketPos);//removes the square brackets from the end of the array type
				int endBracketPos = identName.length();
				arrName = identName.substr(bracketPos+1,endBracketPos-bracketPos-2);
				istringstream iss{line[x]};//creates strign stream from line[x]
				vector<string> stringSplit//creates a vector
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)//loops through vector
				{
					if (sepWord.compare(arrName) == 0)//checks that sepWord doesnt equal array name
					{
						numOfRef++;//increments counter
					}
				}
			}
			istringstream iss{line[x]};
			vector<string> stringSplit
			{
				istream_iterator<std::string>(iss),
				istream_iterator<std::string>()
			};
			if (line[x].find("return ") != string::npos || line[x].find("main") != string::npos)//checks if the line contains return or main
			{
				istringstream iss{line[x]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)//checks if sepWord is equal to the variable name
					{
						numOfRef++;//increments counter
					}
					else if (sepWord.compare("main") == 0)//checks if sepWord is equal to "main"
					{
						x = fileLength;//assigns fileLength to x
					}
				}
				x = fileLength;//assigns fileLength to x
			}
			else if (line[x].find(funcName) != string::npos && line[x].find("}") == string::npos && blackList.compare(name) != 0)//checks if the line has the function name on it and the name isnt on the blacklist already
			{
				if (line[x].find("{") != string::npos)//checks for curly bracket
				{
					oBrackCount++;//increments counter
				}
				else if (line[x].find("}") != string::npos)//checks for curly bracket
				{
					oBrackCount--;//decrements counter
				}
				jump = 1;
				funcFound = 1;
				for (auto sepWord : stringSplit)//loops through vector
				{
					name = remove_spaces(name);//removes spaces from the end of the variable
					if (sepWord.compare(name) == 0)//checks if sepWord is equal to name
					{
						numOfRef++;//increments counter
					}
				}
			}
			else if (line[x].find(funcName) == string::npos && line[x].find("}") == string::npos && blackList.compare(name) != 0)//checks if there is no function name on the line
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 1;
				funcFound = 1;
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)
					{
						numOfRef++;//increments counter
						blackList = name;//assigns name to blacklist
					}
				}
			}
			else if (line[x].find("main") != string::npos)//checks if line has main on it
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 1;
			}
			else if ((jump == 1 || funcFound == 1) && line[x].find("}") == string::npos)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)
					{
						numOfRef++;//increments counter
						blackList = name;//assigns name to blacklist
					}
				}
			}
			else if (line[x].find("}") == string::npos)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				istringstream iss{line[x+1]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					if (sepWord.compare("\n") == 0)//checks if line has a new line character on it
					{
						x = fileLength;//sets x to equal fileLength
					}
				}
			}
			else
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 0;
				funcFound = 0;
			}
		}
	}
	else if (identType.compare("func") == 0)//checks if the identifier type does equal function
	{
		for (int x = lineNum-1; x < fileLength; x++)
		{
			if (identName.find("(") != string::npos && identName.find(")") != string::npos)
			{
				int bracketPos = identName.find("(");
				name = identName.substr(0,bracketPos);
				int endBracketPos = identName.length();
				funcName = identName.substr(bracketPos+1,endBracketPos-bracketPos-2);
			}
			else if (identName.find("(") == string::npos && identName.find(")") == string::npos)
			{
				int bracketPos = identName.find("[");
				name = identName.substr(0,bracketPos);
				int endBracketPos = identName.length();
				arrName = identName.substr(bracketPos+1,endBracketPos-bracketPos-2);
				istringstream iss{line[x]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					if (sepWord.compare(arrName) == 0)
					{
						numOfRef++;
					}
				}
			}
			istringstream iss{line[x]};
			vector<string> stringSplit
			{
				istream_iterator<std::string>(iss),
				istream_iterator<std::string>()
			};
			if (line[x].find(funcName) != string::npos && line[x].find("}") == string::npos && blackList.compare(name) != 0)//checks there isnt a function name on the line
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 1;
				funcFound = 1;
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)
					{
						numOfRef++;
					}
				}
			}
			else if (line[x].find(funcName) == string::npos && line[x].find("}") == string::npos && blackList.compare(name) != 0)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 1;
				funcFound = 1;
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)
					{
						numOfRef++;
						blackList = name;
					}
				}
			}
			else if (line[x].find("main") != string::npos)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 1;
			}
			else if ((jump == 1 || funcFound == 1) && line[x].find("}") == string::npos)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				for (auto sepWord : stringSplit)
				{
					name = remove_spaces(name);
					if (sepWord.compare(name) == 0)
					{
						numOfRef++;
						blackList = name;
					}
				}
			}
			else if (line[x].find("}") == string::npos)
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				istringstream iss{line[x+1]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					if (sepWord.compare("\n") == 0)
					{
						x = fileLength;
					}
				}
			}
			else
			{
				if (line[x].find("{") != string::npos)
				{
					oBrackCount++;
				}
				else if (line[x].find("}") != string::npos)
				{
					oBrackCount--;
				}
				jump = 0;
				funcFound = 0;
			}
		}
	}
	ofstream outFile("identifiers.txt",ios_base::app);//opens file stream with the type to append on the end of the file
	outFile << identName << " , " << lineNum << " , " << identType << " , " << identReturnType << ", " << numOfRef-1 << endl;//outputs to file
	outFile.close();//closes file
}

//printToFile is used to get names of variables and add their function to the end of them if they are function definitions
void printToFile (string line[100], int length, int x, string stringType)
{
	int lineLength = length;
	string identName;
	int flag = 0, count = 0;
	string returnType = stringType;
	istringstream iss{line[x]};
	vector<string> stringSplit
	{
		istream_iterator<std::string>(iss),
		istream_iterator<std::string>()
	};
	for (auto sepWord : stringSplit)
	{
		if (line[x].find("(") == string::npos)//checks there is no brackets
		{
			if (sepWord.compare(returnType) != 0 && flag == 0)//checks if sepWord and the return type are the same
			{
				flag = 1;
				if (returnType.compare("long long") == 0 && count == 0)//checks if return type is long long
				{
					flag = 0;
					count = 1;
				}
			}
			else if (flag == 1)
			{
				int semiCPos = sepWord.find(";");
				string name = sepWord.substr(0,semiCPos);//removes semi-colon
				identName.assign(name);
				flag = 2;
				if (line[x].find("[") != string::npos)//checks for square brackets
				{
					string arrName = identName + "[]";
					outputIdentifier(line, lineLength, arrName, x+1, "arr", stringType);//calls function with correct parameters
				}
				else if (line[x].find("main") == string::npos)
				{
					outputIdentifier(line, lineLength, identName, x+1, "var", stringType);//calls function with correct parameters
				}
			}
		}
	}
}

//prints functions to file
void funcPrintToFile (string line[100], int length, int x, string stringType, string funcName)
{
	int lineLength = length;
	string identName;
	int flag = 0, count = 0, jump = 0;
	string returnType = stringType;
	istringstream iss{line[x]};
	vector<string> stringSplit
	{
		istream_iterator<std::string>(iss),
		istream_iterator<std::string>()
	};
	for (auto sepWord : stringSplit)
	{
		if (funcName.compare("main") == 0)
		{
			if (sepWord.compare(returnType) == 0)
			{
				flag = 3;
			}
			else if (flag == 3)
			{
				string fullName = sepWord;
				flag = 0;
				outputIdentifier(line, lineLength, fullName, x+1, "var", stringType);
			}
		}
		else if (sepWord.compare(returnType) == 0 && flag == 0)
		{
			flag = 1;
			if (returnType.compare("long long") == 0 && count == 0)
			{
				flag = 0;
				count = 1;
			}
		}
		else if (sepWord.compare(returnType) != 0 && flag == 0)
		{
			flag = 1;
			if (returnType.compare("long long") == 0 && count == 0)
			{
				flag = 0;
				count = 1;
			}
		}
		else if (flag == 0 && sepWord.compare("for ") != 0)
		{
			if (sepWord.compare("(") != 0)
			{
				flag = 2;
			}
		}
		else if (flag == 1 && sepWord.compare("main ") != 0)
		{
			if (sepWord.compare("(") != 0)
			{
				flag = 2;
			}
		}
		else if (flag == 2)
		{
			if (sepWord.compare("(") == 0 || sepWord.compare(")") == 0)
			{
				jump = 1;
			}
			else if (jump == 1)
			{
				jump = 2;
			}
			else if (jump == 2)
			{	
				if (sepWord.compare(returnType) != 0)
				{
					int semiCPos = sepWord.find(";");
					string name = sepWord.substr(0,semiCPos);
					identName.assign(name);
					string fullName = identName + "(" + funcName + ")";
					flag = 0;
					outputIdentifier(line, lineLength, fullName, x+1, "var", stringType);
				}
			}
		}
		else 
		{
			jump = 1;
		}
	}
}

//countFunctions is used to count total number of functions
int countFunctions (string line[100], int length)
{
	int lineLength = length;
	static const string stringArr[] = {"int ","short ","long long","long ","double ","char ","float ","void ", "char*"};
	int arrLength = 8, funcCount = 0, delimitLength = 3;
	string identName, funcName;
	int flag = 0, inFunc = 0, forFlag;
	for (int x = 0;x < lineLength;x++)//loops through file
	{
		for (int y = 0;y < arrLength;y++)//loops through array of prefixes
		{
			flag = 0;
			int found = line[x].find(stringArr[y]); 
			int found2 = line[x].find("(");
			int found3 = line[x].find("{");
			int found4 = line[x].find("="); 
			int found5 = line[x].find("}");
			int found6 = line[x].find("main");
			if (found6 != string::npos)
			{
				identName.assign("main");
				funcCount++;
				outputIdentifier(line, lineLength, identName, x+1, "func", stringArr[y]);//calls function with correct parameters
				funcPrintToFile (line, lineLength, x, stringArr[y], identName);//calls function with correct parameters
				x = lineLength;
			}
			else if (found != string::npos && found2 != string::npos && found4 == string::npos) 
			{
				funcCount++;
				istringstream iss{line[x]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					if (sepWord.compare(stringArr[y]) != 0 && flag == 0)//checks if sepWord is equal to the value in stringArr[y]
					{
						flag = 1;
					}
					else if (flag == 1)
					{
						identName.assign(sepWord);
						flag = 2;
						outputIdentifier(line, lineLength, identName, x+1, "func", stringArr[y]);//calls function with correct parameters
						funcPrintToFile (line, lineLength, x, stringArr[y], identName);//calls function with correct parameters
					}
				}
			}
			else if (found3 != string::npos)
			{
				istringstream iss{line[x+1]};
				vector<string> stringSplit
				{
					istream_iterator<std::string>(iss),
					istream_iterator<std::string>()
				};
				for (auto sepWord : stringSplit)
				{
					sepWord = sepWord + " ";
					if (sepWord.compare(stringArr[y]) == 0 && flag == 0)
					{
						flag = 1;
					}
					else if (flag == 1)
					{
						funcName.assign(identName);
						int semiCPos = sepWord.find(";");
						string name = sepWord.substr(0,semiCPos);
						identName.assign(name);
						string fullName = identName + "(" + funcName + ")";
						flag = 0;
						outputIdentifier(line, lineLength, fullName, x+2, "var", stringArr[y]);//calls function with correct parameters
					}
				}
			}
		}
	}
	return funcCount;//returns counter
}

//countVariables counts the total number of variables in a file
int countVariables (string line[100], int length)
{
	int lineLength = length;
	int varCount = 0, funcCount = 0;
	static const string stringArr[] = {"int","int*","short","long","double","char","char*","long*","float","void"};
	static const string stringArr2[] = {"int ","int* ","short ","long ","double ","char ","char* ","long* ","float ","void "};
	int arrLength = 10, found2 = 0;
	string identName;
	int flag = 0, allFound = 0, mainFound = 0;
	for (int x = 0;x < lineLength;x++)
	{
		for (int y = 0;y < arrLength;y++)
		{
			int found = line[x].find(stringArr2[y]);  
			if (found != string::npos) 
			{
				int found2 = line[x].find("main", found+1); 
				int found3 = line[x].find("long ", found+1); 
				if (found2 != string::npos)
				{
					mainFound = 1;
				}
				else if (found3 == string::npos && mainFound == 1)
				{
					if (line[x].find("(", found) == string::npos)
					{
						found = line[x].find(stringArr2[y], found+1);  
						if (found != string::npos)
						{
							for (int z = 0; z < arrLength;z++)
							{
								while (found != string::npos)
								{
									varCount++;
									printToFile (line, lineLength, x, stringArr2[y]);//calls function with correct parameters
									found = line[x].find(stringArr2[z], found+1); 
								} 
							}
						}
						else
						{
							for (int z = 0; z < arrLength;z++)
							{
								found = line[x].find(stringArr2[z], found+1); 
								while (found != string::npos && line[x].find("(", found+1) == string::npos)
								{
									varCount++;
									printToFile (line, lineLength, x, stringArr2[y]);
									found = line[x].find(stringArr2[z], found+1); 
								} 
							}
						}
					}
					else 
					{
						varCount++;
						funcPrintToFile (line, lineLength, x, stringArr[y], "main");//calls function with correct parameters
					}
				}
				else if (mainFound == 0)
				{
					found = line[x].find(stringArr[y], found+1);  
					if (found != string::npos)
					{
						for (int z = 0; z < arrLength;z++)
						{
							while (found != string::npos)
							{
								varCount++;
								printToFile (line, lineLength, x, stringArr[y]);//calls function with correct parameters
								found = line[x].find(stringArr[z], found+1); 
							} 
						}
					}
					else
					{
						for (int z = 0; z < arrLength;z++)
						{
							found = line[x].find(stringArr2[z]); 
							found2 = line[x].find("(", found+1);
							if (found != string::npos && found2 != string::npos)
							{
								istringstream iss{line[x]};
								vector<string> stringSplit
								{
									istream_iterator<std::string>(iss),
									istream_iterator<std::string>()
								};
								for (auto sepWord : stringSplit)
								{
									if (sepWord.compare("(") == 0)
									{
										flag = 1;
									}
									else if (sepWord.compare("(") != 0 && flag == 0 && line[x].find("(") == string::npos)
									{
										if (sepWord.compare(stringArr[z]) == 0)
										{
											varCount++;
											printToFile(line, lineLength, x, stringArr[y]);//calls function with correct parameters
											flag = 0;
										}
									}
									else if (flag == 1 && sepWord.compare(stringArr[z]) == 0)
									{
										flag = 2;
									}
									else if (flag == 2)
									{
										varCount++;
										printToFile(line, lineLength, x, stringArr[y]);
										flag = 0;
									}
								}
							}
							else if (found != string::npos && found2 == string::npos)
							{
								istringstream iss{line[x]};
								vector<string> stringSplit
								{
									istream_iterator<std::string>(iss),
									istream_iterator<std::string>()
								};
								for (auto sepWord : stringSplit)
								{
									if (sepWord.compare(stringArr2[z]) == 0)
									{
										flag = 1;
									}
									else if (flag == 1)
									{
										varCount++;
										printToFile (line, lineLength, x, stringArr2[y]);//calls function with correct parameters
										found = line[x].find(stringArr2[z], found+1); 
										flag = 0;
									}
								}
							}
						}
					}
				}
				else if (found3 != string::npos)
				{
					varCount++;
					printToFile (line, lineLength, x, "long long");//calls function with correct parameters
				}
				else if (line[x].find("(", found) == string::npos)
				{
					found = line[x].find(stringArr[y], found+1); 
					if (found != string::npos)
					{
						varCount++;
						printToFile (line, lineLength, x, stringArr[y]);//calls function with correct parameters
					}
				}
			}
		}
	}
	return varCount;//returns counter
}