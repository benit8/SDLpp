/*
** SDL++, 2020
** Init.cpp
*/

#include "SDL++/SDL.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

bool init(Uint32 flags)
{
	if (SDL_Init(flags) != 0)
		return false;

	atexit(SDL_Quit);
	return true;
}

bool initSubSystem(Uint32 flags)
{
	return SDL_InitSubSystem(flags) == 0;
}

void quit()
{
	SDL_Quit();
}

void quitSubSystem(Uint32 flags)
{
	SDL_QuitSubSystem(flags);
}

bool wasInit(Uint32 flags)
{
	return SDL_WasInit(flags) == flags;
}

////////////////////////////////////////////////////////////////////////////////

}