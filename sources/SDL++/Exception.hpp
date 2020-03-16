/*
** SDL++, 2020
** Exception.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Error.hpp"

#include <SDL2/SDL_error.h>

#include <stdexcept>
#include <string>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Exception : public std::exception
{
public:
	explicit Exception(std::string &&function)
	{
		m_what = "Function: '" + function + "', SDL error: " + Error::get();
		Error::clear();
	}

	const char *what() const noexcept override { return m_what.c_str(); }

private:
	std::string m_what;
};

////////////////////////////////////////////////////////////////////////////////

}