#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include "../include/Warning.hpp"
#include "../include/Verbose.hpp"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

#define DESIRED_FPS 60

void saveImageToFile(const char*, const unsigned char*, const unsigned int);
double waitNextFrame(float, VerboseStream&);
void quit(int, SDL_Renderer*, SDL_Window*, VerboseStream&);

int main(int argc, char **argv){
	VerboseStream::setEnabled(argc, argv);
	VerboseStream vout;
	if(!fs::exists("sprites")){
		vout << "Creating image files." << std::endl;
		if(!fs::create_directory("sprites")){
			std::cerr << "Couldn't create the `sprites` directory.";
			quit(EXIT_FAILURE, nullptr, nullptr, vout);
		}

		for(size_t i = 0; i < allDumpSize-2; i++){
			try{
				saveImageToFile(("sprites/cat0"+ std::to_string(i+1) +".bmp").c_str(), allDump[i], allDumpLen[i]);
			}catch(const Warning& warn){
				std::cerr << "Warning: could not save the `cat0"+ std::to_string(i+1) +".bmp` file in the `sprites` folder." << std::endl;
			}
		}

		try{
			saveImageToFile("sprites/cat10.bmp", allDump[9], allDumpLen[9]);
			saveImageToFile("sprites/other1.bmp", allDump[10], allDumpLen[10]);
		}catch(const Warning& warn){
			std::cerr << "Warning: could not save the `cat10.bmp` nor the `other1.bmp` files in the `sprites` folder." << std::endl;
		}

	}
	
	vout << "Initializing random seed and SDL." << std::endl;
	std::srand(std::time(nullptr));
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		
		quit(EXIT_FAILURE, nullptr, nullptr, vout);
	}

	vout << "Initializing the window and renderer.";
	SDL_Window* win;
	SDL_Renderer* render;
	if(SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &win, &render) < 0){
		std::cerr << "Failed to create renderer or the window: " << SDL_GetError() << std::endl;
		
		quit(EXIT_FAILURE, render, win, vout);
	}

	vout << "Preparing to enter the main loop." << std::endl;
	SDL_SetRenderDrawColor(render, 15, 15, 15, SDL_ALPHA_OPAQUE);
	
	Cat myCat(Pos(WIN_WIDTH/2-50, WIN_HEIGHT/2-50), (uint)100);
	vout << "Entering the main loop.\n\n" << std::endl;
	while(true){
		const Uint64 frameStart = SDL_GetPerformanceCounter();

		vout << "Checking events.\t\t\t(main loop)" << std::endl;
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			if(ev.type == SDL_QUIT)
				quit(EXIT_SUCCESS, nullptr, nullptr, vout);

		vout << "Displaying the cat to the window.\t(main loop)" << std::endl;
		myCat.display(render);

		vout << "Rendering then clearing the window.\t(main loop)" << std::endl;;
		SDL_RenderPresent(render);
		SDL_RenderClear(render);
		
		vout << "Waiting until next frame.\t\t(main loop)" << std::endl;
		try {
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / (float)SDL_GetPerformanceFrequency(), vout);		//how long the frame lasted
		} catch(const Warning& w) {
			std::cerr << "A warning was thrown: " << w.what() << std::endl;
		}
	}

	quit(EXIT_SUCCESS, render, win, vout);
}









/**
 * Saves the `data` dump as an actual file.
 * @param filename The name of the file to save to.
 * @param data An hex dump created by `xxd`.
 * @param size The size of `data`
 * @throw A `Warning` if the function can't create the file.
 */
void saveImageToFile(const char* filename, const unsigned char* data, const unsigned int size) noexcept(false) {
	std::ofstream file(filename, std::ios::binary);
	if(file)
		file.write(reinterpret_cast<const char*>(data), size);
	else
		throw Warning("Failed to save " + std::string(filename) + ".");
	file.close();
}


/**
 * Wait the appropriate time until next frame and returns delta in second/frame (delta is the maximum time that can be allowed)
 * @param lasted How many seconds took the last frame to run
 */
double waitNextFrame(float lasted, VerboseStream& verboseOut) noexcept(false){
	constexpr double delta = 1 / (double)DESIRED_FPS;

	const Uint32 timeTaken = (delta - lasted) * 1000;

	if (timeTaken >= 0)
		SDL_Delay(timeTaken);
	else
		throw Warning("The frame ended " + std::to_string(timeTaken) + "ms late (it took " + std::to_string(lasted * 1000) + "ms to run).");
	

	verboseOut << "Frame completed in " << timeTaken << "ms.\t\t(main loop/waitNextFrame())\n" << std::endl;
	return delta;
}

void quit(int exitCode, SDL_Renderer* renderer, SDL_Window* window, VerboseStream& verbose){
	verbose << "\n\nDestroying all instances." << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	verbose << "Program " << ((exitCode == EXIT_SUCCESS)? "completed with success!" : "ended due to an error.")  << std::endl;
	exit(exitCode);
}