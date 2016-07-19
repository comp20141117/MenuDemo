#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

extern const int Screen_Width = 600;
extern const int Screen_Height = 480;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		print_error("create renderer fail!");
		return 0;
	}

	return 1;
}

void clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

int main(int argc,char *argv[])
{
	if (!init())
		return 1;

	MenuItem items1[] = 
	{
		{ 101, "Open", nullptr },
		{ 102, "Save", nullptr },
		{ 103, "Exit", nullptr }
	};
	MenuItemList itemList1 = { 3, items1 };

	MenuItem items2[] =
	{
		{ 201, "Cut",   nullptr },
		{ 202, "Copy",  nullptr },
		{ 203, "Paste", nullptr }
	};
	MenuItemList itemList2 = { 3, items2 };

	MenuItem items31[] =
	{
		{ 399, "Feature AA", nullptr },
	};
	MenuItemList itemList31 = { 1, items31 };

	MenuItem items3[] =
	{
		{ 301, "Feature A", &itemList31 },
		{ 302, "Feature B", nullptr }
	};
	MenuItemList itemList3 = { 2, items3 };

	MenuItem items4[] =
	{
		{ 401, "Help",  nullptr },
		{ 402, "About", nullptr }
	};
	MenuItemList itemList4 = { 2, items4 };

	MenuItem items0[] =
	{
		{ 0, "File",    &itemList1 },
		{ 0, "Edit",    &itemList2 },
		{ 0, "Options", &itemList3 },
		{ 0, "Help",    &itemList4 },
	};
	MenuItemList itemList0 = { 4, items0 };

	SDL_Color black = { 0, 0, 0 ,255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color orange = { 255, 127, 39, 255 };

	Menu menu;
	menu.itemList = &itemList0;
	menu.fontFile = "bay6.ttf";
	menu.fontSize = 16;
	menu.fgColors[Menu::NORMAL] = orange;
	menu.fgColors[Menu::HIGHLIGHT] = black;
	menu.bgColors[Menu::NORMAL] = black;
	menu.bgColors[Menu::HIGHLIGHT] = orange;

	MenuExecutor e;
	e.init(renderer, &menu);

	bool quit = false;
	while (!quit) {
		SDL_Event ev;
		
		while (SDL_PollEvent(&ev)) {
			if (e.handleEvent(ev))
				continue;

			if (ev.type == SDL_QUIT) {
				return 0;
			} else if (ev.type == SDL_MOUSEBUTTONUP) {
				if (ev.button.button == SDL_BUTTON_RIGHT) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					e.activate(x, y);
				}
			}
		}

		if (e.hasResult()) {
			printf("Result: %d\n", e.popResult());
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		e.render();

		SDL_RenderPresent(renderer);

		SDL_Delay(20);
	}
	
	e.uninit();

	clean();

	return 0;
}
