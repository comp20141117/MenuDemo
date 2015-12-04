#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int Screen_Width=300;
const int Screen_Height=400;

const int Rect_Width=300;
const int Rect_Height=100;

enum Rect
{
	Rect_One=0,
	Rect_Two=1,
	Rect_Three=2,
	Rect_Four=3,
	Rect_Total=4
};

int newrect=0;
SDL_Window *window=NULL;
SDL_Renderer *renderer=NULL;
SDL_Texture *texture=NULL;
SDL_Rect rect[Rect_Total];
TTF_Font *font=NULL;
SDL_Texture *ttexture=NULL;

void init()
{
	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("init fail! SDL_Error:%s\n",SDL_GetError());
	}
	window=SDL_CreateWindow("Menu",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,Screen_Width,Screen_Height,SDL_WINDOW_SHOWN);
	if(window==NULL)
	{
		printf("create window fail SDL_Error:%s\n",SDL_GetError());
	}
		for(int i=0;i<Rect_Total;i++)
	{
		rect[i].w=Rect_Width;
		rect[i].h=Rect_Height;
	}
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
	texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB555,SDL_TEXTUREACCESS_TARGET,Screen_Width,Screen_Height);
	if(TTF_Init()==-1)
	{
		printf("ttf init fail:SDL_Error:%s\n",SDL_GetError());
	}
}

void second(std::string context,SDL_Color color)
{
	if(ttexture!=NULL)
		SDL_DestroyTexture(ttexture);
	SDL_Surface *surface=TTF_RenderText_Solid(font,context.c_str(),color);
	ttexture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
}

int main(int argc,char *argv[])
{
	init();
	font=TTF_OpenFont("bay6.ttf",28);
	SDL_Color color={0,0,0};
	SDL_Event event;
	bool quit=false;
	while(!quit)
	{
		while(SDL_PollEvent(&event)!=0)
		{
			if(event.type==SDL_QUIT)
			{
				quit=true;
			}
			else if(event.type==SDL_MOUSEMOTION)
			{
				int x,y;
				SDL_GetMouseState(&x,&y);
				for(int i=0;i<Rect_Total;i++)
				{
					if(y>=i*Rect_Height && y<=(i+1)*Rect_Height)
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
		for(int i=0;i<Rect_Total;i++)
		{
			rect[i].x=0;
			rect[i].y=i*Rect_Height;
		}
		SDL_SetRenderTarget(renderer,texture);
		SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderClear(renderer);
		for(int i=0;i<Rect_Total;i++)
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

		SDL_Rect *clip=NULL;
		double angle=0.0;
		SDL_Point *center=NULL;
		SDL_RendererFlip flip=SDL_FLIP_NONE;

		second("first",color);
		SDL_Rect rect_1={0,0,Rect_Width,Rect_Height};
		SDL_RenderCopyEx(renderer,ttexture,clip,&rect_1,angle,center,flip);
		second("second",color);
		SDL_Rect rect_2={0,Rect_Height,Rect_Width,Rect_Height};
		SDL_RenderCopyEx(renderer,ttexture,clip,&rect_2,angle,center,flip);
		second("three",color);
		SDL_Rect rect_3={0,2*Rect_Height,Rect_Width,Rect_Height};
		SDL_RenderCopyEx(renderer,ttexture,clip,&rect_3,angle,center,flip);
		second("four",color);
		SDL_Rect rect_4={0,3*Rect_Height,Rect_Width,Rect_Height};
		SDL_RenderCopyEx(renderer,ttexture,clip,&rect_4,angle,center,flip);
		
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyRenderer(renderer);
	renderer=NULL;
	SDL_DestroyWindow(window);
	window=NULL;
	SDL_DestroyTexture(texture);
	texture=NULL;
	SDL_DestroyTexture(ttexture);
	ttexture=NULL;
	TTF_CloseFont(font);
	font=NULL;
	TTF_Quit();
	SDL_Quit();
	return 0;
}
