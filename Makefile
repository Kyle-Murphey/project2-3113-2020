all: project2

project2.o: project2.c
	gcc -c project2.c

project2: project2.o
	gcc project2.o -o project2

clean:
	rm -f *.o project2
