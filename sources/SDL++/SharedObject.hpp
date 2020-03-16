/*
** SDL++, 2020
** SharedObject.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"

#include <SDL2/SDL_loadso.h>

#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

class SharedObject
{
	using SharedObjectHandle = void*;

public:
	using FunctionAddress = void*;

public:
	SharedObject() = default;

	explicit SharedObject(const std::string &filename)
	: m_handle{SDL_LoadObject(filename.c_str())}
	{
		if (!m_handle)
			throw Exception("SDL_LoadObject");
	}

	SharedObject(const SharedObject&) = delete;

	SharedObject(SharedObject &&other) noexcept
	{
		*this = std::move(other);
	}

	~SharedObject()
	{
		if (m_handle)
			SDL_UnloadObject(m_handle);
	}

	////////////////////////////////////////////////////////////////////////////

	FunctionAddress functionPointer(const std::string &fn) const
	{
		const auto address = SDL_LoadFunction(m_handle, fn.c_str());
		if (!address)
			throw Exception("SDL_LoadFunction");
		return address;
	}

	template<typename T>
	T functionPointer(const std::string &fn) const
	{
		return reinterpret_cast<T>(functionPointer(fn));
	}

	////////////////////////////////////////////////////////////////////////////

	SharedObject &operator =(const SharedObject&) = delete;

	SharedObject &operator =(SharedObject &&other) noexcept
	{
		if (m_handle != other.m_handle) {
			if (m_handle)
				SDL_UnloadObject(m_handle);
			m_handle = other.m_handle;
			other.m_handle = nullptr;
		}
		return *this;
	}

private:
	SharedObjectHandle m_handle = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}