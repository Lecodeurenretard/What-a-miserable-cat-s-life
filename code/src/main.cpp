#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include "../include/Verbose.hpp"

constexpr uint8_t DESIRED_FPS = 60;

bool saveImageToFile(const char*, const unsigned char*, const unsigned int);
bool saveImageToFile(const char*, uint8_t);

double waitNextFrame(float);
void quit(int = EXIT_SUCCESS, SDL_Renderer* = nullptr, SDL_Window* = nullptr);

int main(int argc, char **argv){
	const bool stepByStep = argc >= 3 && (std::string(argv[2]) == std::string("-s") || std::string(argv[2]) == std::string("--step"));
	VerboseStream::setEnabled(argc, argv);

	if(!fs::exists("sprites")){
		vout << "Creating image files." << std::endl;
		if(!fs::create_directory("sprites")){
			std::cerr << "Couldn't create the `sprites` directory.";
			quit(EXIT_FAILURE);
		}

		for(size_t i = 0; i < allDumpSize-2; i++){
			const std::string filename = "sprites/cat0"+ std::to_string(i+1) +".bmp";
			if(!saveImageToFile(filename.c_str(), i))
				wout << "Failed to save `" << filename << '`' << std::endl;
		}

		
		if(!saveImageToFile("sprites/cat10.bmp", 9))
			wout << "Failed to save `sprites/cat10.bmp`" << std::endl;

		if(!saveImageToFile("sprites/other1.bmp", 10))
			wout << "Failed to save `sprites/other1.bmp`" << std::endl;


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
	myCat.setRandDest();
	vout << "Entering the main loop.\n\n" << std::endl;
	while(!myCat.isAtDest()){
		const Uint64 frameStart = SDL_GetPerformanceCounter();

		vout << "Checking events.\t\t\t(main loop)" << std::endl;
		SDL_Event ev;
		while (SDL_PollEvent(&ev)){
			switch (ev.type){
				case SDL_QUIT:
					quit(EXIT_SUCCESS, render, win);

				default:
					break;
			}
		}
		
		vout << "Displaying the cat to the window.\t(main loop)" << std::endl;
		myCat.move();
		myCat.draw(render);

		vout << "Rendering then clearing the window.\t(main loop)" << std::endl;
		SDL_RenderPresent(render);
		
		if(stepByStep){
			//Wait for the user to press the button
			while(ev.type != SDL_KEYDOWN || ev.key.keysym.sym != SDLK_RIGHT){	
				if(ev.type == SDL_QUIT)
					quit(EXIT_SUCCESS, render, win);
				SDL_PollEvent(&ev);
			}
		}
		SDL_RenderClear(render);
		
		vout << "Waiting until next frame.\t\t(main loop)" << std::endl;
		if(!stepByStep)
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / (float)SDL_GetPerformanceFrequency());		//how long the frame lasted
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
	if(imageIndex >= allDumpSize || imageIndex >= allDumpLenSize)	//the index is OoB
		return false;

	return saveImageToFile(filename, allDump[imageIndex], allDumpLen[imageIndex]);
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