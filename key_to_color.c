#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init();
void close();


SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;




bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow( "ColorSurface", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}


void close()
{
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
}



int main( int argc, char* args[] )
{
	int BLUE=0,RED=0,GREEN=0;
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		bool quit = false;

		SDL_Event e;


		while( !quit )
		{
			while( SDL_PollEvent( &e ) != 0 )
			{
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				else if( e.type == SDL_KEYDOWN )
				{
					switch( e.key.keysym.sym )
					{
						case SDLK_UP:
							BLUE=255,
								RED=0,
								GREEN=0;
							break;

						case SDLK_DOWN:
							BLUE=0,
								RED=255,
								GREEN=0;
							break;

						case SDLK_LEFT:
							BLUE=0,
								RED=0,
								GREEN=255;
							break;

						case SDLK_RIGHT:
							BLUE=255,
								RED=255,
								GREEN=255;
							break;

						default:
							BLUE=0,
								RED=0,
								GREEN=0;
							break;
					}
				}
			}
			SDL_FillRect(gScreenSurface,NULL,SDL_MapRGB(gScreenSurface->format,RED,GREEN,BLUE));

			SDL_UpdateWindowSurface( gWindow );
		}

	}

	close();

	return 0;
}
