#ifndef _MENU_HPP_
#define _MENU_HPP_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

typedef struct _ITEM ITEM;

typedef struct _ITEM_LIST
{
	ITEM *items;
	int num_items;
}ITEM_LIST;

struct _ITEM
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
	SDL_Color color_text;
        SDL_Color color_background;
} MENU_CONFIG;

extern int show_menu(MENU_CONFIG *, SDL_Window *, int, int);

struct MenuItem;

struct MenuItemList
{
	int numItems;
	MenuItem *items;
};

struct MenuItem
{
	int id;
	const char *text;
	MenuItemList *subItemList;
};

struct Menu
{
	enum MenuColorIndex { NORMAL, HIGHLIGHT, MAX_COLORS };

	MenuItemList *itemList;
	const char *fontFile;
	int fontSize;
	SDL_Color bgColors[MAX_COLORS];
	SDL_Color fgColors[MAX_COLORS];
};

struct MenuBox
{
	MenuItemList *itemList;
	SDL_Texture **unselected;
	SDL_Texture **selected;
	SDL_Rect rect;
	int sel;
};

class MenuExecutor
{
public:
	MenuExecutor() : m_font(nullptr), m_active(false) { }

	bool init(SDL_Renderer *renderer, Menu *menu);
	void uninit();

	void activate(int x, int y);
	void deactivate();
	bool isActive() { return m_active; }

	bool handleEvent(SDL_Event &e);
	void render();
	
	void initBox(MenuBox *box, MenuItemList *itemList, int x, int y);
	void destroyBox(MenuBox *box);
	void drawBox(MenuBox *box);
	void drawAllBoxes();
	void clearBoxes();

	void locateItemFromPoint(int x, int y, int *pBoxIndex, int *pItemIndex);

	bool hasResult() { return m_resultItemId >= 0; }
	int popResult() {
		int r = m_resultItemId;
		m_resultItemId = -1;
		return r;
	}

private:
	SDL_Renderer *m_renderer;
	Menu *m_menu;
	TTF_Font *m_font;

	std::vector<MenuBox *> m_boxes;
	int m_itemHeight;

	bool m_active;
	int m_resultItemId;
};

#endif
