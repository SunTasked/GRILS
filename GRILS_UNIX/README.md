# GRILS_UNIX

In order to build the C++ GRILS implementation, use the cmake tool to build the makefile.
This command will build the release version (gcc -O2)
>> cmake -DCMAKE_BUILD_TYPE=Release CMakeLists.txt
NB: on some systems, it will be necessary to run this command twice.

Then, build the program using the make command.
>> make

grils, the resulting binary file should be created in the current folder (GRILS_UNIX).

Manual of grils :
>> /PATH_TO_BINARY_FILE/grils [-t] [-i] /PATH_TO_TEST_FILE/testFile.txt
	- [-t] prints the turns content at the end of the execution
	- [-i] print each [insertion/removal] of vertices into the turns