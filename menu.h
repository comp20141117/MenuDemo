#ifndef _MENU_HPP_
#define _MENU_HPP_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct _MENU_CONFIG
{
	const char **items;
	int num_items;
	const char *font_file;
	int font_size;
	SDL_Color color_highlight;
	SDL_Color color_background;
} MENU_CONFIG;

extern int show_menu(MENU_CONFIG *, SDL_Window *, int, int);
#endif
