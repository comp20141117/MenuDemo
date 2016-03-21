#ifndef _MENU_HPP_
#define _MENU_HPP_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct _ITEM ITEM;

typedef struct _ITEM_LIST
{
	ITEM *items;
	int num_items;
}ITEM_LIST;

struct ITEM
{
	int id;
	const char *text;
	ITEM_LIST *sub_item_list;
};

typedef struct _MENU_CONFIG
{
	ITEM_LIST *item_list;
	const char *font_file;
	int font_size;
	SDL_Color color_highlight;
	SDL_Color color_background;
} MENU_CONFIG;

extern int show_menu(MENU_CONFIG *, SDL_Window *, int, int);
#endif
