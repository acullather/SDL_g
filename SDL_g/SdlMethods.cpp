#include "SdlMethods.h"

#pragma region Globals

#pragma endregion

#pragma region Function Definitions

bool init( SDL_Window** window, SDL_Surface** windowSurface, string windowName, int screenWidth, int screenHeight )
{
	bool success = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		*window = SDL_CreateWindow( 
			windowName.c_str(), 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			screenWidth, 
			screenHeight, 
			SDL_WINDOW_SHOWN);

		if ( *window == NULL )
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
			
			*windowSurface = SDL_GetWindowSurface( *window );
			if ( *windowSurface == NULL )
			{
				printf( "Window's surface could not be created! SDL_Error: %s\n", SDL_GetError() );
				success = false;
			}
		}
	}

	return success;
}

bool loadMedia( SDL_Surface** image, SDL_Surface** windowSurface, string imagePath )
{
	bool success = true;

	*image = LoadOptimizedPngSurface( *windowSurface, imagePath );
	if ( *image == NULL )
	{
		printf( "Could not load image! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	return success;
}

void close( SDL_Window** window, SDL_Surface** windowSurface )
{
	SDL_FreeSurface( *windowSurface );
	windowSurface = NULL;

	SDL_DestroyWindow( *window );
	window = NULL;

	SDL_Quit();
}

SDL_Surface* LoadOptimizedPngSurface( SDL_Surface* windowSurface, string path )
{
	SDL_Surface* dest = NULL;
	SDL_Surface* src = IMG_Load( path.c_str() );

	if ( src == NULL )
	{
		printf( "Error loading picture '%s'. SDL_Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		dest = SDL_ConvertSurface( src, windowSurface->format, NULL );
		if ( dest == NULL )
		{
			printf( "Error converting to optimized surface for picture '%s'. SDL_Error: %s\n", path.c_str(), SDL_GetError() );
		}
		SDL_FreeSurface( src );
	}
	return dest;
}

SDL_Surface* LoadOptimizedBmpSurface( SDL_Surface* windowSurface, string path )
{
	SDL_Surface* s = NULL;

	SDL_Surface* ls = SDL_LoadBMP( path.c_str() );
	if ( ls == NULL )
	{
		printf( "Unable to load image from %s. SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		s = SDL_ConvertSurface( ls, windowSurface->format, NULL );
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

