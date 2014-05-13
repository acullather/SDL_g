#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>

#include "Physics.h"

using namespace std;

#pragma region Global Variables

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const double VELOCITY = 10.0;
const double ACCELERATION = 1.0;
const int FRAMES_PER_SEC = 15;
const string IMAGE_PATH = "C:\\dev\\projects\\SDL_g\\Debug\\images\\man_1.png";

double _time = 0.0;
double _acceleration = 0;

// the window handling the rendering
SDL_Window* _window = NULL;
// the surface contained by the window
SDL_Surface* _windowSurface = NULL;
// the image we will load and show on the screen
SDL_Surface* _image = NULL;

#pragma endregion

#pragma region Prototypes

// starts up SDL and creates window
bool init( void );

// loads media
bool loadMedia( void );

// frees resources and closes window
void close( void );

SDL_Surface* LoadOptimizedPngSurface( string path );
SDL_Surface* LoadOptimizedBmpSurface( string path );

void StretchSurface( SDL_Surface* src, SDL_Surface* dest, int x, int y, int w, int h );

#pragma endregion

int main( int argc, char* args[] )
{
	if ( !init() )
	{
		printf( "Failed to initialize.\n" );
		std::cin.ignore();
		return -1;
	}
	
	if ( !loadMedia() )
	{
		printf( "Failed to load media.\n" );
		std::cin.ignore();
		return -2;
	}

	bool quit = false;
	SDL_Event e;
	int pos_x = 0;
	int pos_y = 0;

	clock_t start_t = clock();
	clock_t end_t;
	
	double ti = 0.0;
	double vi = VELOCITY;
	double pvi = vi;
	double vf = vi;

	while ( !quit )
	{
		while ( SDL_PollEvent( &e ) != 0 )
		{
			if ( e.type == SDL_QUIT )
			{
				quit = true;
			}
			else if ( e.type == SDL_KEYDOWN )
			{
				#pragma region Key Press Polling Switch
				switch ( e.key.keysym.sym )
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_UP:
						pos_y -= VELOCITY;
						if (pos_y < 0)
						{
							pos_y = SCREEN_HEIGHT - _image->h;
						}
						break;
					case SDLK_DOWN:
						pos_y += VELOCITY;
						if (pos_y > SCREEN_HEIGHT - _image->h)
						{
							pos_y = 0;
						}
						break;
					case SDLK_LEFT:
						pos_x -= VELOCITY;
						if (pos_x < 0)
						{
							pos_x = SCREEN_WIDTH - _image->w;
						}
						break;
					case SDLK_RIGHT:
						pos_x += VELOCITY;
						if (pos_x > SCREEN_WIDTH - _image->w)
						{
							pos_x = 0;
						}
						break;
					default:
						break;
				}
				#pragma endregion
			}
		}

		end_t = clock();
		_time = (end_t - start_t); // / (double)(CLOCKS_PER_SEC / 1000);
		if (fmod(_time, ceil((1/(double)FRAMES_PER_SEC)*1000)) <= 1.0)
		{
			pos_x += (int)Physics::Displacement(vi, vf, _time - ti);
		}

		if (pos_x > SCREEN_WIDTH - _image->w)
		{
			pos_x = 0;
		}
		
		if ( fmod(_time, 1000.0) == 0.0 || _time == 0.0 )
		{
			printf("t=%f:(x=%i,y=%i):vi=%f;vf=%f;a=%f;\n", _time, pos_x, pos_y, vi, vf, _acceleration);
		}

		SDL_FillRect(_windowSurface, NULL, 0x000000);		
		StretchSurface( _image, _windowSurface, pos_x, pos_y, _image->w, _image->h);
		SDL_UpdateWindowSurface( _window );

		pvi = vi;
		vi = vf;
		vf = Physics::Velocity(pvi, _acceleration, _time - ti);
		if (vf > 100.0)
		{
			vi = 100.0;
			vf = 100.0;
		}

		ti = _time;
	}

	close();
	std::cout << "Press <ENTER> to exit..." << std::endl;
	std::cin.ignore();
	return 0;
}

#pragma region Function Definitions

bool init( void )
{
	bool success = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		_window = SDL_CreateWindow( 
			"Test Window", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN);

		if ( _window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;

			if ( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "Could not initialize SDL_image! SDL_Error: %s\n", SDL_GetError() );
				success = false;
			}
			
			_windowSurface = SDL_GetWindowSurface( _window );
			if ( _windowSurface == NULL )
			{
				printf( "Window's surface could not be created! SDL_Error: %s\n", SDL_GetError() );
				success = false;
			}
		}
	}

	return success;
}

bool loadMedia( void )
{
	bool success = true;

	//_image = LoadOptimizedBmpSurface( "images/man_1.bmp" );
	_image = LoadOptimizedPngSurface( IMAGE_PATH );
	if ( _image == NULL )
	{
		printf( "Could not load image! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	return success;
}

void close( void )
{
	SDL_FreeSurface( _windowSurface );
	_windowSurface = NULL;

	SDL_DestroyWindow( _window );
	_window = NULL;

	SDL_Quit();
}

SDL_Surface* LoadOptimizedPngSurface( string path )
{
	SDL_Surface* dest = NULL;
	SDL_Surface* src = IMG_Load( path.c_str() );

	if ( src == NULL )
	{
		printf( "Error loading picture '%s'. SDL_Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		dest = SDL_ConvertSurface( src, _windowSurface->format, NULL );
		if ( dest == NULL )
		{
			printf( "Error converting to optimized surface for picture '%s'. SDL_Error: %s\n", path.c_str(), SDL_GetError() );
		}
		SDL_FreeSurface( src );
	}
	return dest;
}

SDL_Surface* LoadOptimizedBmpSurface( string path )
{
	SDL_Surface* s = NULL;

	SDL_Surface* ls = SDL_LoadBMP( path.c_str() );
	if ( ls == NULL )
	{
		printf( "Unable to load image from %s. SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		s = SDL_ConvertSurface( ls, _windowSurface->format, NULL );
		if ( s == NULL )
		{
			printf( "Unable to convert image to surface. SDL Error: %s\n", SDL_GetError() );
		}
		SDL_FreeSurface( ls );
	}
	return s;
}

void StretchSurface( SDL_Surface* src, SDL_Surface* dest, int x, int y, int w, int h )
{
	SDL_Rect stretchRect;
	stretchRect.x = x;
	stretchRect.y = y;
	stretchRect.w = w;
	stretchRect.h = h;
	SDL_BlitScaled( src, NULL, dest, &stretchRect );
}

#pragma endregion
