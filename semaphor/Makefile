main: clean
	gcc main.c -ansi -pedantic -Wall -O0 -o main -lpthread

main_gdb: clean
	gcc main.c -ansi -pedantic -g -Wall -O0 -o main

clean:
	rm -rf main

run:
	./main
