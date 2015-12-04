.PHONY: all clean
CXXFLAGS=-O2 -Wall
OBJS=main.o menu.o

all: menu

menu: $(OBJS)
	g++ -o menu $(OBJS) -lSDL2 -lSDL2_ttf

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp

menu.o: menu.cpp menu.hpp
	g++ $(CXXFLAGS) -c menu.cpp

clean:
	rm menu $(OBJS)

