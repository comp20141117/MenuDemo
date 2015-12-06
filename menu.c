#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

extern const int Screen_Width;
extern const int Screen_Height;

const int Rect_Width = 300;
const int Rect_Height = 100;

static int msg_loop(int num_items, int *p_selected)
{
	SDL_Event event;
	int selected = *p_selected;

	while(SDL_PollEvent(&event) != 0) {
		if(event.type == SDL_QUIT) {
			return 0;
		}
		else if(event.type == SDL_MOUSEMOTION) {
			int x, y, i;
			SDL_GetMouseState(&x, &y);
			for(i = 0; i < num_items; i++) {
				if(y >= i * Rect_Height && y <= (i + 1) * Rect_Height)
					selected = i;
			}
		}
		else if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
			case SDLK_UP:
				selected--;
				break;
			case SDLK_DOWN:
				selected++;
				break;
			}
			if(selected < 0) {
				selected = num_items - 1;
			}
			if(selected == num_items) {
				selected = 0;
			}
		}
	}
	*p_selected = selected;
	return 1;
}

static void render(
	MENU_CONFIG *config,
	TTF_Font *font, SDL_Renderer *renderer, SDL_Texture *texture,
	int selected, SDL_Rect *rect)
{
	int i;

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	for(i = 0; i < config->num_items; i++) {
		SDL_RenderDrawRect(renderer, &rect[i]);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &rect[i]);
	}
	SDL_RenderDrawRect(renderer, &rect[selected]);
	SDL_SetRenderDrawColor(
		renderer,
		config->color_highlight.r,
		config->color_highlight.g,
		config->color_highlight.b,
		config->color_highlight.a);
	SDL_RenderFillRect(renderer, &rect[selected]);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	for(i = 0; i < config->num_items; i++) {
		SDL_Texture *ttexture;
		SDL_Color color = { 0, 0, 0 };
		SDL_Surface *surface;
		SDL_Rect rect;
		
		surface = TTF_RenderText_Solid(font, config->items[i], color);
		ttexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		rect.x = 0;
		rect.y = Rect_Height * i;
		rect.w = Rect_Width;
		rect.h = Rect_Height;

		SDL_RenderCopyEx(
			renderer, ttexture, NULL, &rect,
			0.0, NULL,SDL_FLIP_NONE);
		SDL_DestroyTexture(ttexture);
	}
	
	SDL_RenderPresent(renderer);
}

int show_menu(MENU_CONFIG *config, SDL_Window *window, int x, int y)
{
	TTF_Font *font;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect *rect;
	int selected = 0;
	int i;

	font = TTF_OpenFont(config->font_file, config->font_size);
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	texture=SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_TARGET,
		Screen_Width, Screen_Height);

	rect = (SDL_Rect *)malloc(sizeof(SDL_Rect) * config->num_items);
	for(i = 0; i < config->num_items; i++) {
		rect[i].x = 0;
		rect[i].y = i*Rect_Height;
		rect[i].w = Rect_Width;
		rect[i].h = Rect_Height;
	}

	while(msg_loop(config->num_items, &selected)) {
		render(config, font, renderer, texture, selected, rect); 
	}

	free(rect);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	return selected;
}
