#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.hpp"

extern const int Screen_Width;
extern const int Screen_Height;

const int Rect_Width=300;
const int Rect_Height=100;

Menu::Menu(const char *items[], int n)
	: items(items), nItems(n) 
{
	rect = new SDL_Rect[n];
}

Menu::~Menu()
{
	delete[] rect;
}

void Menu::Init(SDL_Window *window)
{
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
	texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB555,SDL_TEXTUREACCESS_TARGET,Screen_Width,Screen_Height);

	for(int i=0;i<nItems;i++)
	{
		rect[i].x=0;
		rect[i].y=i*Rect_Height;
		rect[i].w=Rect_Width;
		rect[i].h=Rect_Height;
	}

	font = TTF_OpenFont("bay6.ttf",28);
}

void Menu::Quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

void Menu::Show()
{
	quit = false;
	selected = 0;
	while(!quit) {
		MsgLoop();
		Render();
	}
}

void Menu::MsgLoop()
{
	SDL_Event event;
	while(SDL_PollEvent(&event)!=0) {
		if(event.type==SDL_QUIT)
		{
			quit = true;
		}
		else if(event.type==SDL_MOUSEMOTION)
		{
			int x,y;
			SDL_GetMouseState(&x,&y);
			for(int i=0;i<nItems;i++)
			{
				if(y>=i*Rect_Height && y<=(i+1)*Rect_Height)
					selected=i;
			}
		}
		else if(event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_UP:
					selected--;
					break;
				case SDLK_DOWN:
					selected++;
					break;
			}
			if(selected<0)
				selected=nItems - 1;
			if(selected==nItems)
				selected=0;
		}
	}
}

void Menu::Render()
{
	SDL_SetRenderTarget(renderer,texture);
	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
	SDL_RenderClear(renderer);
	for(int i=0;i<nItems;i++)
	{
		SDL_RenderDrawRect(renderer,&rect[i]);
		SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderFillRect(renderer,&rect[i]);
	}
	SDL_RenderDrawRect(renderer,&rect[selected]);
	SDL_SetRenderDrawColor(renderer,0x00,0x00,0xFF,0xFF);
	SDL_RenderFillRect(renderer,&rect[selected]);

	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderCopy(renderer,texture,NULL,NULL);

	for(int i = 0; i < nItems; i++) {
		SDL_Texture *ttexture;
		SDL_Color color={0,0,0};
		SDL_Surface *surface =
			TTF_RenderText_Solid(font,items[i], color);
		ttexture=SDL_CreateTextureFromSurface(renderer,surface);
		SDL_FreeSurface(surface);

		SDL_Rect rect = { 0, Rect_Height * i, Rect_Width, Rect_Height };	
		SDL_RenderCopyEx(renderer,ttexture,NULL,&rect,0.0,NULL,SDL_FLIP_NONE);
		SDL_DestroyTexture(ttexture);
	}
	
	SDL_RenderPresent(renderer);
}
