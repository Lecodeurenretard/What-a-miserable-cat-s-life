#include "../include/mainFunctions.hpp"

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
	

	if(!fs::exists("sprites"))
		createSpriteDir();
	
	SDL_Window* win			= nullptr;	//will be initialized in init
	SDL_Renderer* render	= nullptr;
	init(&win, &render);

	vout << "Initializing colors." << std::endl;
	SDL_SetRenderDrawColor(render, 15, 15, 15, SDL_ALPHA_OPAQUE);
	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);		//enable transparency
	
	vout << "Initializing cats and dogs." << std::endl;
	Cat::generateCats(catCount, nullptr, Pos::SCREEN_CENTER, 50, 2);
	Dog::generateDogs(dogCount, nullptr, Pos(100, 100));
	
	//randomizing health
	for(ID i = 0; i < CATLIST_SIZE; i++) {
		if(Cat::catList[i].get() == nullptr)
			continue;

		Cat::catList[i]->randomizeHealth(1, 3);
	}

	vout << "Entering main loop." << std::endl;
	while(true) {
		const Uint64 frameStart = SDL_GetPerformanceCounter();
	
		vout << VerboseStream::newLine << "Checking events.\t\t\t\t(main loop)" << std::endl;
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
		for(ID i = 0; i < CATLIST_SIZE; i++) {
			if(Cat::catList[i].get() == nullptr)
				continue;
			Cat& currentCat = *Cat::catList[i];

			currentCat.move(followMouse);
			currentCat.draw(render, nullptr, showDestination);
			currentCat.handleCollisions();
		}

		vout << "Moving and drawing animals to the renderer.\t(main loop)" << std::endl;
		for(ID i = 0; i < Dog::getLowestIndex(); i++) {
			Dog& currentDog = *Dog::dogList[i];

			currentDog.move(followMouse);
			currentDog.draw(render, nullptr, showDestination);
		}

		vout << "Rendering the window.\t\t\t\t(main loop)" << std::endl;
		SDL_RenderPresent(render);
		
		if(stepByStep) {
			vout << "Waiting until next frame.\t\t\t(main loop)" << std::endl;
			waitKeyPress(SDLK_RIGHT, render, win);
		} else {
			vout << "Waiting until next frame.\t\t\t(main loop)" << std::endl;
			waitNextFrame((SDL_GetPerformanceCounter()-frameStart) / static_cast<double>(SDL_GetPerformanceFrequency()));		//For how long the frame lasted
		}
		
		vout << "Clearing the window.\t\t\t\t(main loop)" << std::endl;
		SDL_RenderClear(render);
	}

	quit(EXIT_SUCCESS, render, win);
}