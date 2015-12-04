#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.hpp"

int Screen_Width=300;
int Screen_Height=500;

SDL_Window *window=NULL;

void PrintError(const char *msg)
{
	printf("%s SDL_Error:%s\n", msg, SDL_GetError());
}

void init()
{
	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		PrintError("init fail!");
	}
	window=SDL_CreateWindow("Menu",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,Screen_Width,Screen_Height,SDL_WINDOW_SHOWN);
	if(window==NULL)
	{
		PrintError("create window fail!");
	}
	if(TTF_Init()==-1)
	{
		PrintError("ttf init fail!");
	}
}

int main(int argc,char *argv[])
{
	init();
	
	const char *items[] = { "first", "second", "three", "four", "five"};
	Menu m(items, 5);
	m.Init(window);
	m.Show();
	m.Quit();

	SDL_DestroyWindow(window);
	window=NULL;

	TTF_Quit();
	SDL_Quit();
	return 0;
}
