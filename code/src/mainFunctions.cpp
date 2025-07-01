//functions useful only in the `main()` function.
#include "../include/mainFunctions.hpp"

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

	return static_cast<bool>(file);
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
 * Create the `sprite` directory.
 */
void createSpriteDir(void) {
	vout << "Creating image files." << std::endl;
	if(!fs::create_directory("sprites")) {
		std::cerr << "Couldn't create the `sprites` directory." << std::endl;
		quit(EXIT_FAILURE);
	}

	//extract all images
	size_t sum(0);
	for(size_t i = 0; i < allNamesSize; i++) {
		saveImgs(allNames[i], sum , countList[i]);
		sum += countList[i];
	}
}


/**
 * Wait the appropriate time until next frame.
 * @param lasted How many seconds took the last frame to run
 */
void waitNextFrame(double lasted) {
	if(lasted < 0)
		throw std::invalid_argument("The time took for the last frame must be positive, got " + std::to_string(lasted) + "s");

	constexpr double DESIRED_TIME = 1.0/DESIRED_FPS;	//The time took if the framerate was DESIRED_FPS
	deltaTime = lasted;

	const int timeLeft = std::round(DESIRED_TIME - deltaTime);

	if(timeLeft < 0) {
		wout << "The frame ended " << -timeLeft * 1000 << "ms late (took " << lasted * 1000 << "ms to run)." << std::endl;
		return;
	}

	vout << "Frame completed in " << lasted * 1000 << "ms (finished " << timeLeft * 1000 << "ms early).\t(main loop/waitNextFrame())" << std::endl;
	SDL_Delay(timeLeft);
	deltaTime += timeLeft;	//could do deltaTime = DESIRED_TIME but I find this  more intuitive
}

/**
 * Wait until the specified event or quit if the corresponding event is fired.
 * @param eventType The event type to wait.
 * @param r The renderer to free if the user quits.
 * @param win The window to free if the user quits
 */
void waitEvent(SDL_EventType eventType, SDL_Renderer* r /*= nullptr*/, SDL_Window* win /*= nullptr*/) noexcept {
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
void waitKeyPress(SDL_KeyCode key, SDL_Renderer* r /*= nullptr*/, SDL_Window* win /*= nullptr*/) noexcept {
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != SDL_KEYDOWN && ev.key.keysym.sym != key) {
		if(ev.type == SDL_QUIT)
			quit(EXIT_SUCCESS, r, win);
		SDL_PollEvent(&ev);
	}
}

/**
 * Initialize the program.
 */
void init(SDL_Window** winPtr, SDL_Renderer** renPtr) {
	vout << "Initializing RNG, SDL and SDL_ttf." << std::endl;
	std::srand(std::time(nullptr));

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "Failed to initialize SDL.\n(SDL last error: " << SDL_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE);
	}

	if(TTF_Init() < 0) {
		std::cerr << "Failed to initialize SDL_ttf.\n(SDL_ttf last error: " << TTF_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE);
	}

	vout << "Initializing the window and renderer." << std::endl;
	if(SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, winPtr, renPtr) < 0) {
		std::cerr << "Failed to create renderer or the window.\n(SDL last error: " << SDL_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE, *renPtr, *winPtr);
	}
	SDL_SetWindowTitle(*winPtr, "Evolution killed the cat");
}

/**
 * Free variables, quit SDL and exits with code `exitCode`
 */
void quit(int exitCode /*= EXIT_SUCCESS*/, SDL_Renderer* renderer /*= nullptr*/, SDL_Window* window /*= nullptr*/) noexcept {
	vout << VerboseStream::newLine;
	vout << "Destroying the renderer and the window, then quitting SDL." << std::endl;

	//catList and dogList, are implicitly destroyed

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	vout << "Program " << ((exitCode == EXIT_SUCCESS)? "completed with success!" : "ended due to an error.")  << std::endl;
	exit(exitCode);
}