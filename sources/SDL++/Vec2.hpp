/*
** SDL++, 2020
** Vec2.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL_rect.h>

#include <algorithm>
#include <cmath>
#include <ostream>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

namespace details {

	template <typename T>
	struct Vec2Base {
		T x = T(), y = T();
	};

}

////////////////////////////////////////////////////////////////////////////////

template<typename T, class Base = details::Vec2Base<T>>
struct Vec2 : public Base
{
	constexpr Vec2() = default;

	constexpr Vec2(T x, T y)
	: Base{x, y}
	{}

	template <typename U>
	constexpr Vec2(U x, U y)
	: Base{static_cast<T>(x), static_cast<T>(y)}
	{}

	template <typename U>
	constexpr Vec2(const Vec2<U> &v)
	: Base{static_cast<T>(v.x), static_cast<T>(v.y)}
	{}

	Vec2(const Vec2 &) noexcept = default;
	Vec2(Vec2 &&) noexcept = default;

	////////////////////////////////////////////////////////////////////////////

	template<typename A>
	static constexpr Vec2 fromPolar(A alpha, T radius) {
		return Vec2{radius * std::cos(alpha), radius * std::sin(alpha)};
	}

	Vec2 clamped(const SDL_Rect &box) const
	{
		Vec2 r(Base::x, Base::y);
		r.clamp(box);
		return r;
	}

	void clamp(const SDL_Rect &box)
	{
		Base::x = std::min(std::max(Base::x, box.x), box.x + box.w);
		Base::y = std::min(std::max(Base::y, box.y), box.y + box.h);
	}

	T length() const
	{
		return std::sqrt(sqLength());
	}

	T sqLength() const
	{
		return Base::x * Base::x + Base::y * Base::y;
	}

	bool null() const
	{
		return Base::x == T(0.0L) || Base::y == T(0.0L);
	}

	Vec2 normalized() const
	{
		auto r = Vec2{Base::x, Base::y};
		r.normalize();
		return r;
	}

	void normalize()
	{
		if (null())
			return;

		*this /= length();
	}

	////////////////////////////////////////////////////////////////////////////

	Vec2 &operator =(const Vec2&) noexcept = default;
	Vec2 &operator =(Vec2&&) noexcept = default;

	constexpr Vec2 operator -() const { return Vec2{-Base::x, -Base::y}; }

	constexpr Vec2 operator +(const Vec2 &v) const { return {Base::x + v.x, Base::y + v.y}; }
	constexpr Vec2 operator -(const Vec2 &v) const { return {Base::x - v.x, Base::y - v.y}; }
	constexpr Vec2 operator *(T v) const { return {Base::x * v, Base::y * v}; }
	constexpr Vec2 operator /(T v) const { return {Base::x / v, Base::y / v}; }

	Vec2 &operator +=(const Vec2 &v)
	{
		Base::x += v.x;
		Base::y += v.y;
		return *this;
	}

	Vec2 &operator -=(const Vec2 &v)
	{
		Base::x -= v.x;
		Base::y -= v.y;
		return *this;
	}

	Vec2 &operator *=(T v)
	{
		Base::x *= v;
		Base::y *= v;
		return *this;
	}

	Vec2 &operator /=(T v)
	{
		Base::x /= v;
		Base::y /= v;
		return *this;
	}

	constexpr bool operator ==(const Vec2 &other) const { return (Base::x == other.x && Base::y == other.y); }
	constexpr bool operator !=(const Vec2 &other) const { return !(*this == other); }

	friend std::ostream &operator <<(std::ostream& stream, const Vec2 &v) {
		return stream << "Vec2(" << v.x << ", " << v.y << ")";
	}
};

using Vec2i = Vec2<int, SDL_Point>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

////////////////////////////////////////////////////////////////////////////////

}