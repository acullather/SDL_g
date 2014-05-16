#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>

class Texture
{
public:
	Texture( void );
	Texture( std::string path );
	Texture( SDL_Renderer* renderer, std::string path );

	~Texture( void );

	void FreeResources( void );
	bool LoadImageFromFile( std::string path );
	void RenderTexture( int x, int y );

	void SetRenderer( SDL_Renderer* renderer );

	int getHeight( void );
	int getWidth( void );
	SDL_Texture** GetTexture( void );

private:
	SDL_Texture* _Texture;
	SDL_Renderer** _Renderer;

	int _Height;
	int _Width;
};

