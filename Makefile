all: .o prog 

.o:
	gcc -c -Wall -ansi -pedantic ./src/*.c -lm  -std=gnu99 
	mv *.o ./bin

prog:
	gcc -Wall -ansi -pedantic ./bin/*.o -o ./bin/prog -lm

exec:
	./bin/prog
