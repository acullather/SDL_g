#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <time.h>

#include <SDL.h>

#include "Physics.h"
#include "SdlMethods.h"
#include "Texture.h"

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

// window that will be created
SDL_Window* window;
// the surface contained by the window
SDL_Surface* windowSurface;
// the image we will load and show on the screen
SDL_Surface* image;
// renderer where textures are drawn
SDL_Renderer* renderer;
SDL_Texture* texture;

#pragma endregion

int main( int argc, char* args[] )
{
	//if ( !init( &window, &windowSurface, "Surface Window", SCREEN_WIDTH, SCREEN_HEIGHT ) )
	if ( !Init( &window, &renderer, "Renderer Window", SCREEN_WIDTH, SCREEN_HEIGHT ) )
	{
		printf( "Failed to initialize.\n" );
		std::cin.ignore();
		return -1;
	}
	
	//if ( !loadMedia( &image, &windowSurface, IMAGE_PATH ) )
	if ( !LoadMedia( &texture, &renderer, IMAGE_PATH ) )
	{
		printf( "Failed to load media.\n" );
		std::cin.ignore();
		return -2;
	}

	Texture t1;
	t1.SetRenderer( renderer );
	t1.LoadImageFromFile( IMAGE_PATH );

	// surfaces
	//int w = image->w;
	//int h = image->h;

	// texture
	int w = 100;
	int h = 100;

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
						pos_y -= (int)VELOCITY;
						if (pos_y < 0)
						{
							pos_y = SCREEN_HEIGHT - h;
						}
						break;
					case SDLK_DOWN:
						pos_y += (int)VELOCITY;
						if (pos_y > SCREEN_HEIGHT - h)
						{
							pos_y = 0;
						}
						break;
					case SDLK_LEFT:
						pos_x -= (int)VELOCITY;
						if (pos_x < 0)
						{
							pos_x = SCREEN_WIDTH - w;
						}
						break;
					case SDLK_RIGHT:
						pos_x += (int)VELOCITY;
						if (pos_x > SCREEN_WIDTH - w)
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

		if (pos_x > SCREEN_WIDTH - w)
		{
			pos_x = 0;
		}
		
		if ( fmod(_time, 1000.0) == 0.0 || _time == 0.0 )
		{
			printf("t=%f:(x=%i,y=%i):vi=%f;vf=%f;a=%f;\n", _time, pos_x, pos_y, vi, vf, _acceleration);
		}

		// surfaces
		//SDL_FillRect( windowSurface, NULL, 0x000000 );
		//StretchSurface( image, windowSurface, pos_x, pos_y, image->w, image->h);
		//SDL_UpdateWindowSurface( window );

		// textures
		// 1. clear screen
		SDL_RenderClear( renderer );
		SDL_Rect rect;
		rect.x = pos_x;
		rect.y = 0;
		rect.w = 100;
		rect.h = 100;
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

		// 2. copy image to renderer
		SDL_RenderCopy( renderer, *(t1.GetTexture()), NULL, &rect );
		//SDL_RenderFillRect( renderer, &rect );

		// 3. update window
		SDL_RenderPresent( renderer );

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

	Close( &window, &windowSurface );
	std::cout << "Press <ENTER> to exit..." << std::endl;
	std::cin.ignore();
	return 0;
}

