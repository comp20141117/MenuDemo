#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include "menu.h"

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
