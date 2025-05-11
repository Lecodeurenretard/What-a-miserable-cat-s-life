#include "../include/Cat.hpp"
#include "../include/Verbose.hpp"

//functions defined in mainFunctions.cpp
extern bool saveImageToFile(const char*, const unsigned char*, const unsigned int);
extern bool saveImageToFile(const char*, uint8_t);
extern void saveImgs(const char*, size_t, size_t);

extern double waitNextFrame(float);
extern void waitEvent(SDL_EventType, SDL_Renderer* = nullptr, SDL_Window* = nullptr);
extern void waitKeyPress(SDL_KeyCode, SDL_Renderer* = nullptr, SDL_Window* = nullptr);

extern void quit(int = EXIT_SUCCESS, SDL_Renderer* = nullptr, SDL_Window* = nullptr);

int main(int argc, const char** argv) {
	cmd::Parser parser({
		{"-s",				cmd::Type::boolean},
		{"--step",			cmd::Type::boolean},

		{"-d",				cmd::Type::boolean},
		{"--dbg-infos",		cmd::Type::boolean},

		{"-v",				cmd::Type::boolean},
		{"--verbose",		cmd::Type::boolean},

		{"-m",				cmd::Type::boolean},
		{"--follow-mouse",	cmd::Type::boolean},

		{"--catCount",		cmd::Type::unsignedInteger},
		{"--dogCount",		cmd::Type::unsignedInteger},
	});
	const cmd::Parser::parseReturn_t arguments = parser.parse(argc, argv);

	VerboseStream::setEnabled(arguments);
	const bool stepByStep		= std::get<bool>(arguments.at("-s")) || std::get<bool>(arguments.at("--step"));
	const bool showDestination	= std::get<bool>(arguments.at("-d")) || std::get<bool>(arguments.at("--dbg-infos"));
	const bool followMouse		= std::get<bool>(arguments.at("-m")) || std::get<bool>(arguments.at("--follow-mouse"));

	const uint catCount			= arguments.contains("--catCount")?
		std::max(std::min(std::get<uint>(arguments.at("--catCount")), 255u), 1u)		//keeping the value between 1 and 255
		: 5;
	const uint dogCount			= arguments.contains("--dogCount")?
		std::max(std::min(std::get<uint>(arguments.at("--dogCount")), 255u), 1u)
		: 2;
	

	if(!fs::exists("sprites")) {
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
	SDL_Window* win;
	SDL_Renderer* render;
	if(SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN, &win, &render) < 0) {
		std::cerr << "Failed to create renderer or the window.\n(SDL last error: " << SDL_GetError() << ')' << std::endl;
		
		quit(EXIT_FAILURE, render, win);
	}
	SDL_SetWindowTitle(win, "Evolution killed the cat");

	vout << "Preparing to enter the main loop." << std::endl;
	SDL_SetRenderDrawColor(render, 15, 15, 15, SDL_ALPHA_OPAQUE);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);		//enable transparency

	Cat::generateCats(catCount, nullptr, Pos::SCREEN_CENTER, 50, 2);
	Dog::generateDogs(dogCount, nullptr, Pos(100, 100));
	
	//randomizing health
	for(ID i = 0; i < CATLIST_SIZE; i++) {
		if(Cat::catList[i] == nullptr)
			continue;

		for(uint8_t lifeAdded = 0; lifeAdded < randInt(0, 2); lifeAdded++)
			Cat::catList[i]->incrementHealth();
	}

	while(true) {
		const Uint64 frameStart = SDL_GetPerformanceCounter();
	
		vout << "Checking events.\t\t\t\t(main loop)" << std::endl;
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
				case SDL_QUIT:
					quit(EXIT_SUCCESS, render, win);

				default:
					break;
			}
		}

		vout << "Checking collision for each cat.\t\t(main loop)" << std::endl;
		for(uint8_t i = 0; i < CATLIST_SIZE; i++) {
			if(Cat::catList[i] == nullptr)
				continue;
			Cat* const currentCat = Cat::catList[i];

			currentCat->move(followMouse);
			currentCat->draw(render, nullptr, showDestination);
			currentCat->handleCollisions();
		}

		vout << "Moving and drawing animals to the renderer.\t(main loop)" << std::endl;
		for(uint8_t i = 0; i < Dog::getLowestID(); i++) {
			Dog* const currentDog = Dog::dogList[i];

			currentDog->move(followMouse);
			currentDog->draw(render, nullptr, showDestination);
		}

		vout << "Rendering then clearing the window.\t\t(main loop)" << std::endl;
		SDL_RenderPresent(render);
		
		if(stepByStep) {
			vout << "Waiting until next frame.\t\t\t(main loop)" << std::endl;
			waitKeyPress(SDLK_RIGHT, render, win);
			VerboseStream::newLine(vout);
		} else {
			vout << "Waiting until next frame.\t\t\t(main loop)" << std::endl;
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / (float)SDL_GetPerformanceFrequency());		//For how long the frame lasted
		}
		
		SDL_RenderClear(render);
	}

	quit(EXIT_SUCCESS, render, win);
}