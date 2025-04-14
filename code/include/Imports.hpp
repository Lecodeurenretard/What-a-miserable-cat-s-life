/** all external imports in one file */
#pragma once

//SDL and extentions
#include <SDL2/SDL.h>

//String and string manipulation
#include <iostream>
typedef std::ostream& (*ostream_manipulator)(std::ostream&);

//Error handling
#include <stdexcept>
#include <optional>

//File manipulation
#include <sys/stat.h>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//Misc
#include <algorithm>	//count_if
#include <cstdlib>		//random


//code I wrote
#include "Utilities.hpp"

#if !__has_include(<nlibs/Parser>)
	#error "Run the install script `installSubModules` before compliling."
#endif

#include <nlibs/colors.h>
#include <nlibs/Parser>