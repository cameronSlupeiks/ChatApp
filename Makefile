all: a4

a4: a4.c
	gcc -o a4 a4.c

clean:
	rm -f a4 a4.o
