#ifndef __MENU_HPP__
#define __MENU_HPP__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Menu
{
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Texture *ttexture;
	SDL_Rect *rect;

	TTF_Font *font;
	bool quit;
	int selected;
	void MsgLoop();
	void Render();

	const char **items;
	const int nItems;
public:
	Menu(const char *[], int);
	~Menu();
	void Init(SDL_Window *);
	void Quit();
	void Show();
};
#endif

