all: chat

chat: main.o app.o
	gcc main.o app.o -o chat

main.o: main.c app.h
	gcc -c main.c

app.o: app.c app.h
	gcc -c app.c

clean:
	rm -f a4 main.o app.o
