#include <stdio.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH=300;
const int SCREEN_HEIGHT=400;

const int RECT_WIDTH=300;
const int RECT_HEIGHT=100;

enum RECT
{
	RECT_ONE=0,
	RECT_TWO=1,
	RECT_THREE=2,
	RECT_FOUR=3,
	RECT_TOTAL=4
};

int newrect=0;

int main(int argc,char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Event event;
	SDL_Rect rect[RECT_TOTAL];
	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("init fail!SDL_Error:%s\n",SDL_GetError());
	}
	window=SDL_CreateWindow("menu",0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_RESIZABLE);
	for(int i=0;i<RECT_TOTAL;i++)
	{
		rect[i].w=RECT_WIDTH;
		rect[i].h=RECT_HEIGHT;
	}
	renderer=SDL_CreateRenderer(window,-1,0);
	texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB555,SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);
	bool quit=false;
	while(!quit)
	{
		while(SDL_PollEvent(&event)!=0)
		{
			if(event.type==SDL_QUIT)
				quit=true;
			else if(event.type==SDL_MOUSEMOTION)
			{
				int x,y;
				SDL_GetMouseState(&x,&y);
				for(int i=0;i<RECT_TOTAL;i++)
				{
					if(y>=i*RECT_HEIGHT && y<=(i+1)*RECT_HEIGHT)
						newrect=i;
				}
			}
			else if(event.type==SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						newrect--;
						break;
					case SDLK_DOWN:
						newrect++;
						break;
				}
				if(newrect<0)
					newrect=3;
				if(newrect>3)
					newrect=0;
			}

		}
		for(int i=0;i<RECT_TOTAL;i++)
		{
			rect[i].x=0;
			rect[i].y=i*RECT_HEIGHT;
		}
		SDL_SetRenderTarget(renderer,texture);
		SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderClear(renderer);
		for(int i=0;i<RECT_TOTAL;i++)
		{
			SDL_RenderDrawRect(renderer,&rect[i]);
			SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
			SDL_RenderFillRect(renderer,&rect[i]);
		}
		SDL_RenderDrawRect(renderer,&rect[newrect]);
		SDL_SetRenderDrawColor(renderer,0x00,0x00,0xFF,0xFF);
		SDL_RenderFillRect(renderer,&rect[newrect]);
		

		SDL_SetRenderTarget(renderer,NULL);
		SDL_RenderCopy(renderer,texture,NULL,NULL);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
