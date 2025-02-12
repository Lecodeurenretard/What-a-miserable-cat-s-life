/** all external imports in one file */
#pragma once

//SDL and extentions
#include <SDL.h>

//String and string manipulation
#include <iostream>

//Error handling
#include <stdexcept>
#include <optional>

//file manipulation
#include <sys/stat.h>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//misc
#include <algorithm>	//count_if
#include <cstdlib>		//random


#include "Images.hpp"
#include "Utilities.hpp"
#include "utility libs/Color Constants.hpp"