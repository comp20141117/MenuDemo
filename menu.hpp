#ifndef __MENU_HPP__
#define __MENU_HPP__
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Menu
{
	enum Rect
	{
		Rect_One=0,
		Rect_Two=1,
		Rect_Three=2,
		Rect_Four=3,
		Rect_Total=4
	};

	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Texture *ttexture;
	SDL_Rect rect[Rect_Total];

	TTF_Font *font;
	bool quit;
	int newrect;
	void second(std::string context,SDL_Color color);
	void MsgLoop();
	void Render();
public:
	void Init(SDL_Window *);
	void Quit();
	void Show();
};
#endif

