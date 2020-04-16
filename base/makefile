build:
	rm -f program.exe
	g++ -g -std=c++11 -Wall avl.o main.cpp util.cpp -o program.exe

catch:
	rm -f program.exe
	g++ -g -std=c++11 -Wall avl.o test.cpp util.cpp -o program.exe
	
run:
	./program.exe 

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./program.exe 

avl:
	g++ -c -std=c++11 -Wall avl.cpp