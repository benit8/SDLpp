/*
** SDL++, 2020
** Clipboard.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL_clipboard.h>

#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

class Clipboard
{
public:
	static bool empty()
	{
		return SDL_HasClipboardText() == SDL_FALSE;
	}

	static std::string get()
	{
		return SDL_GetClipboardText();
	}

	static bool set(const std::string& text)
	{
		return SDL_SetClipboardText(text.c_str()) == 0;
	}

private:
	Clipboard() = default;
};

}