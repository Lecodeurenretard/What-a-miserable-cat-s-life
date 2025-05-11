//functions useful only in the `main()` function.
#include "../include/Verbose.hpp"
#include "../include/Cat.hpp"


/**
 * Free variables, quit SDL and exits with code `exitCode`
 */
void quit(int exitCode = EXIT_SUCCESS, SDL_Renderer* renderer = nullptr, SDL_Window* window = nullptr) noexcept {
	vout << VerboseStream::newLine << VerboseStream::newLine << "Destroying the renderer and the window, then quitting SDL." << std::endl;
	
	Cat::freeCatList();
	Dog::freeDogList();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	vout << "Program " << ((exitCode == EXIT_SUCCESS)? "completed with success!" : "ended due to an error.")  << std::endl;
	exit(exitCode);
}

/**
 * Saves the `data` dump as an actual file.
 * @param filename The name of the file to save to.
 * @param data An hex dump created by `xxd`.
 * @param size The size of `data`
 * @return If the function succeeded in saving the file.
 * @throw Exception can be thrown in `ofstream.write()` and `ofstream.read()`.
 */
bool saveImageToFile(const char* filename, const unsigned char* data, const unsigned int size) {
	std::ofstream file(filename, std::ios::binary);
	if(file)
		file.write(reinterpret_cast<const char*>(data), size);
	file.close();

	return (bool)file;
}

/**
 * Saves the `data` dump as an actual file.
 * @param filename The name of the file to save to.
 * @param imageIndex The index of the wanted image in `allDump`
 * @return If the function succeeded in saving the file.
 * @throw Exception can be thrown by `saveImageToFile()`.
 */
bool saveImageToFile(const char* filename, uint8_t imageIndex) {
	if(imageIndex >= allDumpSize || imageIndex >= allDumpLenSize)	//The index is OoB
		return false;
	return saveImageToFile(filename, allDump[imageIndex], allDumpLen[imageIndex]);
}

/**
 * Saves `count` images with names name1, name2, ... and begins at index `offset` in `allDump`.  
 * Assumes the `sprites` directory exists.
 * @throw Exception can be thrown by `saveImageToFile()`.
 */
void saveImgs(const char* name, size_t offset, size_t count) {
	for(uint8_t i = 0; i < count; i++) {
		std::string filename = 
			"sprites/"
			+ std::string(name)
			+ ((i < 9)? "0" : "\0")			//If the sprite number is less than 10, add a trailing `0` (ie: "cat9" -> "cat09")
			+ std::to_string(i+1)
			+ ".bmp";
		
		if(!saveImageToFile(filename.c_str(), (uint8_t)(i + offset)))
			wout << "Failed to save `" << filename << '`' << std::endl;
	}
}


/**
 * Wait the appropriate time until next frame and returns delta in second/frame (delta is the maximum time that can be allowed)
 * @param lasted How many seconds took the last frame to run
 */
double waitNextFrame(float lasted) noexcept {
	constexpr double delta = 1 / (double)DESIRED_FPS;

	const int timeTaken = (delta - lasted) * 1000;

	if (timeTaken >= 0) {
		SDL_Delay(timeTaken);
		vout << "Frame completed in " << timeTaken << "ms.\t\t\t(main loop/waitNextFrame())\n" << std::endl;
	} else {
		wout << "The frame ended " << -timeTaken << "ms late (took " << lasted * 1000 << "ms to run)." << std::endl;
	}
	return delta;
}

/**
 * Wait until the specified event or quit if the corresponding event is fired.
 * @param eventType The event type to wait.
 * @param r The renderer to free if the user quits.
 * @param win The window to free if the user quits
 */
void waitEvent(SDL_EventType eventType, SDL_Renderer* r = nullptr, SDL_Window* win = nullptr) noexcept {
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != eventType) {
		if(ev.type == SDL_QUIT)
			quit(EXIT_SUCCESS, r, win);
		SDL_PollEvent(&ev);
	}
}

/**
 * Wait until the specified key is entered or quit if the corresponding event is fired.
 * @param key The key code (one of `SDLK_*` macros) of the key to wait.
 * @param r The renderer to free if the user quits.
 * @param win The window to free if the user quits
 */
void waitKeyPress(SDL_KeyCode key, SDL_Renderer* r = nullptr, SDL_Window* win = nullptr) noexcept {
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != SDL_KEYDOWN && ev.key.keysym.sym != key) {
		if(ev.type == SDL_QUIT)
			quit(EXIT_SUCCESS, r, win);
		SDL_PollEvent(&ev);
	}
}