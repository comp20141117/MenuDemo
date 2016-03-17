#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"

extern const int Screen_Width;
extern const int Screen_Height;

const int Rect_Width = 100;
const int Rect_Height = 30;

int Rect_Pos_x = 0;
int Rect_Pos_y = 0;

int Show_Menu_Flag = 0;

int PointInRect(const SDL_Point p, const SDL_Rect r)
{
  	if(p.x >= r.x && p.x <= (r.x + r.w)){
	  	if(p.y >= r.y && p.y <=(r.y + r.h))
	    		return 1;
		else
		  	return 0;
	} else
	  	return 0;
}

static int msg_loop(int num_items, int *p_selected, SDL_Rect *rect)
{
	SDL_Event event;
	int selected = *p_selected;
	while(SDL_PollEvent(&event) != 0){
		if(event.type == SDL_QUIT){
			return 0;
		}else if(event.type == SDL_MOUSEBUTTONUP){
			SDL_Point point;
			SDL_Rect rect;
			rect.x = Rect_Pos_x;
			rect.y = Rect_Pos_y;
			rect.w = Rect_Width;
			rect.h = Rect_Height * num_items;			
       			SDL_GetMouseState(&point.x, &point.y);		
			if(event.button.button == SDL_BUTTON_RIGHT){
				if(Show_Menu_Flag == 0){
      					Rect_Pos_x = point.x;
					Rect_Pos_y = point.y;
					Show_Menu_Flag = 1;
				}else{
					if(!PointInRect(point, rect))
						Show_Menu_Flag = 0;
				}			      
			}else if(event.button.button == SDL_BUTTON_LEFT){
				if(Show_Menu_Flag == 1 && !PointInRect(point, rect))
					Show_Menu_Flag = 0;
			}
		}else if(event.type == SDL_MOUSEMOTION){
			int i;
			SDL_Point point;
			SDL_GetMouseState(&point.x, &point.y);
			for(i = 0; i < num_items; i++){
				if(PointInRect(point, rect[i]))
					selected = i;
			}
  		}else if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
			case SDLK_UP:
				selected--;
				break;
			case SDLK_DOWN:
				selected++;
				break;
			}
			if(selected < 0){
				selected = num_items - 1;
			}
			if(selected == num_items){
				selected = 0;
			}
		}
	}
	*p_selected = selected;
	return 1;
}

static void render(MENU_CONFIG *config, TTF_Font *font,
		   SDL_Renderer *renderer, SDL_Texture *texture,
		   int selected, SDL_Rect *rect)
{
	int i;
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	if(Show_Menu_Flag){
		for(i = 0; i < config->num_items; i++){ 
			rect[i].x = Rect_Pos_x; 
			rect[i].y = Rect_Pos_y + (i * Rect_Height); 
		} 

		for(i = 0; i < config->num_items; i++){ 
			SDL_RenderDrawRect(renderer, &rect[i]); 
			SDL_SetRenderDrawColor(renderer, 
					       config->color_background.r, 
					       config->color_background.g, 
					       config->color_background.b, 
					       config->color_background.a); 
			SDL_RenderFillRect(renderer, &rect[i]); 
		} 
		SDL_RenderDrawRect(renderer, &rect[selected]); 
		SDL_SetRenderDrawColor(renderer, 
				       config->color_highlight.r, 
				       config->color_highlight.g, 
				       config->color_highlight.b, 
				       config->color_highlight.a); 
		SDL_RenderFillRect(renderer, &rect[selected]); 

		for(i = 0; i < config->num_items; i++){ 
			SDL_Texture *ttexture; 
			SDL_Color color = {0, 0, 0}; 
			SDL_Surface *surface; 
			SDL_Rect rect; 

			surface = TTF_RenderText_Solid(font, config->items[i], color); 
			ttexture = SDL_CreateTextureFromSurface(renderer, surface); 
			SDL_FreeSurface(surface); 

			rect.x = Rect_Pos_x; 
			rect.y = Rect_Pos_y + (Rect_Height * i); 
			TTF_SizeText(font, config->items[i], &rect.w, &rect.h); 

			SDL_RenderCopyEx(renderer, ttexture, NULL, &rect, 
					 0.0, NULL, SDL_FLIP_NONE); 
			SDL_DestroyTexture(ttexture); 
		}
	}
	SDL_SetRenderTarget(renderer, NULL); 
	SDL_RenderCopy(renderer,texture, NULL, NULL); 

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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB555,
				    SDL_TEXTUREACCESS_TARGET,
				    Screen_Width, Screen_Height);
	rect = (SDL_Rect *)malloc(sizeof(SDL_Rect) * config->num_items);
	for(i = 0; i < config->num_items; i++){
		rect[i].x = Rect_Pos_x;
		rect[i].y = Rect_Pos_y + (i * Rect_Height);
		rect[i].w = Rect_Width;
		rect[i].h = Rect_Height;
	}

	while(msg_loop(config->num_items, &selected, rect)){
		render(config, font, renderer, texture, selected, rect);
	}
	free(rect);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	return selected;
}
