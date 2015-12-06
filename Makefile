.PHONY: all clean
CFLAGS=-O2 -Wall
OBJS=main.o menu.o

all: menu

menu: $(OBJS)
	gcc -o menu $(OBJS) -lSDL2 -lSDL2_ttf

main.o: main.c
	gcc $(CFLAGS) -c main.c

menu.o: menu.c menu.h
	gcc $(CFLAGS) -c menu.c

clean:
	rm menu $(OBJS)

