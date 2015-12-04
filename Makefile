.PHONY: all clean

all: menu

menu: main.cpp
	g++ -o menu main.cpp -lSDL2 -lSDL2_ttf

clean:
	rm menu

