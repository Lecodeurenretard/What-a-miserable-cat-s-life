#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include "../include/Warning.hpp"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

#define DESIRED_FPS 60

void saveImageToFile(const char*, const unsigned char*, const unsigned int);
double waitNextFrame(float, bool);

void printVerbose(std::string, bool, std::string="", bool=false);
std::ostream& printVerbose(std::ostream&, std::string);

int main(int argc, char **argv){
	/** if the user requested verbose */
	const bool verbose = argc > 1 && (std::string(argv[1]) == "--verbose" || std::string(argv[1]) == "-v");

		if(!fs::exists("sprites")){
			printVerbose("Creating image files.", verbose);
			if(!fs::create_directory("sprites")){
				std::cerr << "Couldn't create the `sprites` directory.";
				return EXIT_FAILURE;
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

	int exitCode;
	bool quitting(false);
	
	printVerbose("Initializing random seed and SDL.", verbose);
	std::srand(std::time(nullptr));
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		
		quitting = true;
		exitCode = EXIT_FAILURE;
	}

	printVerbose("Initializing the window and renderer.", verbose);
	SDL_Window* win;
	SDL_Renderer* render;
	if(SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &win, &render) < 0){
		std::cerr << "Failed to create renderer or the window: " << SDL_GetError() << std::endl;
		
		quitting = true;
		exitCode = EXIT_FAILURE;
	}

	printVerbose("Preparing to enter the main loop.", verbose);
	SDL_SetRenderDrawColor(render, 15, 15, 15, SDL_ALPHA_OPAQUE);
	
	Cat myCat(Pos(WIN_WIDTH/2-50, WIN_HEIGHT/2-50), (uint)100);
	printVerbose("Entering the main loop.", verbose, "\0", true);
	while (!quitting){
		const Uint64 frameStart = SDL_GetPerformanceCounter();

		printVerbose("Checking events.\t\t\t", verbose, "main loop");
		SDL_Event ev;
		while (SDL_PollEvent(&ev)){
			if(ev.type == SDL_QUIT){
				quitting = true;
				exitCode = EXIT_SUCCESS;
				break;
			}
		}
		if(quitting)	//skips the display part
			break;

		printVerbose("Displaying the cat to the window.\t", verbose, "main loop");
		myCat.display(render);

		printVerbose("Rendering then clearing the window.\t", verbose, "main loop");
		SDL_RenderPresent(render);
		SDL_RenderClear(render);
		
		printVerbose("Waiting until next frame.\t\t", verbose, "main loop");
		try {
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / (float)SDL_GetPerformanceFrequency(), verbose);		//how long the frame lasted
		} catch(const Warning& w) {
			std::cerr << "A warning was thrown: " << w.what() << std::endl;
		}
	}

	printVerbose("Destroying all instances.", verbose);
	SDL_DestroyRenderer(render);	render = nullptr;
	SDL_DestroyWindow(win);			win = nullptr;
	SDL_Quit();

	printVerbose(
		(exitCode == EXIT_SUCCESS)? "Program completed with success!" : "Program ended due to an error.",
		verbose
	);
	return exitCode;
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
double waitNextFrame(float lasted, bool displayVerbose) noexcept(false){
	constexpr double delta = 1 / (double)DESIRED_FPS;

	const Uint32 timeTaken = (delta - lasted) * 1000;

	if (timeTaken >= 0)
		SDL_Delay(timeTaken);
	else
		throw Warning("The frame ended " + std::to_string(timeTaken) + "ms late (it took " + std::to_string(lasted * 1000) + "ms to run).");
	

	printVerbose("Frame completed in " + std::to_string(timeTaken) + "ms.\t\t", displayVerbose, "main loop/waitNextFrame()", true);
	return delta;
}

/**
 * Returns the verbose to be printed
 */
std::ostream& printVerbose(std::ostream &out, std::string msg){
	out << STYLE_OS_VERBOSE << "verbose: " << msg << COLOR_FG_RESET << '\n';
	return out;
}

/**
 * Prints With `std::cout` the result of `printVerbose(std::cout)`
 */
void printVerbose(std::string msg, bool displayVerbose, std::string location/*=""*/, bool newLine/*=false*/){
	if(displayVerbose){
		printVerbose(
			std::cout,
			msg + 
			(
				(location != "")?
					"  (" + location + ")" 
					: ""
			)
		);
	
		if(newLine)
			std::cout << std::endl;
		else
			std::cout << std::flush;
	}
}