#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include "menu.h"

extern const int Screen_Width;
extern const int Screen_Height;

const int Rect_Width = 100;
const int Rect_Height = 30;

int Rect_Pos_x = 0;
int Rect_Pos_y = 0;

int Show_Menu_Flag = 0;

int PointInRect(SDL_Point p, SDL_Rect r)
{
  	if(p.x >= r.x && p.x <= (r.x + r.w)){
	  	if(p.y >= r.y && p.y <=(r.y + r.h))
	    		return 1;
		else
		  	return 0;
	} else
	  	return 0;
}

int RenderFillRect(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color)
{
	SDL_RenderDrawRect(renderer, &rect); 
	SDL_SetRenderDrawColor(renderer, 
			       color.r, 
			       color.g, 
			       color.b, 
			       color.a); 
	SDL_RenderFillRect(renderer, &rect);
	return 1;
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
	SDL_SetRenderDrawColor(renderer,
			       config->color_background.r,
			       config->color_background.g,
			       config->color_background.b,
			       config->color_background.a);
	SDL_RenderClear(renderer);

	if(Show_Menu_Flag){
		for(i = 0; i < config->item_list->num_items; i++){ 
			rect[i].x = Rect_Pos_x; 
			rect[i].y = Rect_Pos_y + (i * Rect_Height); 
		} 

		for(i = 0; i < config->item_list->num_items; i++)
			RenderFillRect(renderer, rect[i], config->color_text);
		RenderFillRect(renderer, rect[selected], config->color_highlight);

		for(i = 0; i < config->item_list->num_items; i++){ 
			SDL_Texture *ttexture; 
			SDL_Color color = {0, 0, 0}; 
			SDL_Surface *surface; 
			SDL_Rect rect; 

			surface = TTF_RenderText_Solid(font, config->item_list->items[i].text, color); 
			ttexture = SDL_CreateTextureFromSurface(renderer, surface); 
			SDL_FreeSurface(surface); 

			rect.x = Rect_Pos_x; 
			rect.y = Rect_Pos_y + (Rect_Height * i); 
			TTF_SizeText(font, config->item_list->items[i].text, &rect.w, &rect.h); 

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
	rect = (SDL_Rect *)malloc(sizeof(SDL_Rect) * config->item_list->num_items);
	for(i = 0; i < config->item_list->num_items; i++){
		rect[i].x = Rect_Pos_x;
		rect[i].y = Rect_Pos_y + (i * Rect_Height);
		rect[i].w = Rect_Width;
		rect[i].h = Rect_Height;
	}

	while(msg_loop(config->item_list->num_items, &selected, rect)){
		render(config, font, renderer, texture, selected, rect);
	}
	free(rect);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	return selected;
}

bool MenuExecutor::init(SDL_Renderer *renderer, Menu *menu)
{
	m_renderer = renderer;
	m_menu = menu;

	m_font = TTF_OpenFont(m_menu->fontFile, m_menu->fontSize);
	if (m_font == nullptr)
		return false;

	m_itemHeight = TTF_FontHeight(m_font);

	return true;
}

void MenuExecutor::uninit()
{
	clearBoxes();

	m_renderer = nullptr;
	m_menu = nullptr;

	if (m_font != nullptr) {
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

void MenuExecutor::activate(int x, int y)
{
	MenuBox *box1 = new MenuBox;
	initBox(box1, m_menu->itemList, x, y);
	m_boxes.push_back(box1);

	m_resultItemId = -1;

	m_active = true;
}

void MenuExecutor::deactivate()
{
	clearBoxes();

	m_active = false;
}

bool MenuExecutor::handleEvent(SDL_Event &e)
{
	if (!m_active)
		return false;

	if (e.type == SDL_MOUSEMOTION) {
		int curBoxIndex = m_boxes.size() - 1;
		int curItemIndex = m_boxes[curBoxIndex]->sel;
		
		int newBoxIndex;
		int newItemIndex;

		int x, y;
		SDL_GetMouseState(&x, &y);
		locateItemFromPoint(x, y, &newBoxIndex, &newItemIndex);
		// printf("x: %d\ty = %d\tbi = %d\tni = %d\n", x, y, newBoxIndex, newItemIndex);

		if (newBoxIndex == -1) {
			m_boxes[curBoxIndex]->sel = -1;
			return true;
		}

		if (newBoxIndex == curBoxIndex && newItemIndex == curItemIndex)
			return true;

		while (newBoxIndex < curBoxIndex) {
			MenuBox *lastBox = m_boxes.back();
			destroyBox(lastBox);
			m_boxes.pop_back();
			curBoxIndex--;
		}
		
		MenuBox *newBox = m_boxes[newBoxIndex];
		MenuItem *newItem = &newBox->itemList->items[newItemIndex];
		if (newItem->subItemList != nullptr) {
			// pop up its sub menu
			MenuBox *box = new MenuBox;
			int x = newBox->rect.x + newBox->rect.w + 5;
			int y = newBox->rect.y + m_itemHeight * newItemIndex;
			initBox(box, newItem->subItemList, x, y);
			m_boxes.push_back(box);
		}

		newBox->sel = newItemIndex;

		return true;
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);

		int boxIndex, itemIndex;
		locateItemFromPoint(x, y, &boxIndex, &itemIndex);

		if (boxIndex < 0) {
			// at any time, a mouse button event happened outside will terminate menu
			deactivate();
		}
		else {
			// only if you release left mouse button on a menu item will end
			// the menu with a result
			if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				MenuItem *resultItem = &m_boxes[boxIndex]->itemList->items[itemIndex];
				if (resultItem->subItemList == nullptr) {
					m_resultItemId = resultItem->id;
					deactivate();
				}
			}
			return true;
		}
	}
	
	return false;
}

void MenuExecutor::render()
{
	if (!m_active)
		return;

	drawAllBoxes();
}

static SDL_Texture *RenderText(SDL_Renderer *renderer, TTF_Font *font, const char *str, SDL_Color color)
{
	SDL_Texture *result = nullptr;
	SDL_Surface *surface = TTF_RenderText_Blended(font, str, color);
	if (surface == nullptr) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		SDL_Texture	*texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			result = texture;
		}

		SDL_FreeSurface(surface);
	}
	
	return result;
}

void MenuExecutor::initBox(MenuBox *box, MenuItemList *itemList, int x, int y)
{
	int n = itemList->numItems;
	SDL_Texture **selected = new SDL_Texture *[n];
	SDL_Texture **unselected = new SDL_Texture *[n];
	int width = 0;
	int height = 0;

	for (int i = 0; i < n; i++) {
		selected[i] = RenderText(m_renderer, m_font, itemList->items[i].text, m_menu->fgColors[Menu::HIGHLIGHT]);
		unselected[i] = RenderText(m_renderer, m_font, itemList->items[i].text, m_menu->fgColors[Menu::NORMAL]);

		int w, h;
		SDL_QueryTexture(selected[i], nullptr, nullptr, &w, &h);
		if (w > width) width = w;
		height += h;
	}

	width += 20;

	SDL_Rect rect = { x, y, width, height };
	
	box->itemList = itemList;
	box->selected = selected;
	box->unselected = unselected;
	box->rect = rect;
	box->sel = -1;
}

void MenuExecutor::destroyBox(MenuBox * box)
{
	delete[] box->selected;
	delete[] box->unselected;
	delete box;
}

void MenuExecutor::drawBox(MenuBox *box)
{
	SDL_Color bgNormal = m_menu->bgColors[Menu::NORMAL];
	SDL_SetRenderDrawColor(m_renderer, bgNormal.r, bgNormal.g, bgNormal.b, bgNormal.a);
	SDL_RenderFillRect(m_renderer, &box->rect);

	if (box->sel >= 0) {
		SDL_Color bgSelected = m_menu->bgColors[Menu::HIGHLIGHT];
		SDL_SetRenderDrawColor(m_renderer, bgSelected.r, bgSelected.g, bgSelected.b, bgSelected.a);
		SDL_Rect selRect = {
			box->rect.x,
			box->rect.y + m_itemHeight * box->sel,
			box->rect.w,
			m_itemHeight
		};
		SDL_RenderFillRect(m_renderer, &selRect);
	}

	int numItems = box->itemList->numItems;
	SDL_Rect dstRect = { box->rect.x, box->rect.y };
	for (int i = 0; i < numItems; i++) {
		SDL_Texture *texture = (i == box->sel) ? box->selected[i] : box->unselected[i];
		SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
		SDL_RenderCopy(m_renderer, texture, nullptr, &dstRect);

		dstRect.y += m_itemHeight;
	}
}

void MenuExecutor::drawAllBoxes()
{
	for (auto iter = m_boxes.begin(); iter != m_boxes.end(); iter++)
		drawBox(*iter);
}

void MenuExecutor::clearBoxes()
{
	while (!m_boxes.empty()) {
		destroyBox(m_boxes.back());
		m_boxes.pop_back();
	}
}

static bool pointInRect(int x, int y, SDL_Rect &rect)
{
	return x >= rect.x && x < (rect.x + rect.w) && y >= rect.y && y < (rect.y + rect.h);
}

void MenuExecutor::locateItemFromPoint(int x, int y, int *pBoxIndex, int *pItemIndex)
{
	auto iterBox = std::find_if(
		m_boxes.begin(), m_boxes.end(),
		[=](auto pBox) { return pointInRect(x, y, pBox->rect); });

	if (iterBox == m_boxes.end()) {
		*pBoxIndex = -1;
		*pItemIndex = -1;
		return;
	}

	*pBoxIndex = iterBox - m_boxes.begin();
	*pItemIndex = (y - (*iterBox)->rect.y) / m_itemHeight;
}
