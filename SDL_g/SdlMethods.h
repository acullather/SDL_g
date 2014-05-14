#include <string>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#pragma region Prototypes

// starts up SDL and creates window
bool init( string windowName, int screenWidth, int screenHeight );

// loads media
bool loadMedia( void );

// frees resources and closes window
void close( void );

SDL_Surface* LoadOptimizedPngSurface( string path );
SDL_Surface* LoadOptimizedBmpSurface( string path );

void StretchSurface( SDL_Surface* src, SDL_Surface* dest, int x, int y, int w, int h );

#pragma endregion

