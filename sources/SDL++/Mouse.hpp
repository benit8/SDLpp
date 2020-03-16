/*
** SDL++, 2020
** Mouse.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"
#include "Surface.hpp"
#include "Video.hpp"

#include <SDL2/SDL_mouse.h>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Mouse
{
public:
	static void setRelative(bool enabled)
	{
		if (SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) < 0)
			throw Exception("SDL_SetRelativeMouseMode");
	}

	static bool isRelative()
	{
		return SDL_GetRelativeMouseMode() == SDL_TRUE;
	}

	static void warpInWindow(const Window &window, const Vec2i &position)
	{
		SDL_WarpMouseInWindow(window.ptr(), position.x, position.y);
	}

	static void warpGlobal(const Vec2i &position)
	{
		SDL_WarpMouseGlobal(position.x, position.y);
	}

private:
	Mouse() = default;
};

////////////////////////////////////////////////////////////////////////////////

class Cursor
{
public:
	explicit Cursor(SDL_SystemCursor id)
	: m_cursor(SDL_CreateSystemCursor(id))
	{
		if (!m_cursor)
			throw Exception("SDL_CreateSystemCursor");
	}

	Cursor(const uint8_t *data, const uint8_t *mask, const Vec2i &size, const Vec2i &hot)
	: m_cursor(SDL_CreateCursor(data, mask, size.x, size.y, hot.x, hot.y))
	{
		if (!m_cursor)
			throw Exception("SDL_CreateCursor");
	}

	Cursor(const Surface &surface, const Vec2i &hot)
	: m_cursor(SDL_CreateColorCursor(surface.ptr(), hot.x, hot.y))
	{
		if (!m_cursor)
			throw Exception("SDL_CreateColorCursor");
	}

	~Cursor()
	{
		if (m_cursor)
			SDL_FreeCursor(m_cursor);
	}

	Cursor(Cursor &&other) noexcept
	{
		*this = std::move(other);
	}

	////////////////////////////////////////////////////////////////////////////

	void set() const
	{
		if (m_cursor)
			SDL_SetCursor(m_cursor);
	}

	////////////////////////////////////////////////////////////////////////////

	static void show()
	{
		const auto value = SDL_ShowCursor(SDL_ENABLE);
		if (value != SDL_ENABLE)
			throw Exception("SDL_ShowCursor");
	}

	static void hide()
	{
		const auto value = SDL_ShowCursor(SDL_DISABLE);
		if (value != SDL_DISABLE)
			throw Exception("SDL_ShowCursor");
	}

	static bool visible()
	{
		const auto value = SDL_ShowCursor(SDL_QUERY);
		if (value < 0)
			throw Exception("SDL_ShowCursor");
		return value == SDL_ENABLE;
	}

	////////////////////////////////////////////////////////////////////////////

	Cursor &operator =(Cursor &&other) noexcept
	{
		if (m_cursor != other.m_cursor) {
			m_cursor = other.m_cursor;
			other.m_cursor = nullptr;
		}
		return *this;
	}

private:
	SDL_Cursor *m_cursor = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}