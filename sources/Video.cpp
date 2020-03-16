/*
** SDL++, 2020
** Video.cpp
*/

#include "SDL++/Video.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

void showMessageBox(uint32_t flags, std::string &&title, std::string &&message)
{
	SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), nullptr);
}

void showMessageBox(uint32_t flags, std::string &&title, std::string &&message, const SDL::Window &parent)
{
	SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), parent.ptr());
}

////////////////////////////////////////////////////////////////////////////////

}