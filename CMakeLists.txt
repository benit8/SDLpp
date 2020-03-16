##
## SDL++, 2019
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0076 NEW)

project(SDL++ VERSION 0.1.0 LANGUAGES CXX)
add_library(SDL++)

target_compile_features(SDL++
PRIVATE
	cxx_std_17
)

target_compile_options(SDL++
PRIVATE
	-fPIC
	-W -Wall -Wextra
	-g3
)

target_include_directories(SDL++
PUBLIC
	./sources
)

target_sources(SDL++
PUBLIC
	sources/SDL++/Audio.hpp
	sources/SDL++/Clipboard.hpp
	sources/SDL++/Error.hpp
	sources/SDL++/Events.hpp
	sources/SDL++/Exception.hpp
	sources/SDL++/GameController.hpp
	sources/SDL++/Haptic.hpp
	sources/SDL++/Joystick.hpp
	sources/SDL++/Keyboard.hpp
	sources/SDL++/Mouse.hpp
	sources/SDL++/Pixels.hpp
	sources/SDL++/Rect.hpp
	sources/SDL++/Render.hpp
	sources/SDL++/SDL.hpp
	sources/SDL++/SharedObject.hpp
	sources/SDL++/Surface.hpp
	sources/SDL++/Texture.hpp
	sources/SDL++/Timer.hpp
	sources/SDL++/Utils.hpp
	sources/SDL++/Vec2.hpp
	sources/SDL++/Video.hpp

PRIVATE
	sources/Color.cpp
	sources/Error.cpp
	sources/Init.cpp
	sources/Utils.cpp
	sources/Video.cpp
)

target_link_libraries(SDL++
PUBLIC
	SDL2
	SDL2_image
)