/** all external imports in one file */
#pragma once

//SDL and extentions
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//String and string manipulation
#include <iostream>

//Error handling
#include <stdexcept>
#include <optional>

//File manipulation
#include <sys/stat.h>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//C replacements
#include <memory>	//unique_ptr
#include <array>
#include <vector>
#include <cstdint>
using uint=unsigned int;

//Misc
#include <algorithm>	//count_if
#include <cstdlib>		//random
#include <concepts>
#include <numbers>		//math constants


//code I wrote
#include "Color_SDL.hpp"

#if !__has_include(<nlibs/Parser>)
	#error "Run the install script `installSubModules` before compliling."
#endif

#include <nlibs/colors.h>
#include <nlibs/Parser>