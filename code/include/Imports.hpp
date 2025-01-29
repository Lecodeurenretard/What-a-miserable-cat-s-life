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


//images
/*
#include "libImages.so/sprites/hexDump/cat01.h"
#include "libImages.so/sprites/hexDump/cat02.h"
#include "libImages.so/sprites/hexDump/cat03.h"
#include "libImages.so/sprites/hexDump/cat04.h"
#include "libImages.so/sprites/hexDump/cat05.h"
#include "libImages.so/sprites/hexDump/cat06.h"
#include "libImages.so/sprites/hexDump/cat07.h"
#include "libImages.so/sprites/hexDump/cat08.h"
#include "libImages.so/sprites/hexDump/cat09.h"
#include "libImages.so/sprites/hexDump/cat10.h"
#include "libImages.so/sprites/hexDump/other1.h"
*/