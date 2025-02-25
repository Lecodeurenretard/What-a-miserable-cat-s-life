#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include "../include/Verbose.hpp"

bool saveImageToFile(const char*, const unsigned char*, const unsigned int);
bool saveImageToFile(const char*, uint8_t);
void saveImgs(const char*, size_t, size_t);

double waitNextFrame(float);
void waitEvent(SDL_EventType, SDL_Renderer* = nullptr, SDL_Window* = nullptr);
void waitKeyPress(SDL_KeyCode, SDL_Renderer* = nullptr, SDL_Window* = nullptr);

void quit(int = EXIT_SUCCESS, SDL_Renderer* = nullptr, SDL_Window* = nullptr);

int main(int argc, char **argv){
	const bool stepByStep = argc >= 3 && (std::string(argv[2]) == std::string("-s") || std::string(argv[2]) == std::string("--step"));
	const bool showDestination = argc >= 4 && (std::string(argv[3]) == std::string("-d") || std::string(argv[3]) == std::string("--destination") || std::string(argv[3]) == std::string("--show-destination"));
	VerboseStream::setEnabled(argc, argv);

	if(!fs::exists("sprites")){
		vout << "Creating image files." << std::endl;
		if(!fs::create_directory("sprites")){
			std::cerr << "Couldn't create the `sprites` directory.";
			quit(EXIT_FAILURE);
		}

		saveImgs("dog"	, 0						, dogCount	);
		saveImgs("cat"	, dogCount				, catCount	);
		saveImgs("other", dogCount + catCount	, otherCount);
	}
	
	vout << "Initializing random seed and SDL." << std::endl;
	std::srand(std::time(nullptr));
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::cerr << "Failed to initialize SDL.\n(SDL last error: " << SDL_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE);
	}

	vout << "Initializing the window and renderer." << std::endl;
	SDL_Window* win;
	SDL_Renderer* render;
	if(SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &win, &render) < 0){
		std::cerr << "Failed to create renderer or the window.\n(SDL last error: " << SDL_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE, render, win);
	}

	vout << "Preparing to enter the main loop." << std::endl;
	SDL_SetRenderDrawColor(render, 15, 15, 15, SDL_ALPHA_OPAQUE);
	
	Cat myCat(Pos::SCREEN_CENTER, 100, 1, 2);
	Dog myDog(Pos(100, 100));
	while(true){
		const Uint64 frameStart = SDL_GetPerformanceCounter();
	
		vout << "Checking events.\t\t\t\t(main loop)" << std::endl;
		SDL_Event ev;
		while (SDL_PollEvent(&ev)){
			switch (ev.type){
				case SDL_QUIT:
					quit(EXIT_SUCCESS, render, win);

				default:
					break;
			}
		}
		
		vout << "Moving and displaying animals to the window.\t(main loop)" << std::endl;
		myCat.move();
		myDog.move();
		myCat.draw(render, showDestination);
		myDog.draw(render, showDestination);

		vout << "Rendering then clearing the window.\t\t(main loop)" << std::endl;
		SDL_RenderPresent(render);
		
		if(stepByStep)
			waitKeyPress(SDLK_RIGHT, render, win);
		SDL_RenderClear(render);
		
		vout << "Waiting until next frame.\t\t\t(main loop)" << std::endl;
		if(!stepByStep)
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / (float)SDL_GetPerformanceFrequency());		//For how long the frame lasted
		else
			VerboseStream::newLine(vout);
	}

	quit(EXIT_SUCCESS, render, win);
}









/**
 * Saves the `data` dump as an actual file.
 * @param filename The name of the file to save to.
 * @param data An hex dump created by `xxd`.
 * @param size The size of `data`
 * @return If the function succeeded in saving the file.
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
 */
bool saveImageToFile(const char* filename, uint8_t imageIndex) {
	if(imageIndex >= allDumpSize || imageIndex >= allDumpLenSize)	//The index is OoB
		return false;
	return saveImageToFile(filename, allDump[imageIndex], allDumpLen[imageIndex]);
}

/**
 * Saves `count` images with names name1, name2, ... and begins at index `offset` in `allDump`.  
 * Assumes the `sprites` directory exists.
 */
void saveImgs(const char* name, size_t offset, size_t count){
	for(uint8_t i = 0; i < count; i++){
		std::string filename = 
			"sprites/"
			+ std::string(name)
			+ std::string(
				(i < 9)?			//If the sprite number is less than 10, add a trailing `0` (ie: "cat9" -> "cat09")
					"0" + std::to_string(i+1)
					: std::to_string(i+1)
			) 
			+".bmp";
		
		if(!saveImageToFile(filename.c_str(), (uint8_t)(i + offset)))
			wout << "Failed to save `" << filename << '`' << std::endl;
	}
}


/**
 * Wait the appropriate time until next frame and returns delta in second/frame (delta is the maximum time that can be allowed)
 * @param lasted How many seconds took the last frame to run
 */
double waitNextFrame(float lasted){
	constexpr double delta = 1 / (double)DESIRED_FPS;

	const Uint32 timeTaken = (delta - lasted) * 1000;

	if (timeTaken >= 0){
		SDL_Delay(timeTaken);
		vout << "Frame completed in " << timeTaken << "ms.\t\t(main loop/waitNextFrame())\n" << std::endl;
	} else {
		wout << "The frame ended " << timeTaken << "ms late (it took " << lasted * 1000 << "ms to run)." << std::endl;
	}
	return delta;
}

/**
 * Wait until the specified event or quit if the corresponding event is fired.
 * @param eventType The event type to wait.
 * @param r The renderer to free if the user quits.
 * @param win The window to free if the user quits
 */
void waitEvent(SDL_EventType eventType, SDL_Renderer* r /*= nullptr*/, SDL_Window* win /*= nullptr*/){
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != eventType){
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
void waitKeyPress(SDL_KeyCode key, SDL_Renderer* r /*= nullptr*/, SDL_Window* win /*= nullptr*/){
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != SDL_KEYDOWN && ev.key.keysym.sym != key){
		if(ev.type == SDL_QUIT)
			quit(EXIT_SUCCESS, r, win);
		SDL_PollEvent(&ev);
	}
}

/**
 * Free variables, quit SDL and exits with code `exitCode`
 */
void quit(int exitCode/*=EXIT_SUCCESS*/, SDL_Renderer* renderer/*=nullptr*/, SDL_Window* window/*=nullptr*/){
	vout << VerboseStream::newLine << VerboseStream::newLine << "Destroying the renderer and the window, then quitting SDL." << std::endl;
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	vout << "Program " << ((exitCode == EXIT_SUCCESS)? "completed with success!" : "ended due to an error.")  << std::endl;
	exit(exitCode);
}