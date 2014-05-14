#include <string>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#pragma region Globals

const string IMAGE_PATH = "C:\\dev\\projects\\SDL_g\\Debug\\images\\man_1.png";

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

