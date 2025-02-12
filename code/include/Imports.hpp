/** all external imports in one file */
#pragma once

//SDL and extentions
#include <SDL.h>

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


#include "Images.hpp"
#include "Utilities.hpp"
#include "utility libs/Color Constants.hpp"