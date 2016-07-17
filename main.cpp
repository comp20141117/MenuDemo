#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

extern const int Screen_Width = 600;
extern const int Screen_Height = 480;

SDL_Window *window = NULL;

void print_error(const char *msg)
{
	printf("%s SDL_Error:%s\n", msg, SDL_GetError());
}

int init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		print_error("init fall!");
		return 0;
	}

	window=SDL_CreateWindow(
		"menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);

	if(window == NULL){
		print_error("create window fail!");
		return 0;
	}

	if(TTF_Init() == -1){
		print_error("ttf init fail!");
		return 0;
	}

	return 1;
}

void clean()
{
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

int main(int argc,char *argv[])
{
  	ITEM items[] =
		{
			{0, "font", NULL},
			{1, "bg color", NULL},
			{2, "txt color", NULL},
			{3, "hl color", NULL},
			{4, "exit", NULL}
		};

	ITEM_LIST item_list = {items, 5};

	MENU_CONFIG config = {&item_list, "bay6.ttf", 24,
			      {0xFF, 0x66, 0x00, 0xFF},
			      {0xCC, 0xCC, 0xFF, 0xFF},
			      {0xFF, 0xFF, 0xFF, 0xFF}};

	if(!init()){
		return 1;
	}
	show_menu(&config, window, 0, 0);
	clean();
	return 0;
}	
