/*
** SDL++, 2020
** Utils.cpp
*/

#include "SDL++/Utils.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

std::string version()
{
	SDL_version v;
	SDL_GetVersion(&v);
	return std::to_string((int)v.major) + '.' + std::to_string((int)v.minor) + '.' + std::to_string((int)v.patch);
}

std::string platform()
{
	return SDL_GetPlatform();
}

////////////////////////////////////////////////////////////////////////////////

namespace System
{
	int CPUCachelineSize()
	{
		return SDL_GetCPUCacheLineSize();
	}

	int CPUCount()
	{
		return SDL_GetCPUCount();
	}

	int RAM()
	{
		return SDL_GetSystemRAM();
	}
}

////////////////////////////////////////////////////////////////////////////////

namespace Power
{
	State state()
	{
		return State(SDL_GetPowerInfo(nullptr, nullptr));
	}

	int batteryRemainingTime()
	{
		int time = 0;
		SDL_GetPowerInfo(&time, nullptr);
		return time;
	}

	int batteryRemainingCharge()
	{
		int charge = 0;
		SDL_GetPowerInfo(nullptr, &charge);
		return charge;
	}
}

////////////////////////////////////////////////////////////////////////////////

}