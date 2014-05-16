#include "SdlMethods.h"

#pragma region Globals

#pragma endregion

#pragma region Function Definitions

bool Init( SDL_Window** window, SDL_Surface** windowSurface, string windowName, int screenWidth, int screenHeight )
{
	bool success = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
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
			return false;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;

			if ( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "Could not initialize SDL_image! SDL_Error: %s\n", SDL_GetError() );
				return false;
			}
			
			*windowSurface = SDL_GetWindowSurface( *window );
			if ( *windowSurface == NULL )
			{
				printf( "Window's surface could not be created! SDL_Error: %s\n", SDL_GetError() );
				return false;
			}
		}
	}

	return success;
}

bool Init( SDL_Window** window, SDL_Renderer** renderer, string windowName, int screenWidth, int screenHeight )
{
	bool success = true;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
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
			return false;
		}
		else
		{
			*renderer = SDL_CreateRenderer( *window, -1, SDL_RENDERER_ACCELERATED );
			if ( *renderer == NULL )
			{
				printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
				return false;
			}

			SDL_SetRenderDrawColor( *renderer, 0xFF, 0xFF, 0xFF, 0xFF );

			// initiate PNG loading
			int imgFlags = IMG_INIT_PNG;
			if ( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "Could not initialize SDL_image! SDL_Error: %s\n", SDL_GetError() );
				return false;
			}
		}
	}

	return success;
}

bool LoadMedia( SDL_Surface** image, SDL_Surface** windowSurface, string imagePath )
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

bool LoadMedia( SDL_Texture** texture, SDL_Renderer** renderer, string imagePath )
{
	*texture = LoadPngAsTexture( *renderer, imagePath );
	if ( *texture == NULL )
	{
		printf( "Failed to load image.\n" );
		return false;
	}
	return true;
}

bool LoadMedia( void )
{
	bool success = true;

	return success;
}

void Close( SDL_Window** window, SDL_Surface** windowSurface )
{
	SDL_FreeSurface( *windowSurface );
	windowSurface = NULL;

	SDL_DestroyWindow( *window );
	window = NULL;

	IMG_Quit();
	SDL_Quit();
}

void Close( SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture )
{
	SDL_DestroyTexture( *texture );
	*texture = NULL;

	SDL_DestroyRenderer( *renderer );
	SDL_DestroyWindow( *window );

	*renderer = NULL;
	*window = NULL;

	IMG_Quit();
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

SDL_Texture* LoadPngAsTexture( SDL_Renderer* renderer, string path )
{
	SDL_Texture* finalTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if ( loadedSurface == NULL )
	{
		printf( "Unable to load image '%s'. SDL_image Error: %s", path.c_str(), SDL_GetError() );
		return NULL;
	}

	finalTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
	if ( finalTexture == NULL )
	{
		printf( "Unable to create texture from image '%s'. SDL_image Error: %s", path.c_str(), SDL_GetError() );
		return NULL;
	}

	SDL_FreeSurface( loadedSurface );

	return finalTexture;
}

void LoadTextures( SDL_Renderer* renderer, vector<Texture>* textures, vector<string>* fileNames )
{
	for (int i = 0; i < fileNames->size(); i++)
	{
		Texture t;

		t.SetRenderer( renderer );
		t.LoadImageFromFile( fileNames->at(i) );

		textures->push_back( t );
	}
}

FileType GetExtensionFromPath( std::string path )
{
	FileType ft;
	if ( path.size() < 4 || path.substr( path.size() - 4, 1 ).c_str() != "." )
	{
		ft = NONE;
	}
	else
	{
		string s = path.substr( path.size() - 3, 3 );
		std::transform(s.begin(), s.end(), s.begin(), ::tolower );
	
		if ( s == "png" )
		{
			ft = PNG;
		}
		else if ( s == "jpg" )
		{
			ft = JPG;
		}
		else if ( s == "bmp" )
		{
			ft = BMP;
		}
		else
		{
			ft = NONE;
		}
	}

	return ft;
}

#pragma endregion

