.PHONY: all clean
OBJS=main.o menu.o

all: menu

menu: $(OBJS)
	g++ -o menu $(OBJS) -lSDL2 -lSDL2_ttf

main.o: main.cpp
	g++ -O2 -c main.cpp

menu.o: menu.cpp menu.hpp
	g++ -O2 -c menu.cpp

clean:
	rm menu $(OBJS)

