all: symbol_table 

symbol: 
	cl /EHsc symbol_table.cpp
	
compile: 
	cl /c /EHsc symbol_table.cpp symbolTableProcess.cpp 
	lib symbolTableProcess.obj
	link symbol_table.obj symbolTableProcess.lib 
	
compile2: 
	cl /c /EHsc symbol_table2.cpp symbolTableProcess2.cpp binarySearchTree.cpp
	lib symbolTableProcess2.obj
	lib binarySearchTree.obj
	link symbol_table2.obj symbolTableProcess2.lib binarySearchTree.lib

clean: 
	del *.obj
	del *.exe
	del *.lib