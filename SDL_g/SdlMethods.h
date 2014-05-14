#include <string>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#pragma region Prototypes

// starts up SDL and creates window
bool init( SDL_Window** window, SDL_Surface** windowSurface, string windowName, int screenWidth, int screenHeight );

// loads media
bool loadMedia( SDL_Surface** image, SDL_Surface** windowSurface, string imagePath );

// frees resources and closes window
void close( SDL_Window** window, SDL_Surface** windowSurface );

SDL_Surface* LoadOptimizedPngSurface( SDL_Surface* windowSurface, string path );
SDL_Surface* LoadOptimizedBmpSurface( SDL_Surface* windowSurface, string path );

void StretchSurface( SDL_Surface* src, SDL_Surface* dest, int x, int y, int w, int h );

#pragma endregion

