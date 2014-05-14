#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <time.h>

#include "Physics.h"
#include "SdlMethods.h"

using namespace std;

#pragma region Global Variables

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const double VELOCITY = 10.0;
const double ACCELERATION = 1.0;
const int FRAMES_PER_SEC = 15;

double _time = 0.0;
double _acceleration = 0;


#pragma endregion

int main( int argc, char* args[] )
{
	if ( !init( "This Window", SCREEN_WIDTH, SCREEN_HEIGHT ) )
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

