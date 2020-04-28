Kyle Murphey, Project 1

Compile: make all or gcc -c project1.c -o project1

Run: ./project1 testfile.txt

Bugs: If the format of the file is off at all, then the program will fail. If the datatypes are off, then the program will fail.

I assume that all input will be correctly formatted and within range.

The main function opens the file, extracts the data from the instruction file, and stores them into a struct array. It then calls
the calculate() function which crunches the statistics for the output.


Sources:
	Linux man pages