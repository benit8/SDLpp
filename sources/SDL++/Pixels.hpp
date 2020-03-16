/*
** SDL++, 2020
** Pixels.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"

#include <SDL2/SDL_pixels.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Color : public SDL_Color
{
public:
	static const Color Transparent, Black, White, Red, Green, Blue, Yellow, Magenta, Cyan;

public:
	constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
	: SDL_Color{r, g, b, a}
	{}

	constexpr Color()
	: SDL_Color{0, 0, 0, 255}
	{}

	constexpr Color(const Color&) = default;
	constexpr Color(Color&&) = default;

	Color(Uint32 raw, const SDL_PixelFormat &format)
	{
		SDL_GetRGBA(raw, &format, &r, &g, &b, &a);
	}

	Color(Uint32 raw, Uint32 format)
	{
		auto f = SDL_AllocFormat(format);
		if (!f)
			throw Exception{"SDL_AllocFormat"};
		SDL_GetRGBA(raw, f, &r, &g, &b, &a);
		SDL_FreeFormat(f);
	}

	////////////////////////////////////////////////////////////////////////////

	Uint32 asUint(const SDL_PixelFormat &format) const
	{
		if (SDL_ISPIXELFORMAT_ALPHA(format.format))
			return SDL_MapRGBA(&format, r, g, b, a);
		else
			return SDL_MapRGB(&format, r, g, b);
	}

	Uint32 asUint(Uint32 format) const
	{
		auto f = SDL_AllocFormat(format);
		if (!f)
			throw Exception{"SDL_AllocFormat"};
		auto raw = asUint(*f);
		SDL_FreeFormat(f);
		return raw;
	}

	////////////////////////////////////////////////////////////////////////////

	Color &operator =(const Color&) = default;
	Color &operator =(Color&&) = default;

	bool operator ==(const Color &c) const {
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	friend std::ostream &operator <<(std::ostream &os, const Color &c) {
		return os << "Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
	}
};

////////////////////////////////////////////////////////////////////////////////

class Pixel
{
public:
	Pixel(void *target, const SDL_PixelFormat &fmt)
	: m_target{target}
	, m_fmt{fmt}
	{}

	Pixel &operator =(const Color &c)
	{
		setColor(c);
		return *this;
	}

	void setColor(const Color &c)
	{
		setRaw(c.asUint(m_fmt));
	}

	Color color() const
	{
		return Color{getRaw(), m_fmt};
	}

	Uint8 r() const { return color().r; }
	Uint8 g() const { return color().g; }
	Uint8 b() const { return color().b; }
	Uint8 a() const { return color().a; }

	void r(Uint8 r)
	{
		auto c = color();
		c.r	= r;
		setColor(c);
	}

	void g(Uint8 g)
	{
		auto c = color();
		c.g	= g;
		setColor(c);
	}

	void b(Uint8 b)
	{
		auto c = color();
		c.b	= b;
		setColor(c);
	}

	void a(Uint8 a)
	{
		auto c = color();
		c.a	= a;
		setColor(c);
	}

private:
	void setRaw(Uint32 raw)
	{
		switch (m_fmt.BytesPerPixel) {
		case 1:
			*static_cast<Uint8*>(m_target) = static_cast<Uint8>(raw);
			break;
		case 2:
			*static_cast<Uint16*>(m_target) = static_cast<Uint16>(raw);
			break;
		case 3:
			if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				static_cast<Uint8*>(m_target)[0] = static_cast<Uint8>((raw >> 16) & 0xFF);
				static_cast<Uint8*>(m_target)[1] = static_cast<Uint8>((raw >> 8) & 0xFF);
				static_cast<Uint8*>(m_target)[2] = static_cast<Uint8>(raw & 0xFF);
			}
			else {
				static_cast<Uint8*>(m_target)[0] = static_cast<Uint8>(raw & 0xFF);
				static_cast<Uint8*>(m_target)[1] = static_cast<Uint8>((raw >> 8) & 0xFF);
				static_cast<Uint8*>(m_target)[2] = static_cast<Uint8>((raw >> 16) & 0xFF);
			}
			break;
		case 4:
			*static_cast<Uint32*>(m_target) = raw;
			break;
		}
	}

	Uint32 getRaw() const
	{
		switch (m_fmt.BytesPerPixel) {
		case 1:
			return *static_cast<Uint8*>(m_target);
		case 2:
			return *static_cast<Uint16*>(m_target);
		case 3:
			if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return static_cast<Uint8*>(m_target)[0] << 16 | static_cast<Uint8*>(m_target)[1] << 8 | static_cast<Uint8*>(m_target)[2];
			else
				return static_cast<Uint8*>(m_target)[0] | static_cast<Uint8*>(m_target)[1] << 8 | static_cast<Uint8*>(m_target)[2] << 16;
		case 4:
			return *static_cast<Uint32*>(m_target);
		}

		return 0;
	}

private:
	void *m_target;
	const SDL_PixelFormat &m_fmt;
};

////////////////////////////////////////////////////////////////////////////////

}