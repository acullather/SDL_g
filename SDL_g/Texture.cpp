#include "Texture.h"

#pragma region Constructor & Destructor

Texture::Texture( void )
{
	_Renderer = NULL;
	_Texture = NULL;
	_Width = 0;
	_Height = 0;
}

Texture::Texture( std::string path )
{
	Texture();
	LoadImageFromFile( path );
}

Texture::Texture( SDL_Renderer* renderer, std::string path )
{
	Texture();
	LoadImageFromFile( path );
	SetRenderer( renderer );
}

Texture::~Texture( void )
{
	FreeResources();
}

#pragma endregion

#pragma region Public Methods

SDL_Texture** Texture::GetTexture( void ) { return &_Texture; }

void Texture::FreeResources( void )
{
	if ( _Texture != NULL )
	{
		SDL_DestroyTexture( _Texture );
	}
	_Texture = NULL;
	_Renderer = NULL;
	_Width = 0;
	_Height = 0;
}

bool Texture::LoadImageFromFile( std::string path )
{
	FreeResources();

	if ( path.length() < 1 )
	{
		printf( "No path provided to load image from file" );
		return false;
	}

	SDL_Texture* newTexture = NULL;

	SDL_Surface* surface = IMG_Load( path.c_str() );
	if ( surface == NULL )
	{
		printf( "Could not load image '%s'. SDL Error: %s", path.c_str(), SDL_GetError() );
		return false;
	}
	else
	{
		_Width = surface->w;
		_Height = surface->h;

		SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0x00, 0xFF, 0xFF ) );
		newTexture = SDL_CreateTextureFromSurface( *_Renderer, surface );
		if ( newTexture == NULL )
		{
			printf( "Could not create texture from surface. SDL Error: %s", SDL_GetError() );
		}
	}

	SDL_FreeSurface( surface );
	_Texture = newTexture;
	
	return _Texture != NULL;
}

void Texture::RenderTexture( int x, int y )
{
	SDL_Rect rect = { x, y, _Width, _Height };
	SDL_RenderCopy( *_Renderer, _Texture, NULL, &rect );
}

void Texture::SetRenderer( SDL_Renderer* renderer )
{
	_Renderer = &renderer;
}

#pragma endregion

#pragma region Properties

int Texture::getHeight( void )
{
	return _Height;
}

int Texture::getWidth( void )
{
	return _Width;
}

#pragma endregion