#include <algorithm>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"

using namespace std;

#pragma region Prototypes

enum FileType
{
	NONE = 0,
	PNG = 1,
	BMP = 2,
	JPG = 3
};

// starts up SDL and creates window
bool Init( void );
bool Init( SDL_Window** window, SDL_Surface** windowSurface, string windowName, int screenWidth, int screenHeight );
bool Init( SDL_Window** window, SDL_Renderer** renderer, string windowName, int screenWidth, int screenHeight );

// loads media
bool LoadMedia( void );
bool LoadMedia( SDL_Surface** image, SDL_Surface** windowSurface, string imagePath );
bool LoadMedia( SDL_Texture** texture, SDL_Renderer** renderer, string imagePath );

// frees resources and closes window
void Close( void );
void Close( SDL_Window** window, SDL_Surface** windowSurface );
void Close( SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture );

SDL_Surface* LoadOptimizedPngSurface( SDL_Surface* windowSurface, string path );
SDL_Surface* LoadOptimizedBmpSurface( SDL_Surface* windowSurface, string path );

SDL_Texture* LoadPngAsTexture( SDL_Renderer* renderer, string path );

void StretchSurface( SDL_Surface* src, SDL_Surface* dest, int x, int y, int w, int h );

void LoadTextures( vector<Texture> textures, vector<string> fileNames );

FileType GetExtensionFromPath( std::string path );

#pragma endregion

