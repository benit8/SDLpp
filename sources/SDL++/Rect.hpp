/*
** SDL++, 2020
** Rect.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Vec2.hpp"

#include <SDL2/SDL_rect.h>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class Rect : public SDL_Rect
{
public:
	constexpr Rect()
	: SDL_Rect{0, 0, 0, 0}
	{}

	constexpr Rect(int x, int y, int w, int h)
	: SDL_Rect{x, y, w, h}
	{}

	constexpr Rect(const Vec2i &corner, const Vec2i &size)
	: SDL_Rect{corner.x, corner.y, size.x, size.y}
	{}

	constexpr explicit Rect(const SDL_Rect &r)
	: SDL_Rect{r}
	{}

	Rect(const Rect &) noexcept = default;
	Rect(Rect&&) noexcept = default;

	////////////////////////////////////////////////////////////////////////////

	constexpr int x1() const { return x; }
	constexpr int x2() const { return x + w; }
	constexpr int y1() const { return y; }
	constexpr int y2() const { return y + h; }

	Vec2i botleft()  const { return Vec2i{x1(), y1()}; }
	Vec2i botright() const { return Vec2i{x2(), y1()}; }
	Vec2i topleft()  const { return Vec2i{x1(), y2()}; }
	Vec2i topright() const { return Vec2i{x2(), y2()}; }

	Vec2i size() const { return Vec2i{w, h}; }
	Vec2i center() const { return Vec2i{x + w / 2, y + h / 2}; }

	////////////////////////////////////////////////////////////////////////////

	bool empty() const
	{
		return SDL_RectEmpty(this);
	}

	bool contains(int px, int py) const
	{
		return px >= x1() && px < x2() && py >= y1() && py < y2();
	}

	bool contains(const Vec2i &point) const
	{
		return contains(point.x, point.y);
	}

	bool intersects(const Rect &r) const
	{
		return x1() < r.x2() && x2() > r.x1() && y1() < r.y2() && y2() > r.y1();
	}

	bool intersects(const Vec2i &p1, const Vec2i &p2) const
	{
		auto p1mut = const_cast<Vec2i&>(p1);
		auto p2mut = const_cast<Vec2i&>(p2);

		return SDL_IntersectRectAndLine(this, &p1mut.x, &p1mut.y, &p2mut.x, &p2mut.y);
	}

	Rect inter(const Rect &r) const
	{
		Rect tmp;
		SDL_IntersectRect(this, &r, &tmp);
		return tmp;
	}

	Rect getUnion(const Rect &r) const
	{
		Rect tmp;
		SDL_UnionRect(this, &r, &tmp);
		return tmp;
	}

	////////////////////////////////////////////////////////////////////////////

	static constexpr Rect fromCenter(int cx, int cy, int w, int h)
	{
		return Rect{cx - w / 2, cy - h / 2, w, h};
	}

	static constexpr Rect fromCenter(const Vec2i &center, const Vec2i &size)
	{
		return Rect{center - size / 2, size};
	}

	static constexpr Rect fromCorners(int x1, int y1, int x2, int y2)
	{
		return Rect(x1, y1, x2 - x1, y2 - y1);
	}

	static constexpr Rect fromCorners(const Vec2i &corner1, const Vec2i &corner2)
	{
		return Rect(corner1.x, corner1.y, corner2.x - corner1.x, corner2.y - corner1.y);
	}

	////////////////////////////////////////////////////////////////////////////

	Rect &operator =(const Rect &) noexcept = default;
	Rect &operator =(Rect&&) noexcept = default;

	bool operator ==(const Rect &other) const {
		return SDL_RectEquals(this, &other);
	}

	bool operator !=(const Rect &other) const {
		return !SDL_RectEquals(this, &other);
	}
};

////////////////////////////////////////////////////////////////////////////////

}