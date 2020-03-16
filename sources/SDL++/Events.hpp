/*
** SDL++, 2020
** Events.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_version.h>

#include <functional>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace SDL
{

////////////////////////////////////////////////////////////////////////////////

union Event
{
public:
	Uint32                    type;     ///< Event type, shared with all events
	SDL_CommonEvent           common;   ///< Common event data
	SDL_WindowEvent           window;   ///< Window event data
	SDL_KeyboardEvent         key;      ///< Keyboard event data
	SDL_TextEditingEvent      edit;     ///< Text editing event data
	SDL_TextInputEvent        text;     ///< Text input event data
	SDL_MouseMotionEvent      motion;   ///< Mouse motion event data
	SDL_MouseButtonEvent      button;   ///< Mouse button event data
	SDL_MouseWheelEvent       wheel;    ///< Mouse wheel event data
	SDL_JoyAxisEvent          jaxis;    ///< Joystick axis event data
	SDL_JoyBallEvent          jball;    ///< Joystick ball event data
	SDL_JoyHatEvent           jhat;     ///< Joystick hat event data
	SDL_JoyButtonEvent        jbutton;  ///< Joystick button event data
	SDL_JoyDeviceEvent        jdevice;  ///< Joystick device change event data
	SDL_ControllerAxisEvent   caxis;    ///< Game Controller axis event data
	SDL_ControllerButtonEvent cbutton;  ///< Game Controller button event data
	SDL_ControllerDeviceEvent cdevice;  ///< Game Controller device event data
	SDL_AudioDeviceEvent      adevice;  ///< Audio device event data
	SDL_QuitEvent             quit;     ///< Quit request event data
	SDL_UserEvent             user;     ///< Custom event data
	SDL_SysWMEvent            syswm;    ///< System dependent window event data
	SDL_TouchFingerEvent      tfinger;  ///< Touch finger event data
	SDL_MultiGestureEvent     mgesture; ///< Gesture event data
	SDL_DollarGestureEvent    dgesture; ///< Gesture event data
	SDL_DropEvent             drop;     ///< Drag and drop event data
#if SDL_VERSION_ATLEAST(2, 0, 9)
	SDL_SensorEvent           sensor;   /// Sensor event data
	SDL_DisplayEvent          display;  /// Window event data
#endif

	Uint8 padding[56];

	Event()
	{
		static_assert(sizeof(Event) == sizeof(SDL_Event), "Hey, it's likely that the bits in sdl::Event and SDL_Event don't lineup. Please check you are using a supported version of SDL2!!");
		static_assert(offsetof(Event, common.timestamp) == offsetof(SDL_Event, common.timestamp));
		static_assert(offsetof(Event, user.windowID) == offsetof(SDL_Event, user.windowID));

		memset(this, 0, sizeof(SDL_Event));
	}

	Event(const SDL_Event &e)
	: Event{*reinterpret_cast<const Event*>(&e)}
	{}


	static Event &from(SDL_Event &e) { return *reinterpret_cast<Event*>(&e); }
	static Event &from(SDL_Event *e) { return *reinterpret_cast<Event*>(e); }
	static const Event &from(const SDL_Event &e) { return *reinterpret_cast<const Event*>(&e); }
	static const Event &from(const SDL_Event *e) { return *reinterpret_cast<const Event*>(e); }

	operator SDL_Event() const { return *reinterpret_cast<const SDL_Event*>(this); }

	const SDL_Event* ptr() const { return reinterpret_cast<const SDL_Event*>(this); }
	SDL_Event *ptr() { return reinterpret_cast<SDL_Event*>(this); }


	enum class State : int
	{
		Query  = SDL_QUERY,
		Ignore = SDL_IGNORE,
		Enable = SDL_ENABLE,
	};

	bool poll()
	{
		return SDL_PollEvent(ptr());
	}

	void wait()
	{
		if (!SDL_WaitEvent(ptr()))
			throw Exception{"SDL_WaitEvent"};
	}

	void wait(int timeout)
	{
		if (!SDL_WaitEventTimeout(ptr(), timeout))
			throw Exception{"SDL_WaitEventTimeout"};
	}

	void push() const
	{
		if (!SDL_PushEvent(const_cast<SDL_Event*>(ptr())))
			throw Exception{"SDL_PushEvent"};
	}

	void peek()
	{
		if (SDL_PeepEvents(ptr(), 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) < 0)
			throw Exception{"SDL_PeepEvents"};
	}


	bool hasEvents() { return SDL_HasEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); }
	bool hasEvents(Uint32 type) { return SDL_HasEvent(type); }
	bool hasEvents(Uint32 minType, Uint32 maxType) { return SDL_HasEvents(minType, maxType); }
	void pumpEvents() { SDL_PumpEvents(); }
	void flushEvents(Uint32 minType, Uint32 maxType) { SDL_FlushEvents(minType, maxType); }
	void flushEvents() { flushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); }
	void flushEvents(Uint32 type) { flushEvents(type, type); }

	void addEvents(const std::vector<Event> &events, Uint32 minType, Uint32 maxType)
	{
		auto array = const_cast<SDL_Event*>(reinterpret_cast<const SDL_Event*>(&events[0]));
		if (SDL_PeepEvents(array, int(events.size()), SDL_ADDEVENT, minType, maxType) < 0)
			throw Exception{"SDL_PeepEvents"};
	}

	void addEvents(const std::vector<Event> &events) { addEvents(events, SDL_FIRSTEVENT, SDL_LASTEVENT); }
	void addEvents(const std::vector<Event> &events, Uint32 type) { addEvents(events, type, type); }

	std::vector<Event> peekEvents(size_t maxEvents, Uint32 minType, Uint32 maxType)
	{
		auto res = std::vector<Event>(maxEvents);
		auto array = reinterpret_cast<SDL_Event*>(&res[0]);
		if (SDL_PeepEvents(array, int(maxEvents), SDL_PEEKEVENT, minType, maxType) < 0)
			throw Exception{"SDL_PeepEvents"};
		return res;
	}

	std::vector<Event> peekEvents(size_t maxEvents) { return peekEvents(maxEvents, SDL_FIRSTEVENT, SDL_LASTEVENT); }
	std::vector<Event> peekEvents(size_t maxEvents, Uint32 type) { return peekEvents(maxEvents, type, type); }

	std::vector<Event> getEvents(size_t maxEvents, Uint32 minType, Uint32 maxType)
	{
		auto res = std::vector<Event>(maxEvents);
		auto array = reinterpret_cast<SDL_Event*>(&res[0]);
		if (SDL_PeepEvents(array, int(maxEvents), SDL_GETEVENT, minType, maxType) < 0)
			throw Exception{"SDL_PeepEvents"};
		return res;
	}

	std::vector<Event> getEvents(size_t maxEvents) { return getEvents(maxEvents, SDL_FIRSTEVENT, SDL_LASTEVENT); }
	std::vector<Event> getEvents(size_t maxEvents, Uint32 type) { return getEvents(maxEvents, type, type); }


	struct EventFilter
	{
		using func_type = bool (*)(void*, Event&);

		func_type m_filter = nullptr;
		void *m_userdata = nullptr;
		bool m_isWatcher = false;

		EventFilter(func_type filter, void *userdata)
		: m_filter{filter}
		, m_userdata{userdata}
		{}

		explicit EventFilter(func_type filter)
		: m_filter{filter}
		{}

		~EventFilter()
		{
			if (m_isWatcher)
				deleteWatcher();
		}

		static int callFilter(void *data, SDL_Event *event)
		{
			auto filter = static_cast<EventFilter*>(data);
			return filter->m_filter(filter->m_userdata, SDL::Event::from(event));
		}

		void filterQueue()
		{
			SDL_FilterEvents(&callFilter, this);
		}

		void set() { SDL_SetEventFilter(&callFilter, m_userdata); }
		static void unset() { SDL_FilterEvents(nullptr, nullptr); }

		void addWatcher()
		{
			SDL_AddEventWatch(&callFilter, this);
			m_isWatcher = true;
		}

		void deleteWatcher()
		{
			SDL_DelEventWatch(&callFilter, this);
			m_isWatcher = false;
		}
	};

	Event::State eventState(Uint32 type)
	{
		return static_cast<Event::State>(SDL_GetEventState(type));
	}

	void setEventState(Uint32 type, Event::State state)
	{
		SDL_EventState(type, int(state));
	}
};

////////////////////////////////////////////////////////////////////////////////

}