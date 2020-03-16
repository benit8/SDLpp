/*
** SDL++, 2020
** Keyboard.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Video.hpp"

#include <SDL2/SDL_keyboard.h>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Keyboard
{
public:
	static bool hasScreenSupport() { return SDL_HasScreenKeyboardSupport(); }
	static bool isShown(const Window &win) { return SDL_IsScreenKeyboardShown(win.ptr()); }

	static SDL_Keycode keyFromName(std::string &&name) { return SDL_GetKeyFromName(name.c_str()); }
	static SDL_Keycode keyFromScancode(SDL_Scancode sc) { return SDL_GetKeyFromScancode(sc); }
	static std::string keyName(SDL_Keycode k) { return SDL_GetKeyName(k); }

	static SDL_Scancode scanFromName(std::string &&name) { return SDL_GetScancodeFromName(name.c_str()); }
	static SDL_Scancode scanFromKey(SDL_Keycode k) { return SDL_GetScancodeFromKey(k); }
	static std::string scanName(SDL_Scancode sc) { return SDL_GetScancodeName(sc); }

	static const Uint8 *state()
	{
		return SDL_GetKeyboardState(NULL);
	}

	static SDL_Keymod modState() { return SDL_GetModState(); }
	static void setModState(SDL_Keymod mods) { SDL_SetModState(mods); }

	static void startTextInput() { return SDL_StartTextInput(); }
	static void stopTextInput() { return SDL_StopTextInput(); }
	static bool textInputActive() { return SDL_IsTextInputActive(); }
	static void setTextInputRect(Rect &r) { return SDL_SetTextInputRect(&r); }

private:
	Keyboard();
};

////////////////////////////////////////////////////////////////////////////////

}