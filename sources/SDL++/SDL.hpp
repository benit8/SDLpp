/*
** SDL++, 2020
** SDL.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Audio.hpp"
#include "Clipboard.hpp"
#include "Error.hpp"
#include "Events.hpp"
#include "Exception.hpp"
#include "GameController.hpp"
#include "Haptic.hpp"
#include "Joystick.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Rect.hpp"
#include "Render.hpp"
#include "Pixels.hpp"
#include "SharedObject.hpp"
#include "Surface.hpp"
#include "Texture.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
#include "Vec2.hpp"
#include "Video.hpp"

#include <SDL2/SDL.h>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

bool init(Uint32 flags = SDL_INIT_EVERYTHING);
void quit();

bool initSubSystem(Uint32 flags);
void quitSubSystem(Uint32 flags);

bool wasInit(Uint32 flags);

////////////////////////////////////////////////////////////////////////////////

class Root
{
public:
	Root()
	{
		if (!init())
			throw Exception{"SDL_Init"};

#ifdef SDLPP_USE_SDL_IMAGE
		const int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
		if (IMG_Init(img_flags) != img_flags)
			throw Exception{"IMG_Init"};
		atexit(IMG_Quit);
#endif
	}

	~Root() = default;
};

////////////////////////////////////////////////////////////////////////////////

}