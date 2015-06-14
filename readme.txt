// TEAM

// First team member name  : Robert Valladares

// Second team member name : Danish Waheed

	To compile and run the Virtual Machine lex.c, the user can
enter to a terminal window gcc lex.c -o extensionName to compile the .c file
then user can either specify a input file on the execution comand or allow the 
default input file name stand input.txt ./extensionName (optional) CustomInputFileName.txt
If user executes with an invalid input file name, the program will output
to the console an ERROR message and terminate. The default input file name is
input.txt
Once the program has finished running, it will create an output file with the
name cleaninput.txt, lexemelist.txt, lexemetable.txt and inform the user by printing a message to the
screen that it has finished. The output files will be located on the same folder as the lex.c
file.  If the program encounters an error along the process, it will terminate
and inform the user about the error and code line when possible.

To compile and execute program:
default input file name is input.txt
gcc lex.c -o extensionName
./extensionName (optional) CustomInputFileName.txt

output files are: 
cleaninput.txt
lexemelist.txt
lexemetable.txt
